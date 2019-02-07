#ifndef _SPIDEV_H__
#define _SPIDEV_H__

#include <stdio.h>  
#include <stdlib.h>  
#include <stdint.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <linux/types.h>  
#include <sys/ioctl.h>  
#include <linux/spi/spidev.h>  

static const char *device = "/dev/spidev0.0";
static uint8_t _spi_mode;//=0;
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

#endif /*_SPIDEV_H__*/