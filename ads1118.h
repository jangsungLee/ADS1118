#ifndef _ADS1118_H_
#define _ADS1118_H_

#include <stdio.h>  
#include <stdlib.h>  
#include <stdint.h>  
#include <string.h>
#include <unistd.h>  
#include <fcntl.h>  
#include <linux/types.h>  
#include <sys/ioctl.h>  
#include <linux/spi/spidev.h>
#include <wiringPi.h>


// ==============================================SPI Fields=================================================================================================
static const char *device = "/dev/spidev0.0";
static uint8_t _spi_mode=1;// You need to set this correctly. if you not, your slave deivce do not response correct.
/* mode   CPOL(Clock Ploarity)    CPHA(Clock Phase)
   0 :           0                     0
   1 :           0                     1
   2 :           1                     0
   3 :           1                     1
*/
static uint8_t _spi_bits = 8;
static uint32_t _spi_speed = 1000000;// 1MHz(I designed for the sensor of 'ads1118'. - page.18)
static uint16_t _spi_delay = 0;
static int _spi_fd;


void startSPI(void)
{
	_spi_fd = open("/dev/spidev0.0", O_RDWR);
	if (_spi_fd < 0)
	{
		fputs("Can't open the SPI interface.\n", stderr);
		exit(1);
	}

	/*
	 * spi mode
	 */
	if ((ioctl(_spi_fd, SPI_IOC_WR_MODE, &_spi_mode)) == -1)
	{
		fputs("can't get spi mode on write.\n", stderr);
		exit(1);
	}

	if ((ioctl(_spi_fd, SPI_IOC_RD_MODE, &_spi_mode)) == -1)
	{
		fputs("can't get spi mode on read.\n", stderr);
		exit(1);
	}

	/*
	 * bits per word
	 */
	if ((ioctl(_spi_fd, SPI_IOC_WR_BITS_PER_WORD, &_spi_bits)) == -1)
	{
		fputs("can't set bits per word on write.\n", stderr);
		exit(1);
	}

	if ((ioctl(_spi_fd, SPI_IOC_RD_BITS_PER_WORD, &_spi_bits)) == -1)
	{
		fputs("can't set bits per word on read.\n", stderr);
		exit(1);
	}

	/*
	 * max speed hz
	 */
	if ((ioctl(_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_spi_speed)) == -1)
	{
		fputs("can't set max speed hz on write.\n", stderr);
		exit(1);
	}

	if ((ioctl(_spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &_spi_speed)) == -1)
	{
		fputs("can't set max speed hz on read.\n", stderr);
		exit(1);
	}

	// config the data when transfer to slave.
}

void transfer(char* tx, char *rx, int len)
{
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = _spi_delay,
		.speed_hz = _spi_speed,
		.bits_per_word = _spi_bits,
	};

	// When you see the documentation<spidev.h>, , the meaning of N of SPI_IOC_MESSAGE(N) is the number of array of type "struct spi_ioc_transfer"
	// if you cannot understand this, just refer line .80 .82 in spidev.h
	if ((ioctl(_spi_fd, SPI_IOC_MESSAGE(1), &tr)) < 1)
	{
		fputs("An error occured while transmitting.\n", stderr);
		return;
	}
}

void EndSPI(void)
{
	close(_spi_fd);
}
//====================================================================================================================================================================

//======================================================================ADS1118=======================================================================================
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

#define ADS1118_CONFIG_REGISTER_TS_MODE_ADC 0x0000
#define ADS1118_CONFIG_REGISTER_TS_MODE_TEMPERATURE 0x0100

#define ADS1118_CONFIG_REGISTER_PULL_UP_DISENABLE 0x0000
#define ADS1118_CONFIG_REGISTER_PULL_UP_ENABLE 0x0080// (default)

#define ADS1118_CONFIG_REGISTER_NOP_INVALID_DATA_DISUPDATE_CONTENT 0x0000
#define ADS1118_CONFIG_REGISTER_NOP_VALID_DATA_UPDATE_CONTENT 0x0002// (default)
#define ADS1118_CONFIG_REGISTER_NOP_INVALID_DATA_DISUPDATE_CONTENT_1 0x0004
#define ADS1118_CONFIG_REGISTER_NOP_INVALID_DATA_DISUPDATE_CONTENT_2 0x0006

#define ADS1118_CONFIG_REGISTER_RESERVED 0x0001 // Read only and this is olny read purpose register at config register.

int16_t readADC(int channel, int gpio_cs)
{
	static uint16_t data;
	static char tx_buffer[2], rx_buffer[2];

	// reset the spi interface of slave to ready.
#ifdef __WIRING_PI_H__
	digitalWrite(gpio_cs, HIGH);
#else
	gpioWrite(gpio_cs, 1);
#endif

	// set the config register.
	data = ADS1118_CONFIG_REGISTER_SS_SINGLE |
		ADS1118_CONFIG_REGISTER_PGA_6_144 |
		ADS1118_CONFIG_REGISTER_MODE_SINGLE |
		ADS1118_CONFIG_REGISTER_DR_128_SPS |
		ADS1118_CONFIG_REGISTER_TS_MODE_ADC |
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
	case 4:
		data |= ADS1118_CONFIG_REGISTER_MUX_DIFF_0_1;
		break;
	case 5:
		data |= ADS1118_CONFIG_REGISTER_MUX_DIFF_0_3;
		break;
	case 6:
		data |= ADS1118_CONFIG_REGISTER_MUX_DIFF_1_3;
		break;
	case 7:
		data |= ADS1118_CONFIG_REGISTER_MUX_DIFF_2_3;
		break;
	default:
		fprintf(stderr, "Invalid such a channel number.\n");
		return -1;
	}

	// ready to traffic the data
#ifdef __WIRING_PI_H__
	digitalWrite(gpio_cs, LOW);
#else
	gpioWrite(gpio_cs, 0);
#endif
	tx_buffer[0] = (data & 0xff00) >> 8, tx_buffer[1] = data & 0xff;
	transfer(tx_buffer, rx_buffer, 2);
	data=(rx_buffer[0] << 8) | rx_buffer[1];

	// set to ready for a later use.
#ifdef __WIRING_PI_H__
	digitalWrite(gpio_cs, HIGH);
#else
	gpioWrite(gpio_cs, 1);
#endif

	return data;
}

float CalVoltage(uint16_t adc_data)
{
	if (adc_data & 0x8000)
		return (adc_data & 0x7fff) * -0.0001875;
	else
		return adc_data * 0.0001875;
}
//==========================================================================================================================================================================
#endif /*_ADS1118_H_*/

