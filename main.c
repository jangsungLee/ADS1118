#include <stdio.h>
#include <unistd.h>
#include "ads1118.h"

int main(void)
{
	int i,k,j,d;
	unsigned short save;
	//gpioInitialise();
	startSPI();


	for (k = 0; k < 99999999; k++)
	{
		save = readADC(0, 22);
		printf("ADC value : %d, voltage: %f\n", save,save*0.00009375);
	}
	//gpioTerminate();

	EndSPI();
	return 0;
}
