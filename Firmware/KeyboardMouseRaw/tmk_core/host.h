/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

#ifndef HOST_H
#define HOST_H

#include <stdint.h>
#include <stdbool.h>
#include "report.h"
#include "host_driver.h"


#ifdef __cplusplus
extern "C" {
#endif


/* host driver */
void host_set_driver(host_driver_t *driver);
host_driver_t *host_get_driver(void);

/* host driver interface */
void host_mouse_send(report_mouse_t *report);


#ifdef __cplusplus
}
#endif

#endif
