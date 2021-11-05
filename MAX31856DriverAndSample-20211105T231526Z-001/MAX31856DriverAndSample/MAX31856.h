#ifndef MAX31856_H_
#define MAX31856_H_

#include "spiLIB.h"
#include <stdbool.h>
#include "msp432.h"

//Defines SPI Hardware and IRQ Handler 
#define EUSCI_X            EUSCI_A3
#define MAX_IRQn           EUSCIA3_IRQn
#define MAX_IRQHandler     EUSCIA3_IRQHandler

//Defines MSP Registers as Macros
#define MAX_CTL           (EUSCI_X->CTLW0)
#define MAX_STAT          (EUSCI_X->STATW)
#define MAX_RXBUF         (EUSCI_X->RXBUF)
#define MAX_TXBUF         (EUSCI_X->TXBUF)
#define MAX_CLKDIVIDE     (EUSCI_X->BRW)
#define MAX_IE            (EUSCI_X->IE)
#define MAX_IFG           (EUSCI_X->IFG)

//Gives Port for MSP432
#define MAX_PORT           P9
#define MAX_MOSI_PIN       BIT7
#define MAX_MISO_PIN       BIT6
#define MAX_CLK_PIN        BIT5
#define MAX_CS_PIN         BIT4 //TODO: Determine if needed

//Second select pin is on PORT 8
#define CS_PORT2           P8
#define MAX_CS_PIN2        BIT0

//0b1110 0000
#define SEL0_MAX_MASK      0xE0 //TODO: switch to port used on control board
#define SEL1_MAX_MASK      0

//Read Addresses for MAX31856 Macros + Descriptions

                //READ ADDR         DESC                    TYPE
#define CR0_R       0x00        //Config 0                  R/W
#define CR1_R       0x01        //Config 1                  R/W
#define MASK_R      0x02        //Fault Mask                R/W
#define CJHF_R      0x03        //Cold-Jnctn High Fault     R/W
#define CJLF_R      0x04        //Cold-Jnctn Low Fault      R/W
#define LTHFTH_R    0x05        //Lin Tmp High Fault MSB    R/W
#define LTHFTL_R    0x06        //Lin Tmp High Fault LSB    R/W
#define LTLFTH_R    0x07        //Lin Tmp Low Fault MSB     R/W
#define LTLFTL_R    0x08        //Lin Tmp Low Fault LSB     R/W
#define CJTO_R      0x09        //Cold-Jnctn Offset         R/W
#define CJTH_R      0x0A        //Cold-Jnctn Temp MSB       R/W
#define CJTL_R      0x0B        //Cold-Jnctn Temp LSB       R/W
#define LTCBH_R     0x0C        //Lin TC Temp BYTE 2        R
#define LTCBM_R     0x0D        //Lin TC Temp BYTE 1        R
#define LTCBL_R     0x0E        //Lin TC Temp BYTE 0        R
#define SR_R        0x0F        //Faul Status               R

//Write Addresses for MAX31856
#define CR0_W       0x80        //Config 0                  R/W
#define CR1_W       0x81        //Config 1                  R/W
#define MASK_W      0x82        //Fault Mask                R/W
#define CJHF_W      0x83        //Cold-Jnctn High Fault     R/W
#define CJLF_W      0x84        //Cold-Jnctn Low Fault      R/W
#define LTHFTH_W    0x85        //Lin Tmp High Fault MSB    R/W
#define LTHFTL_W    0x86        //Lin Tmp High Fault LSB    R/W
#define LTLFTH_W    0x87        //Lin Tmp Low Fault MSB     R/W
#define LTLFTL_W    0x88        //Lin Tmp Low Fault LSB     R/W
#define CJTO_W      0x89        //Cold-Jnctn Offset         R/W
#define CJTH_W      0x8A        //Cold-Jnctn Temp MSB       R/W
#define CJTL_W      0x8B        //Cold-Jnctn Temp LSB       R/W

//CR0 Config Masks
#define CMODE_AUTO      BIT7
#define CMODE_OFF       0x00
#define ONESHOT_TRIG    BIT6
#define OPENFAULT_OFF   0x00  //NOTE: Open fault detection time depends on lead resistance
#define OPENFAULT_10MS  BIT4  //values given are nominal... refer to datasheet
#define OPENFAULT_32MS  BIT5
#define OPENFAULT_100MS (BIT5|BIT4)
#define COLDJNCTN_EN    0x00
#define COLDJNCTN_OFF   BIT3
#define FAULTINT_EN     BIT2
#define FAULTINT_OFF    0x00
#define FAULTCLR        BIT1
#define REJ60HZ         0x00
#define REJ50HZ         BIT0

//CR1 Config Masks
#define AVGSAMPLE_1     0x00
#define AVGSAMPLE_2     BIT4
#define AVGSAMPLE_4     BIT5
#define AVGSAMPLE_8    (BIT4|BIT5)
#define AVGSAMPLE_16    BIT6
#define TYPE_B          0x00
#define TYPE_E          0x01
#define TYPE_J          0x02
#define TYPE_K          0x03
#define TYPE_N          0x04
#define TYPE_R          0x05
#define TYPE_S          0x06
#define TYPE_T          0x07

//MASK Register Configs
#define COLDJNCTNHIGH_OFF   BIT5
#define COLDJNCTNLOW_OFF    BIT4
#define TCHIGH_OFF          BIT3
#define TCLOW_OFF           BIT2
#define OVUV_OFF            BIT1
#define OPEN_OFF            BIT0

//Define buffer size and address size for part
#define BUFF_SIZE 5
#define ADDR_SIZE 2

//DRDY PINS and port
#define DRDY_PORT P7
#define DRDY_MAX1 BIT4
#define DRDY_MAX2 BIT5

//FAULT PINS
#define FAULT_PORT P7
#define FAULT_MAX1 BIT1
#define FAULT_MAX2 BIT2

//END MACROS

typedef struct MAX
{
    uint8_t tx[BUFF_SIZE];
    uint8_t rx[BUFF_SIZE];
    uint8_t txAddress;
    uint8_t rxAddress;
    uint8_t csPin;
} MAX;

void initSPI(void);
void initMAX(MAX*);
void writeToMAX(MAX);
void readMAX(MAX*);
bool checkDRDY(void);
uint8_t getTail(MAX);
bool purgeTxBuff(MAX*);
bool purgeRxBuff(MAX*);
void encode_16(uint16_t source, uint8_t*);
uint16_t decode_16(uint8_t* source);

#endif
