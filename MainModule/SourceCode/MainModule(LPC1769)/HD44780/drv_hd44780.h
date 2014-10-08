/*
 * drv_hd44780.h
 *
 *  Created on: Jul 11, 2013
 *      Author: nikica
 */

#ifndef DRV_HD44780_H_
#define DRV_HD44780_H_

#include "data_types.h"

typedef Int8U HD44780_XY_DEF;
typedef Int8S HD44780_STRING_DEF;

/* Display address range definition */
#define HD44780_MIN_LINE1_ADD     0x00
#define HD44780_MAX_LINE1_ADD     0x27
#define HD44780_MIN_LINE2_ADD     0x40
#define HD44780_MAX_LINE2_ADD     0x67
/* Max DDRAM Address */
#define HD44780_MAX_DDRAM         0x67
/* Max CGRAM Address */
#define HD44780_MAX_CGRAM         0x3F
/* Address counter auto increment/decrement definition */
#define HD44780_AC_DECREMENT      0
#define HD44780_AC_INCREMENT      1
/* Auto Display shift or cursor move definition */
#define HD44780_CURSOR_MOVE       1
#define HD44780_DISPLAY_SHIFT     1
/* Number of line 1/2 definition */
#define HD44780_1_LINE            1
#define HD44780_2_LINE            0
/* Dot 5x8 or 5x10 definition */
#define HD44780_DOT_5_8           1
#define HD44780_DOT_5_10          0
/* Display On/Off definition */
#define HD44780_DISPLAY_OFF       0
#define HD44780_DISPLAY_ON        1
/* Cursor On/Off definition */
#define HD44780_CURSOR_OFF        0
#define HD44780_CURSOR_ON         1
/* Cursor Blink definition */
#define HD44780_CURSOR_NORM       0
#define HD44780_CURSOR_BLINK      1


typedef struct _HD44780_CTRL_DEF
{
  Int8U  DisplayPos;
  Int16U AC_Direction  : 1;
  Int16U DisplayShift  : 1;
  Int16U Line          : 1;
  Int16U DotMode       : 1;
  Int16U DisplayOn     : 1;
  Int16U CursorOn      : 1;
  Int16U CursorBlink   : 1;
} HD44780_CTRL_DEF;

typedef enum _HD44780_ERROR_CODE_DEF
{
  HD44780_OK = 0,
  HD44780_BUSY_TO_ERROR,
  HD44780_ERROR,
  HD44780_OUT_OF_VISUAL,
} HD44780_ERROR_CODE_DEF;

HD44780_ERROR_CODE_DEF HD44780_PowerUpInit (void);
HD44780_CTRL_DEF * HD44780_GetSetting(void);
HD44780_ERROR_CODE_DEF HD44780_SetMode (void);
HD44780_ERROR_CODE_DEF HD44780_ClearDisplay (void);
HD44780_ERROR_CODE_DEF HD44780_ReturnToHome (void);
HD44780_ERROR_CODE_DEF HD44780_DisplayShift (Boolean DisplayOn, Int8S DisplayShift);
HD44780_ERROR_CODE_DEF HD44780_CursorPosSet (Boolean CursorOn, Boolean CursorBlink, HD44780_XY_DEF X, HD44780_XY_DEF Y);
HD44780_ERROR_CODE_DEF HD44780_RdCGRAM (HD44780_STRING_DEF * CG_Data, Int8U CGRAM_Add);
HD44780_ERROR_CODE_DEF HD44780_WrCGRAM (const HD44780_STRING_DEF * CG_Data, Int8U CGRAM_Add);
HD44780_ERROR_CODE_DEF HD44780_StrShow(HD44780_XY_DEF X, HD44780_XY_DEF Y,  const HD44780_STRING_DEF * DataStr);
Int8U HD44780_GetDisplayPosition (void);

extern void Dly100us(void *arg);



#endif /* DRV_HD44780_H_ */
