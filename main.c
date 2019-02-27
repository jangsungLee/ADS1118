#include <stdio.h>
#include <unistd.h>
#include "ads1118.h"

#define ADC_GPIO_CS_1 25

int main(void)
{
	int i,k,j,d;
	unsigned short save;
	//gpioInitialise();
	startSPI();
	wiringPiSetup();
	pinMode(ADC_GPIO_CS_1,OUTPUT);

	for (k = 0; k < 99999999; k++)
	{
		save = readADC(4, ADC_GPIO_CS_1);
		printf("ADC value : %d, voltage: %f\n", save, CalVoltage(save));
	}
	//gpioTerminate();

	EndSPI();
	return 0;
}
