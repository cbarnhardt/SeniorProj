
#ifndef SPILIB_H_
#define SPILIB_H_

#include "msp432.h"
#include "msp.h"

//Clock settings
    #define   CLKPHASEFIRST      0
    #define   CLKPHASESECOND     BITF
    #define   CLKPOLARITY_LOW    0
    #define   CLKPOLARITY_HIGH   BITE

//Bit Settings
    #define   LSBFIRST           0
    #define   MSBFIRST           BITD
    #define   EIGHTBITS          0
    #define   SEVENBITS          BITC

//Command
    #define   SLAVEMODE          0
    #define   MASTERMODE         BITB
 
 //SPI WIRE MODES //not wire
    #define   THREEWIREMODE          0
    #define   FOURWIREMODE_STEHIGH  BIT9
    #define   FOURWIREMODE_STELOW   BITA
    #define   I2C               (BIT9|BITA)
 
//Sync settings
    #define   ASYNC              0
    #define   SYNC               BIT8

 //Clock Select
    #define  ACLK                BIT6
  //#define  SMCLK               BIT7
    #define  SMCLK               BIT7|BIT8
 
 //STE Mode select 
    #define STEPREVCONF          0
    #define STEENSIG             BIT1
 
//RESET
    #define RST_DISABLE          0
    #define RST_ENABLE           BIT0



#endif /* SPILIB_H_ */
