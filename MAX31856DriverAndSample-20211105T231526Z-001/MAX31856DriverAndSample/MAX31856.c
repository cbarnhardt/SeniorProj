/*
 * MAX31856.c
 *
 *  Created on: Jul 25, 2021
 *      Author: socce
 */
#include "MAX31856.h"
#include <stdbool.h>
#include "msp432.h"

void initMAX(MAX* max)
{

    //CONFIG DRDY as input
    (DRDY_PORT->SEL0) &= ~(DRDY_MAX1|DRDY_MAX2);
    (DRDY_PORT->SEL1) &= ~(DRDY_MAX1|DRDY_MAX2);
    (DRDY_PORT->DIR)  &= ~(DRDY_MAX1|DRDY_MAX2);
    (DRDY_PORT->OUT)  |= (DRDY_MAX1|DRDY_MAX2);
    (DRDY_PORT->REN)  |= (DRDY_MAX1|DRDY_MAX2);

    //CONFIG FAILT as input
    (FAULT_PORT->SEL0) &= ~(FAULT_MAX1|FAULT_MAX2);
    (FAULT_PORT->SEL1) &= ~(FAULT_MAX1|FAULT_MAX2);
    (FAULT_PORT->DIR)  &= ~(FAULT_MAX1|FAULT_MAX2);
    (FAULT_PORT->OUT)  |= (FAULT_MAX1|FAULT_MAX2);
    (FAULT_PORT->REN)  |= (FAULT_MAX1|FAULT_MAX2);

    //TODO: change to local MAX struct here
    //Build register masks
    uint8_t CR0Mask =  (CMODE_OFF    |
                        OPENFAULT_OFF|
                        COLDJNCTN_EN |
                        FAULTINT_OFF |
                        REJ60HZ);

    uint8_t CR1Mask = (AVGSAMPLE_1|TYPE_K);

    uint8_t MASKMask = (COLDJNCTNHIGH_OFF|
                        COLDJNCTNLOW_OFF |
                        TCHIGH_OFF       |
                        TCLOW_OFF        |
                        OVUV_OFF         |
                        OPEN_OFF);

    //Write same masks to both chips together

    (*max).tx[0] = CR0Mask;
    (*max).txAddress = CR0_W;

    (CS_PORT2->OUT) &= ~MAX_CS_PIN2;
     writeToMAX(*max);
    (CS_PORT2->OUT) |= MAX_CS_PIN2;

    (*max).tx[0] = CR1Mask;
    (*max).txAddress = CR1_W;

    (CS_PORT2->OUT) &= ~MAX_CS_PIN2;
     writeToMAX(*max);
    (CS_PORT2->OUT) |= MAX_CS_PIN2;

    (*max).tx[0] = MASKMask;
    (*max).txAddress = MASK_W;

    (CS_PORT2->OUT) &= ~MAX_CS_PIN2;
     writeToMAX(*max);
    (CS_PORT2->OUT) |= MAX_CS_PIN2;




}

void initSPI(void)
{
    //Config SPI
        MAX_CTL = RST_ENABLE;
        MAX_CTL =  (CLKPHASEFIRST   |
                    CLKPOLARITY_LOW | //NOTE: Part can manage either polarity.
                    MSBFIRST        |
                    EIGHTBITS       |
                    MASTERMODE      |
                    THREEWIREMODE   |
                    SYNC            |
                    ACLK            | //TODO: Change clk speed to something reasonable
                    STEPREVCONF     |
                    RST_DISABLE
                    );

        //Set clkdivider to 1
        MAX_CLKDIVIDE = 1;

        //config CS pin as GPIO

        //TODO: ADD Other CS pin here
        (MAX_PORT->SEL0) &= ~MAX_CS_PIN;
        (MAX_PORT->SEL1) &= ~MAX_CS_PIN;
        (MAX_PORT->DIR)  |= MAX_CS_PIN;

        (CS_PORT2->SEL0) &= ~MAX_CS_PIN2;
        (CS_PORT2->SEL1) &= ~MAX_CS_PIN2;
        (CS_PORT2->DIR)  |= MAX_CS_PIN2;

        //Init EUSCI pins
        (MAX_PORT -> SEL0)  |= SEL0_MAX_MASK;
        (MAX_PORT ->  SEL1) |= 0;

        MAX_IE |= BIT0; //enable recieve interrupt
        MAX_PORT->OUT |= MAX_CS_PIN;
        CS_PORT2->OUT |= MAX_CS_PIN2;
        // NVIC_EnableIRQ(MAX_IRQn);

}

uint8_t getTail(MAX max)
{
    switch(max.rxAddress)
          {
          case LTHFTH_R :
              return 1;
          case LTLFTH_R :
              return 1;
          case CJTH_R :
              return 1;
          case LTCBH_R :
              return 2;
          default :
              return 0;
          }
}
void writeToMAX(MAX max)
{

    uint8_t tail = getTail(max);

    //Assert Bus
    if(max.csPin == MAX_CS_PIN)
    {
    (MAX_PORT->OUT) &= ~max.csPin;
    }
    else
    {
       (CS_PORT2 -> OUT) &= ~max.csPin;
    }

    while(!(EUSCI_X->IFG & 2));

    //Send write address
      while(!(EUSCI_X->IFG & 2));
      EUSCI_X->TXBUF = max.txAddress;

    //Write from start to tail

    int i = 0;
    for(i = 0; i <= tail; i++)
    {
       while(!(EUSCI_X->IFG & 2));
       EUSCI_X->TXBUF = max.tx[i];
    }

    //Finish Op
    while(!(EUSCI_X->IFG & 2));
    while(EUSCI_X->STATW & 1);


    if(max.csPin == MAX_CS_PIN)
    {

    (MAX_PORT->OUT) |= max.csPin;

    }
    else
    {
        (CS_PORT2 -> OUT) |= max.csPin;
    }


}

void readMAX(MAX* max)
{
    uint8_t tail = getTail((*max));

    //Assert Bus
    if((*max).csPin == MAX_CS_PIN)
    {
    (MAX_PORT->OUT) &= ~(*max).csPin;
    }
    else
    {
      (CS_PORT2 -> OUT) &= ~(*max).csPin;
    }

     while(!(EUSCI_X->IFG & 2));

    //Write Read ADDY
    while(!(EUSCI_X->IFG & 2));
    EUSCI_X->TXBUF = (*max).rxAddress;

    while(EUSCI_X->STATW & 1);
    while(!(EUSCI_X->IFG) & 2);

    int i = 0;
    for(i = 0; i <= tail; i++)
    {
        EUSCI_X->TXBUF = 0x00;
        while(EUSCI_X->STATW & 1);
        (*max).rx[i] = (EUSCI_X->RXBUF);
    }

    while(EUSCI_X->STATW & 1);

    if((*max).csPin == MAX_CS_PIN)
    {

    (MAX_PORT->OUT) |= (*max).csPin;

    }
    else
    {
        (CS_PORT2 -> OUT) |= (*max).csPin;
    }
}


bool purgeTxBuff(MAX* max)
{

    return true;
}


bool checkDRDY(void)
{
    if(!((DRDY_PORT->IN) & (DRDY_MAX1)) ||!((DRDY_PORT->IN) & (DRDY_MAX2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool purgeRxBuff(MAX* max)
{

    return true;
}

void encode_16(uint16_t source, uint8_t* max)
{
    max[0] = (source & 0xFF00) >> 8;
    max[1] = source & 0x00FF;
}

uint16_t decode_16(uint8_t* source)
{

    uint16_t dest = 0;

    dest = source[0];
    dest = (dest << 8);
    dest = (dest | source[1]);

    return dest;
}

