#include <ps2.h>

/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
PS2 mouse(2, 3);

/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */
void mouse_init()
{
  unsigned  char ret;
  Serial.print("Reset (0xFF). Ack: ");
  mouse.write(0xff);  // reset
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("ID, XX: ");
  ret = mouse.read();  // blank */
  Serial.print(ret, HEX);   Serial.print(", ");
  ret = mouse.read();  // blank */
  Serial.println(ret, HEX);
  
  // 0xF3 (Set Sample Rate) - The mouse responds with "acknowledge" (0xFA) 
  // then reads one more byte from the host. 
  // The mouse saves this byte as the new sample rate. 
  //After receiving the sample rate, the mouse again responds with "acknowledge" (0xFA) 
  //and resets its movement counters. 
  // Valid sample rates are 10, 20, 40, 60, 80, 100, and 200 samples/sec. 
/*  
  Serial.print("Set Sample Rate (0xF3). Ack: ");
  mouse.write(0xf3);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);
*/

  Serial.print("Send 0xE8. Ack: ");
  mouse.write(0xE8);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 3. Ack: ");
  mouse.write(3);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 0xE6. Ack: ");
  mouse.write(0xE6);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 0xE6. Ack: ");
  mouse.write(0xE6);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 0xE6. Ack: ");
  mouse.write(0xE6);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 0xE9. Ack: ");
  mouse.write(0xE9);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Get Status byte: ");
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Get Resolution: ");
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);
  
  
  Serial.print("Reports per sec: ");
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);
  
/*
  Serial.print("Set Sample Rate (0xF3). Ack: ");
  mouse.write(0xf3);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send Rate 200. Ack: ");
  mouse.write(200);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Set Sample Rate (0xF3). Ack: ");
  mouse.write(0xf3);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send Rate 100. Ack: ");
  mouse.write(100);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Set Sample Rate (0xF3). Ack: ");
  mouse.write(0xf3);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send Rate 80. Ack: ");
  mouse.write(80);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);
*/
  Serial.print("Get Device ID (0xF2). Ack: ");
  mouse.write(0xf2);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("ID: ");
  ret = mouse.read();  // blank */
  Serial.println(ret, HEX);



  Serial.print("Send 0xE8. Ack: ");
  mouse.write(0xE8);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 3. Ack: ");
  mouse.write(3);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);

  Serial.print("Send 0xE6. Ack: ");
  mouse.write(0xE6);  // 
  ret = mouse.read();  // ack byte
  Serial.println(ret, HEX);



//  mouse.write(0xea);  // stream mode
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
}

void setup()
{
  while(!Serial){};
  Serial.begin(9600);
  mouse_init();
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop1()
{
    mouse.write(0xff);  // reset

}
void loop()
{
  unsigned char mstat;
  char mx;
  char my;
  char extra;

  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();
//extra  = mouse.read();
  /* send the data back up */
  Serial.print("Yo Xo Ys Xs 1 M R L");
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.print("\t 0 0 5th 4th Z=");
//  Serial.print(extra, DEC);

  Serial.println();
  delay(200);  /* twiddle */
}
