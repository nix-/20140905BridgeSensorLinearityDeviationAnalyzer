/*
 * drv_hd44780_l.h
 *
 *  Created on: Jul 11, 2013
 *      Author: nikica
 */

#ifndef DRV_HD44780_L_H_
#define DRV_HD44780_L_H_

#include "data_types.h"

void HD44780SetRS (Boolean Data);
void HD44780SetE (Boolean Data);
#if HD4780_WR > 0
void HD44780SetRW (Boolean Data);
Int8U HD44780RdIO (void *arg);
#endif
void HD44780WrIO (Int8U);
void HD44780_IO_Init (void *arg);



#endif /* DRV_HD44780_L_H_ */
