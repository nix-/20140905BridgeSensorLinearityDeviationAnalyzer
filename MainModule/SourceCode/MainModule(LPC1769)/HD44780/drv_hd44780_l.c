/*
 * drv_hd44780_l.c
 *
 *  Created on: Jul 11, 2013
 *      Author: nikica
 */

#include 	"drv_hd44780_l.h"

#include    "drv_hd44780_cnfg.h"
#include    "drv_hd44780.h"
#include 	"LPC17xx.h"

#define HD44780_INPUT_SET_MASK        ~((1 << 8) | (1 << 10) | (1 << 11) | (1 << 12))
#define HD44780_OUTPUT_SET_MASK        ((1 << 8) | (1 << 10) | (1 << 11) | (1 << 12))

/* Delay macro */
#define HD44780_BUS_DLY()      volatile int dly = 20; for(;dly;--dly)

/*************************************************************************
 * Function Name: HD44780_IO_Init
 * Parameters: none
 * Return: none
 * Description: Init IO ports directions and level
 *
 *************************************************************************/
void HD44780_IO_Init (void *arg)
{
	LPC_PINCON->PINSEL4 &= ~0x3F30000;
	// TODO: ne se ovie pinovite treba da se smenat !!!!
	//(DATA) 		P2.12 (DB7) , P2.11 (DB6) , P2.10 (DB5) , P2.8 (DB4) -> GPIO
	LPC_GPIO2->FIODIR |= HD44780_OUTPUT_SET_MASK;

	LPC_PINCON->PINSEL3 &= ~0x03F0000;
	//(CONTROL)	P1.25 (E), P1.26 (R/W), P1.24 (RS) -> GPIO
	LPC_GPIO1->FIODIR |= ((1 << 24) | (1 << 25) | (1 << 26));

  // RS Output - Low
  HD44780SetRS(0);
  // E  Output - Low
  HD44780SetE(0);
#if HD4780_WR > 0
  // WR Output - Low
  HD44780SetRW(0);
#endif
}

/*************************************************************************
 * Function Name: HD44780SetRS
 * Parameters: Boolean Data
 * Return: none
 * Description: Set RS signal
 *
 *************************************************************************/
void HD44780SetRS (Boolean Data)
{
  if(Data)
  {
	  LPC_GPIO1->FIOSET = 1<<24; // RS
  }
  else
  {
	  LPC_GPIO1->FIOCLR = 1<<24;
  }
}

/*************************************************************************
 * Function Name: HD44780SetE
 * Parameters: Boolean Data
 * Return: none
 * Description: Set E signal
 *
 *************************************************************************/
void HD44780SetE (Boolean Data)
{
  if(Data)
  {
	  LPC_GPIO1->FIOSET = 1<<25; // E
  }
  else
  {
	  LPC_GPIO1->FIOCLR = 1<<25;
  }
}

/*************************************************************************
 * Function Name: HD44780WrIO
 * Parameters: Int8U Data
 * Return: none
 * Description: Write to HD44780 I/O
 *
 *************************************************************************/
void HD44780WrIO (Int8U Data)
{
  Int32U Tmp;
//
//  Tmp = ((Int32U)Data & 0x03) << 6; // smeneto e poradi sto pinovite se smeneti 6,7,12,13
//  Tmp |= ((Int32U)Data & 0x0C) << 10; // 10 pati zatoa sto prvite 2 bita se xx00
//
//	printf("%d", Data);
  Tmp  = (( (Int32U)Data & 0x0e ) << 9);
  Tmp |= (( (Int32U)Data & 0x01 ) << 8);

  // Set Direction
  LPC_GPIO2->FIODIR  |= HD44780_OUTPUT_SET_MASK;
#if HD4780_WR > 0
  HD44780SetRW(0);
#endif
  // Write Data
  // Clock E
  HD44780SetE(1);

  LPC_GPIO2->FIOSET |=  Tmp & HD44780_OUTPUT_SET_MASK;
  LPC_GPIO2->FIOCLR |= ~Tmp & HD44780_OUTPUT_SET_MASK;
  HD44780_BUS_DLY();
  HD44780SetE(0);
}

#if HD4780_WR > 0
/*************************************************************************
 * Function Name: HD44780SetRW
 * Parameters: Boolean Data
 * Return: none
 * Description: Set R/W signal
 *
 *************************************************************************/
void HD44780SetRW (Boolean Data)
{
  if(Data)
  {
	  LPC_GPIO1->FIOSET = 1<<26;
  }
  else
  {
	  LPC_GPIO1->FIOCLR = 1<<26;
  }
}

/*************************************************************************
 * Function Name: HD44780RdIO
 * Parameters: none
 * Return: Int8U
 * Description: Read from HD44780 I/O
 *
 *************************************************************************/
Int8U HD44780RdIO (void *arg)
{
Int8U Data;
  // Set Direction
LPC_GPIO2->FIODIR   &= HD44780_INPUT_SET_MASK;
  HD44780SetRW(1);
  // Read Data
  HD44780SetE(1);
  HD44780_BUS_DLY();
  Data  = ((LPC_GPIO2->FIOPIN >> 9) & 0xe); // nixson: pinovi se P0.4, P0.6, P0.8, P0.10
  Data |= ((LPC_GPIO2->FIOPIN >> 8) & 0x1);

  HD44780SetE(0);
  // Clock E
  return Data;
}
#endif


