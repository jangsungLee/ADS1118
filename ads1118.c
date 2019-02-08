#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include "SPIdev.h"

#define ADS1118_CONFIG_REGISTER_SS_NO_EFFECT 0x0000
#define ADS1118_CONFIG_REGISTER_SS_SINGLE 0x8000

#define ADS1118_CONFIG_REGISTER_MUX_DIFF_0_1	0x0000//(default)
#define ADS1118_CONFIG_REGISTER_MUX_DIFF_0_3	0x1000
#define ADS1118_CONFIG_REGISTER_MUX_DIFF_1_3	0x2000
#define ADS1118_CONFIG_REGISTER_MUX_DIFF_2_3	0x3000
#define ADS1118_CONFIG_REGISTER_MUX_SINGLE_0	0x4000
#define ADS1118_CONFIG_REGISTER_MUX_SINGLE_1	0x5000
#define ADS1118_CONFIG_REGISTER_MUX_SINGLE_2	0x6000
#define ADS1118_CONFIG_REGISTER_MUX_SINGLE_3	0x7000

#define ADS1118_CONFIG_REGISTER_PGA_6_144	0x0000
#define ADS1118_CONFIG_REGISTER_PGA_4_096	0x0200
#define ADS1118_CONFIG_REGISTER_PGA_2_048	0x0400//(default)
#define ADS1118_CONFIG_REGISTER_PGA_1_024	0x0600
#define ADS1118_CONFIG_REGISTER_PGA_0_512	0x0800
#define ADS1118_CONFIG_REGISTER_PGA_0_256	0x0A00

#define ADS1118_CONFIG_REGISTER_MODE_CONTINUE	0x0000
#define ADS1118_CONFIG_REGISTER_MODE_SINGLE	0x0100//(default)

#define ADS1118_CONFIG_REGISTER_DR_8_SPS	0x0000
#define ADS1118_CONFIG_REGISTER_DR_16_SPS	0x0020
#define ADS1118_CONFIG_REGISTER_DR_32_SPS	0x0040
#define	ADS1118_CONFIG_REGISTER_DR_64_SPS	0x0060
#define ADS1118_CONFIG_REGISTER_DR_128_SPS	0x0080//(default)
#define ADS1118_CONFIG_REGISTER_DR_250_SPS	0x00A0
#define ADS1118_CONFIG_REGISTER_DR_475_SPS	0x00C0
#define ADS1118_CONFIG_REGISTER_DR_800_SPS	0x00E0

#define ADS1118_CONFIG_REGISTER_TS_MODE_ADC 0x0000// (default)
#define ADS1118_CONFIG_REGISTER_TS_MODE_TEMPERATURE 0x0100

#define ADS1118_CONFIG_REGISTER_PULL_UP_DISENABLE 0x0000
#define ADS1118_CONFIG_REGISTER_PULL_UP_ENABLE 0x0080// (default)

#define ADS1118_CONFIG_REGISTER_NOP_INVALID_DATA_DISUPDATE_CONTENT 0x0000
#define ADS1118_CONFIG_REGISTER_NOP_VALID_DATA_UPDATE_CONTENT 0x0002// (default)
#define ADS1118_CONFIG_REGISTER_NOP_INVALID_DATA_DISUPDATE_CONTENT_1 0x0004
#define ADS1118_CONFIG_REGISTER_NOP_INVALID_DATA_DISUPDATE_CONTENT_2 0x0006

#define ADS1118_CONFIG_REGISTER_RESERVED 0x0001 // Read only and this is olny read purpose register at config register.

int16_t readADC(int channel,int gpio_cs)
{
	static uint16_t data;
	static char tx_buffer[2], rx_buffer[2];

// reset the spi interface of slave to ready.
#ifdef __WIRING_PI_H__
	digitalWrite(gpio_cs, HIGH);
#elif PIGPIO_H
	gpioWrite(gpio_cs, 1);
#endif

	// set the config register.
	data = ADS1118_CONFIG_REGISTER_SS_SINGLE       |
		ADS1118_CONFIG_REGISTER_PGA_6_144      |
		ADS1118_CONFIG_REGISTER_MODE_SINGLE    |
		ADS1118_CONFIG_REGISTER_DR_128_SPS     |
		ADS1118_CONFIG_REGISTER_TS_MODE_ADC    |
		ADS1118_CONFIG_REGISTER_PULL_UP_ENABLE |
		ADS1118_CONFIG_REGISTER_NOP_VALID_DATA_UPDATE_CONTENT;
	    // ============================================================================================
		// cs must be switched between low and high if you want to get 16-Bit Data Transmission Cycle.
	    // ============================================================================================
		   

	switch (channel)
	{
	case 0:
		data |= ADS1118_CONFIG_REGISTER_MUX_SINGLE_0;
		break;
	case 1:
		data |= ADS1118_CONFIG_REGISTER_MUX_SINGLE_1;
		break;
	case 2:
		data |= ADS1118_CONFIG_REGISTER_MUX_SINGLE_2;
		break;
	case 3:
		data |= ADS1118_CONFIG_REGISTER_MUX_SINGLE_3;
		break;
	default:
		fprintf(stderr, "Invalid such a channel number.\n");
		return -1;
	}

// ready to traffic the data
#ifdef __WIRING_PI_H__
	digitalWrite(gpio_cs, LOW);
#elif PIGPIO_H
	gpioWrite(gpio_cs, 0);
#endif
	memcpy(tx_buffer, &data, 2);
	transfer(tx_buffer, rx_buffer, 2);
	memcpy(&data, rx_buffer, 2);

// ready to read afterfore
#ifdef __WIRING_PI_H__
	digitalWrite(gpio_cs, HIGH);
#elif PIGPIO_H
	gpioWrite(gpio_cs, 1);
#endif
	
	return data;
}



int main(void)
{
	startSPI();
	readADC(0, 12);

	return 0;
}
