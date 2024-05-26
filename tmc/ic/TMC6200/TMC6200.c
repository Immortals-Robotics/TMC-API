/*******************************************************************************
* Copyright © 2018 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2023 Analog Devices Inc. All Rights Reserved. This software is
* proprietary & confidential to Analog Devices, Inc. and its licensors.
*******************************************************************************/


#include "TMC6200.h"

// => SPI wrapper
#ifdef FEATURE_SPI_ARRAY_TRANSFER
extern void tmc6200_readWriteArray(uint8_t motor, uint8_t *data, size_t length);
#else
extern uint8_t tmc6200_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer);
#endif
// <= SPI wrapper

// spi access
int32_t tmc6200_readInt(uint8_t motor, uint8_t address)
{
	// clear write bit
	address = TMC_ADDRESS(address);

#ifdef FEATURE_SPI_ARRAY_TRANSFER
	uint8_t data[5];

	data[0] = address;
	tmc6200_readWriteArray(motor, &data[0], 5);

	return ((uint32_t)data[1] << 24) | ((uint32_t)data[2] << 16) | (data[3] << 8) | data[4];
#else
	// write address
	tmc6200_readwriteByte(motor, address, false);

	// read data
	int32_t value = tmc6200_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc6200_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc6200_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc6200_readwriteByte(motor, 0, true);

	return value;
#endif
}

void tmc6200_writeInt(uint8_t motor, uint8_t address, int32_t value)
{
	// add write bit
	address = address | TMC6200_WRITE_BIT;

#ifdef FEATURE_SPI_ARRAY_TRANSFER
	uint8_t data[5] = { address, BYTE(value, 3), BYTE(value, 2), BYTE(value, 1), BYTE(value, 0) };
	tmc6200_readWriteArray(motor, &data[0], 5);
#else
	// write address
	tmc6200_readwriteByte(motor, address, false);

	// write value
	tmc6200_readwriteByte(motor, 0xFF & (value>>24), false);
	tmc6200_readwriteByte(motor, 0xFF & (value>>16), false);
	tmc6200_readwriteByte(motor, 0xFF & (value>>8), false);
	tmc6200_readwriteByte(motor, 0xFF & (value>>0), true);
#endif
}
