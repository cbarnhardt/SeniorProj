#include "msp.h"
#include "MAX31856.h"
#include "spiLIB.h"
#include "Thermocouple.h"

/**
 * main.c
 */

MAX MAX1;
MAX MAX2;
Thermocouple thermo[16];


void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	int count = -1;

	MAX1.txAddress = 0x1;
    MAX1.csPin = MAX_CS_PIN;
    MAX1.tx[0] = 0xA;
    MAX1.tx[1] = 0x00;

	initSPI();
	initMAX(&MAX1);

	MAX1.rxAddress = CR0_R;
	readMAX(&MAX1);
	MAX1.rxAddress = CR1_R;
	readMAX(&MAX1);
	MAX1.rxAddress = MASK_R;
	readMAX(&MAX1);
	MAX1.rxAddress = LTCBH_R;
	readMAX(&MAX1);

	initThermocouple(thermo);




	while(1)
	{

	 if(checkDRDY())
	   {

	     if(count < 7)
	     {
//	     readTemp(thermo, count);
	      count++;
	     // count = 0;
	     }
	     else
	     {
	     count = 0;
	     }

         readTemp(thermo, count);

        // checkFault(&thermo[count]);

	   }

	}
}


