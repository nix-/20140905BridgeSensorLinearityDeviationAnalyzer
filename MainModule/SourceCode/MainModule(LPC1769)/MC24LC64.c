#include <string.h>
#include "LPC17xx.h"			/* LPC17xx Peripheral Registers */
#include "i2c.h"
#include "MC24LC64.h"

#define MC24LC64_ADDR 0xA0		/* I2C address of the 24LC64 EEPROM */


/*****************************************************************************
** Function name:	MC24LC64_Wr
**
** Descriptions:	Function for writing data to MC24LC64
**
** parameters:		Address:	EEPROM Start Address for writing
** 								(0x00 - 0x1FFF)
** 					Buffer:		Pointer to data for write
** 					Length:		Data Length (1 - 32)
**
** Returned value:	Status
**
*****************************************************************************/
uint32_t MC24LC64_Wr(uint32_t Address, uint8_t *Buffer, uint32_t Length)
{
	if(Address > 0x1FFF)
		return FALSE;

	if(Buffer == 0)
		return FALSE;

	if((Length < 1) || (Length > 32))
		return FALSE;

	/* In order to start the I2CEngine, the all the parameters
  	  must be set in advance, including I2CWriteLength, I2CReadLength,
  	  I2CCmd, and the I2cMasterBuffer which contains the stream
  	  command/data to the I2c slave device.
  	  (1) If it's a I2C write only, the number of bytes to be written is
  	  I2CWriteLength, I2CReadLength is zero, the content will be filled
  	  in the I2CMasterBuffer.
  	  (2) If it's a I2C read only, the number of bytes to be read is
  	  I2CReadLength, I2CWriteLength is 0, the read value will be filled
  	  in the I2CMasterBuffer.
  	  (3) If it's a I2C Write/Read with repeated start, specify the
  	  I2CWriteLength, fill the content of bytes to be written in
  	  I2CMasterBuffer, specify the I2CReadLength, after the repeated
  	  start and the device address with RD bit set, the content of the
  	  reading will be filled in I2CMasterBuffer index at
  	  I2CMasterBuffer[I2CWriteLength+2].

  	  e.g. Start, DevAddr(W), WRByte1...WRByteN, Repeated-Start, DevAddr(R),
  	  RDByte1...RDByteN Stop. The content of the reading will be filled
  	  after (I2CWriteLength + two devaddr) bytes. */

	/* Write SLA(W), address and one data byte */

	memcpy((void*)(&I2CMasterBuffer[3]), (void*)Buffer, Length);

	I2CWriteLength = Length + 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MC24LC64_ADDR;
	I2CMasterBuffer[1] = ((uint8_t)(Address >> 8)) & 0x1F;	/* address MSB */
	I2CMasterBuffer[2] = (uint8_t)Address;					/* address LSB */

	uint32_t fin_state = I2CEngine();

	return TRUE;
}

/*****************************************************************************
** Function name:	MC24LC64_Rd
**
** Descriptions:	Function for reading from MC24LC64
**
** parameters:		Address:	EEPROM Start Address for reading
** 								(0x00 - 0x1FFF)
** 					Buffer:		Pointer to data from read
** 					Length:		Data Length (1 - 32)
** Returned value:	Status
**
*****************************************************************************/
uint32_t MC24LC64_Rd(uint32_t Address, uint8_t *Buffer, uint32_t Length)
{
	if(Address > 0x1FFF)
		return FALSE;

	if(Buffer == 0)
		return FALSE;

	if((Length < 1) || (Length > 32))
		return FALSE;

	I2CWriteLength = 3;
	I2CReadLength = Length;
	I2CMasterBuffer[0] = MC24LC64_ADDR;
	I2CMasterBuffer[1] = ((uint8_t)(Address >> 8)) & 0x1F;	/* address MSB */
	I2CMasterBuffer[2] = (uint8_t)Address;					/* address LSB */
	I2CMasterBuffer[3] = MC24LC64_ADDR | RD_BIT;

	while(I2CEngine() == I2CSTATE_SLA_NACK);

	memcpy((void*)Buffer, (void*)I2CSlaveBuffer, Length);

	return TRUE;
}

/*****************************************************************************
** Function name:	MC24LC64_Init
**
** Descriptions:	Function for MC24LC64 communication channel (I2C)
** 					initialization
**
** parameters:		None
** Returned value:	Status
**
*****************************************************************************/
uint32_t MC24LC64_Init(void)
{
	if(I2CInit((uint32_t)I2CMASTER) == 0)	/* initialize I2c */
		return FALSE;				/* Fatal error */

	return TRUE;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
