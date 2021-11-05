#ifndef THERMOCOUPLE_H_
#define THERMOCOUPLE_H_

#include "MAX31856.h"
#include <stdbool.h>



//MUX Macros
/*
 * A0TC_1 = P5.0
 * A1TC_1 = P5.1
 * A2TC_1 = P5.2
 *
 * A0TC_2 = P5.3
 * A0TC_2 = P5.4
 * A0TX_3 = P5.5
 */
#define MUX_PORT P5

#define MUX_PINMASK 0x3F

#define MUX1_0  0x00
#define MUX1_1  0x01
#define MUX1_2  0x02
#define MUX1_3  0x03
#define MUX1_4  0x04
#define MUX1_5  0x05
#define MUX1_6  0x06
#define MUX1_7  0x07

#define MUX2_0  0x00
#define MUX2_1  BIT3
#define MUX2_2  BIT4
#define MUX2_3  BIT4|BIT3
#define MUX2_4  BIT5
#define MUX2_5  BIT5|BIT3
#define MUX2_6  BIT5|BIT4
#define MUX2_7  BIT5|BIT4|BIT3


enum devName{capOne     = MUX1_0,
             capTwo     = MUX1_1,
             capThree   = MUX1_2,
             capFour    = MUX1_3,
             waterIn    = MUX1_4,
             waterOut   = MUX1_5,
             invOne     = MUX1_6,
             invTwo     = MUX1_7,
             open0      = MUX2_0,
             open1      = MUX2_1,
             open2      = MUX2_2,
             open3      = MUX2_3,
             open4      = MUX2_4,
             open5      = MUX2_5,
             open6      = MUX2_6,
             open7      = MUX2_7};

typedef struct device
{
    enum devName name;
    int maxTemp;
    float currentTemp;
    bool fault;
} device;

//TODO: Associate devName enum with case switch to set MUX signals
//TODO: Add generic names for extra thermocouples in devName
//TODO: Add more garlic

typedef struct Thermocouple
{
    MAX MAX;
    device device;

} Thermocouple;

void initThermocouple(Thermocouple*);
void readTemp(Thermocouple* thermo, uint8_t count);
void triggerRead(uint8_t count);
bool checkFault(Thermocouple* thermo);

#endif /* THERMOCOUPLE_H_ */
