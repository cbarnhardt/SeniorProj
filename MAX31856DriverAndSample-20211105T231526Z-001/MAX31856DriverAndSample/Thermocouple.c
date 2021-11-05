/*
 * Thermocouple.c
 *
 *  Created on: Jul 27, 2021
 *      Author: socce
 */

#include "Thermocouple.h"

void initThermocouple(Thermocouple* thermo)
{
    //Initialize mux sel pins as ouputs
    (MUX_PORT->SEL0) &= ~MUX_PINMASK;
    (MUX_PORT->SEL1) &= ~MUX_PINMASK;
    (MUX_PORT->DIR)  |= MUX_PINMASK;

    //set channels
    (MUX_PORT->OUT)  |= (MUX1_0|MUX2_0);


    (thermo[0]).device.name = capOne;
    (thermo[1]).device.name = capTwo;
    (thermo[2]).device.name = capThree;
    (thermo[3]).device.name = capFour;
    (thermo[4]).device.name = waterIn;
    (thermo[5]).device.name = waterOut;
    (thermo[6]).device.name = invOne;
    (thermo[7]).device.name = invTwo;
    (thermo[8]).device.name = open0;
    (thermo[9]).device.name = open1;
    (thermo[10]).device.name = open2;
    (thermo[11]).device.name = open3;
    (thermo[12]).device.name = open4;
    (thermo[13]).device.name = open5;
    (thermo[14]).device.name = open6;
    (thermo[15]).device.name = open7;

    int i = 0;
    for(i = 0; i < 8; i++)
    {
    (thermo[i]).MAX.csPin = MAX_CS_PIN;
    }

    for(i = 8; i < 16; i++)
    {
    (thermo[i]).MAX.csPin = MAX_CS_PIN2;
    }

    //TODO: Get initial temp readings
    for(i = 0; i < 16; i++)
    {
     (thermo[i]).device.maxTemp = 30;
     (thermo[i]).device.currentTemp = 15;
    }

    triggerRead(0);

}

void triggerRead(uint8_t count)
{

    MAX temp;

    if(count < 8)
    {
    temp.csPin = MAX_CS_PIN;
    temp.txAddress = CR0_W;
    temp.tx[0] =  (CMODE_OFF|
                 ONESHOT_TRIG|
                OPENFAULT_OFF|
                COLDJNCTN_EN |
                FAULTINT_OFF |
                REJ60HZ);
    }
    else
    {
        temp.csPin = MAX_CS_PIN2;
        temp.txAddress = CR0_W;
        temp.tx[0] =  (CMODE_OFF|
                     ONESHOT_TRIG|
                    OPENFAULT_OFF|
                    COLDJNCTN_EN |
                    FAULTINT_OFF |
                    REJ60HZ);
    }

    writeToMAX(temp);

}
void readTemp(Thermocouple* thermo, uint8_t count)
{
    //get and convert temp
    //initMAX(&thermo[count].MAX);
    thermo[count].MAX.rxAddress = LTCBH_R;
//    thermo[count].MAX.rx[0] = 0;
//    thermo[count].MAX.rx[1] = 0;
//    thermo[count].MAX.rx[2] = 0;
    readMAX(&(thermo[count]).MAX);
    int temp = 0;
    int temptwo =0;
    double tempOne = 0.0;
    temp = (thermo[count].MAX.rx[0]<<16)| (thermo[count].MAX.rx[1]<<8) |(thermo[count].MAX.rx[2]);
    temptwo = temp >> 5;
    tempOne = temptwo/128.0;

    thermo[count].device.currentTemp = tempOne;

    if(count < 7)
    {
    (MUX_PORT->OUT) = thermo[count+1].device.name;
    triggerRead(count+1);
    //triggerRead(&thermo[0]);
    }
    else
    {
    (MUX_PORT->OUT) = thermo[0].device.name;
    triggerRead(0);
    }

}

bool checkFault(Thermocouple* thermo)
{
    if((*thermo).device.currentTemp >= (*thermo).device.maxTemp)
    {
       (*thermo).device.fault = true;
       return true;
    }
    else
    {
        return false;
    }

}
