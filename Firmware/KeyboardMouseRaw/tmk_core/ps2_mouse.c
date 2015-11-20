/*
Copyright 2011,2013 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include<avr/io.h>
#include<util/delay.h>
#include "ps2.h"
#include "ps2_mouse.h"
#include "report.h"
#include "host.h"
#include "timer.h"
#include "print.h"
#include "debug.h"


static uint8_t ps2_mouse_type;
static report_mouse_t mouse_report = {};


uint8_t ps2_get_mouse_type(void){ return ps2_mouse_type;}

/* supports only 3 button mouse at this time */
uint8_t ps2_mouse_init(void) {
    uint8_t rcv, rcv2;
    ps2_mouse_type = PS2_MOUSE_NONE;
    ps2_host_init();

    _delay_ms(1000);    // wait for powering up

    // send Reset
    if(ps2_host_send(0xFF) != PS2_ACK) return ps2_mouse_type;
    // read completion code of BAT  (Basic Assurance Test) 
    rcv = ps2_host_recv_response();
    // read Device ID
    rcv = ps2_host_recv_response();
    if(rcv == STANDARD_ID) ps2_mouse_type = PS2_MOUSE_STANDARD;

    // try to switch to genius "netscroll optical" mouse (5 buttons)
    if(ps2_host_send(0xE8) != PS2_ACK) return ps2_mouse_type;
    if(ps2_host_send(0x03) != PS2_ACK) return ps2_mouse_type;
    if(ps2_host_send(0xE6) != PS2_ACK) return ps2_mouse_type;
    if(ps2_host_send(0xE6) != PS2_ACK) return ps2_mouse_type;
    if(ps2_host_send(0xE6) != PS2_ACK) return ps2_mouse_type;
    if(ps2_host_send(0xE9) != PS2_ACK) return ps2_mouse_type;
    // get status byte
    rcv = ps2_host_recv_response();
    // get resolution
    rcv = ps2_host_recv_response();
    // get Reports per sec
    rcv2 = ps2_host_recv_response();
    if(rcv == 0x33 && rcv2 == 0x55) {
        ps2_mouse_type = PS2_MOUSE_GENIUS5;
    } else {
        //intellimouse compatible? Switch to 3-buttons mode
        //Send commad sequence
		if(ps2_host_send(0xF3) != PS2_ACK) return ps2_mouse_type;
		if(ps2_host_send(200) != PS2_ACK) return ps2_mouse_type;
		if(ps2_host_send(0xF3) != PS2_ACK) return ps2_mouse_type;
		if(ps2_host_send(100) != PS2_ACK) return ps2_mouse_type;
		if(ps2_host_send(0xF3) != PS2_ACK) return ps2_mouse_type;
		if(ps2_host_send(80) != PS2_ACK) return ps2_mouse_type;
		if(ps2_host_send(0xF2) != PS2_ACK) return ps2_mouse_type;
		//Check ID
        rcv = ps2_host_recv_response();
        if(rcv == MIE_ID) {
            ps2_mouse_type = PS2_MOUSE_MIE;
        }
    }

    // Set Resolution 8 count/mm 
	if(ps2_host_send(0xE8) != PS2_ACK) return ps2_mouse_type;
	if(ps2_host_send(0x03) != PS2_ACK) return ps2_mouse_type;

    // Set Scaling 1:1
	if(ps2_host_send(0xE6) != PS2_ACK) return ps2_mouse_type;

    // Set Scaling 1:2
//	if(ps2_host_send(0xE7) != PS2_ACK) return ps2_mouse_type;

    // Set Sample Rate 200
	if(ps2_host_send(0xF3) != PS2_ACK) return ps2_mouse_type;
	if(ps2_host_send(200) != PS2_ACK) return ps2_mouse_type;

    // send Set Remote mode
	if(ps2_host_send(0xF0) != PS2_ACK) return ps2_mouse_type;

    return ps2_mouse_type;
}

void ps2_mouse_task(void)
{
    switch(ps2_mouse_type){
        case PS2_MOUSE_STANDARD: // Standard PS2 mouse
            ps2_standard_mouse_task();
            break;
        case PS2_MOUSE_MIE: // Microsoft Intellimouse Extensions (MIE)
            ps2_mie_mouse_task();
            break;
        case PS2_MOUSE_GENIUS5: //genius "netscroll optical" (5 buttons)
            ps2_genius_mouse_task();
            break;
        case PS2_MOUSE_NONE: // No PS2 mouse present
        default:
            break;
    }
}

#define X_IS_NEG  (mouse_report.buttons & (1<<PS2_MOUSE_X_SIGN))
#define Y_IS_NEG  (mouse_report.buttons & (1<<PS2_MOUSE_Y_SIGN))
#define X_IS_OVF  (mouse_report.buttons & (1<<PS2_MOUSE_X_OVFLW))
#define Y_IS_OVF  (mouse_report.buttons & (1<<PS2_MOUSE_Y_OVFLW))

void ps2_mie_mouse_task(void)
{
 
    /* receives packet from mouse */
    uint8_t rcv;
    uint8_t buttons;
    rcv = ps2_host_send(PS2_MOUSE_READ_DATA);
    if (rcv == PS2_ACK) {
        buttons = ps2_host_recv_response();
        mouse_report.x = ps2_host_recv_response();
        mouse_report.y = ps2_host_recv_response();
        mouse_report.v = ps2_host_recv_response();
        mouse_report.h = 0;
    } else {
        //ps2_mouse: fail to get mouse packet
		ps2_mouse_type = PS2_MOUSE_NONE;
        return;
    }
    mouse_report.buttons = (buttons & PS2_MOUSE_BTN_MASK);


    /* if mouse moves or buttons state changes */
    if (mouse_report.x || mouse_report.y || mouse_report.v ||
            mouse_report.buttons ) {
        // PS/2 mouse data is '9-bit integer'(-256 to 255) which is comprised of sign-bit and 8-bit value.
        // bit: 8    7 ... 0
        //      sign \8-bit/
        //
        // Meanwhile USB HID mouse indicates 8bit data(-127 to 127), note that -128 is not used.
        //
        // This converts PS/2 data into HID value. Use only -127-127 out of PS/2 9-bit.
        mouse_report.x = (mouse_report.x < -127) ? -127 : mouse_report.x;
        mouse_report.x = (mouse_report.x >  127) ?  127 : mouse_report.x;
 
        mouse_report.y = (mouse_report.y < -127) ? -127 : mouse_report.y;
        mouse_report.y = (mouse_report.y >  127) ?  127 : mouse_report.y;

        // invert coordinate of y to conform to USB HID mouse
        mouse_report.y = -mouse_report.y;
        mouse_report.v = -mouse_report.v;
        host_mouse_send(&mouse_report);
    }
    // clear report
    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.v = 0;
    mouse_report.h = 0;
    mouse_report.buttons = 0;
}

void ps2_genius_mouse_task(void)
{
    /* receives packet from mouse */
    uint8_t rcv;
    uint8_t buttons;
    rcv = ps2_host_send(PS2_MOUSE_READ_DATA);
    if (rcv == PS2_ACK) {
        buttons = ps2_host_recv_response();
        mouse_report.x = ps2_host_recv_response();
        mouse_report.y = ps2_host_recv_response();
        mouse_report.v = ps2_host_recv_response();
        mouse_report.h = 0;
    } else {
        //ps2_mouse: fail to get mouse packet
		ps2_mouse_type = PS2_MOUSE_NONE;
        return;
    }
    mouse_report.buttons = (buttons & PS2_MOUSE_BTN_MASK) | 
		((buttons & (1<<PS2_MOUSE_X_OVFLW))>>3) | 
		((buttons & (1<<PS2_MOUSE_Y_OVFLW))>>3);


    /* if mouse moves or buttons state changes */
    if (mouse_report.x || mouse_report.y || mouse_report.v ||
            mouse_report.buttons ) {
 
       // PS/2 mouse data is '9-bit integer'(-256 to 255) which is comprised of sign-bit and 8-bit value.
        // bit: 8    7 ... 0
        //      sign \8-bit/
        //
        // Meanwhile USB HID mouse indicates 8bit data(-127 to 127), note that -128 is not used.
        //
        // This converts PS/2 data into HID value. Use only -127-127 out of PS/2 9-bit.
        mouse_report.x = (mouse_report.x < -127) ? -127 : mouse_report.x;
        mouse_report.x = (mouse_report.x >  127) ?  127 : mouse_report.x;
 
        mouse_report.y = (mouse_report.y < -127) ? -127 : mouse_report.y;
        mouse_report.y = (mouse_report.y >  127) ?  127 : mouse_report.y;

        // invert coordinate of y to conform to USB HID mouse
        mouse_report.y = -mouse_report.y;
        mouse_report.v = -mouse_report.v;
        host_mouse_send(&mouse_report);
    }
    // clear report
    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.v = 0;
    mouse_report.h = 0;
    mouse_report.buttons = 0;
}

void ps2_standard_mouse_task(void)
{
    /* receives packet from mouse */
    uint8_t rcv;
    rcv = ps2_host_send(PS2_MOUSE_READ_DATA);
    if (rcv == PS2_ACK) {
        mouse_report.buttons = ps2_host_recv_response();
        mouse_report.x = ps2_host_recv_response();
        mouse_report.y = ps2_host_recv_response();
    } else {
        //ps2_mouse: fail to get mouse packet
        return;
    }
 
    /* if mouse moves or buttons state changes */
    if (mouse_report.x || mouse_report.y ||
            ((mouse_report.buttons) & PS2_MOUSE_BTN_MASK)) {
 
        // PS/2 mouse data is '9-bit integer'(-256 to 255) which is comprised of sign-bit and 8-bit value.
        // bit: 8    7 ... 0
        //      sign \8-bit/
        //
        // Meanwhile USB HID mouse indicates 8bit data(-127 to 127), note that -128 is not used.
        //
        // This converts PS/2 data into HID value. Use only -127-127 out of PS/2 9-bit.
        mouse_report.x = (mouse_report.x < -127) ? -127 : mouse_report.x;
        mouse_report.x = (mouse_report.x >  127) ?  127 : mouse_report.x;
 
        mouse_report.y = (mouse_report.y < -127) ? -127 : mouse_report.y;
        mouse_report.y = (mouse_report.y >  127) ?  127 : mouse_report.y;

        // remove sign and overflow flags
        mouse_report.buttons &= PS2_MOUSE_BTN_MASK;

        // invert coordinate of y to conform to USB HID mouse
        mouse_report.y = -mouse_report.y;
        mouse_report.v = 0;
        mouse_report.h = 0;
        host_mouse_send(&mouse_report);
    }
    // clear report
    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.v = 0;
    mouse_report.h = 0;
    mouse_report.buttons = 0;
}

/* PS/2 Mouse Synopsis
 * http://www.computer-engineering.org/ps2mouse/
 *
 * Command:
 * 0xFF: Reset
 * 0xF6: Set Defaults Sampling; rate=100, resolution=4cnt/mm, scaling=1:1, reporting=disabled
 * 0xF5: Disable Data Reporting
 * 0xF4: Enable Data Reporting
 * 0xF3: Set Sample Rate
 * 0xF2: Get Device ID
 * 0xF0: Set Remote Mode
 * 0xEB: Read Data
 * 0xEA: Set Stream Mode
 * 0xE9: Status Request
 * 0xE8: Set Resolution
 * 0xE7: Set Scaling 2:1
 * 0xE6: Set Scaling 1:1
 *
 * Mode:
 * Stream Mode: devices sends the data when it changs its state
 * Remote Mode: host polls the data periodically
 *
 * This code uses Remote Mode and polls the data with Read Data(0xEB).
 *
 * Data format:
 * byte|7       6       5       4       3       2       1       0
 * ----+--------------------------------------------------------------
 *    0|Yovflw  Xovflw  Ysign   Xsign   1       Middle  Right   Left
 *    1|                    X movement
 *    2|                    Y movement
 */
