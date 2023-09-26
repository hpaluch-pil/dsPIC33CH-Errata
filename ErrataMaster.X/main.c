/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
   ErrataMaster/main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33CH512MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#define FCY 90000000UL
#include <libpic30.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/slave1.h"

// NOTE:
// NOTE: Master Core should not be susceptible to REPEAT errata
// NOTE: The code below is just to verify this claim...
// NOTE:

volatile uint16_t gSpMax=0;
volatile uint16_t gInt1Sp=0;
volatile uint16_t gInt2Sp=0;
volatile uint16_t gMainCount=0;
volatile uint16_t gCcp1Counter = 0;
volatile uint16_t gCcp2Counter = 0;

int fatal_error_code = 0; // to ease Variable watch...

void my_fatal_error(int err)
{
    fatal_error_code = err;
    RE0_LED1_SetHigh();
    // and loop forever
    while(1){
        Nop(); // put BP here
    }
}

// at least 1 variable is volatile to not optimize out division operation
volatile uint32_t INT1_X = 300000991;
uint16_t INT1_Y = 30000;
uint16_t INT1_Z = 10000; // Z = X / Y, Y and Z must fit in 16-bit numbers
uint16_t INT1_M = 991;   // M = X % Y
uint16_t INT1_GOT = 0;
// variable below may not be volatile:
// main.c:124:13: error: passing argument 3 of '__builtin_divmodud' discards qualifiers from pointer target type
// c:\program files\microchip\xc16\v2.10\bin\bin\../..\support\generic\h/builtins.h:107:10: note: expected 'unsigned int *' but argument is of type 'volatile uint16_t *'
uint16_t INT1_M_GOT = 0;

// called at approx 10 kHz rate
void SCCP1_TMR_Timer32CallBack(void)
{
   // BSET & BCLR make this bug to appear sooner
   asm("BSET LATB,#0x2");
   // even such piece of code makes the AddressError trap to go away
#if 0
   // ensure that we don't test past good results
   INT1_GOT = 0xabc1; INT1_M_GOT=0xcde3;
#endif
   INT1_GOT = __builtin_divmodud(INT1_X,INT1_Y,&INT1_M_GOT);
   if (INT1_GOT != INT1_Z || INT1_M_GOT != INT1_M){
       __builtin_disi(0x3FFF); /* disable interrupts */
        my_fatal_error(2);
    }
    gCcp1Counter++;
    // WREG15 macro is deprecated so use assembler:
    asm("mov w15,%0" : "=g"(gInt1Sp));
    if (gSpMax < gInt1Sp)  gSpMax = gInt1Sp;
    asm("BCLR LATB,#0x2");
}

// at least 1 variable is volatile to not optimize out division operation
volatile uint32_t INT2_X = 310000981;
uint16_t INT2_Y = 31000;
uint16_t INT2_Z = 10000; // Z = X / Y, Y and Z must fit in 16-bit numbers
uint16_t INT2_M = 981;   // M = X % Y
uint16_t INT2_GOT = 0;
// variable below may not be volatile:
// main.c:124:13: error: passing argument 3 of '__builtin_divmodud' discards qualifiers from pointer target type
// c:\program files\microchip\xc16\v2.10\bin\bin\../..\support\generic\h/builtins.h:107:10: note: expected 'unsigned int *' but argument is of type 'volatile uint16_t *'
uint16_t INT2_M_GOT = 0;

// called at approx 100 kHz rate
void SCCP2_TMR_Timer32CallBack(void)
{
   asm("BSET LATC,#0x6");
   // enabling this will trigger trap much much later (around 101 seconds)
#if 0
   // ensure that we don't test past good results
   INT2_GOT = 0xabc1; INT2_M_GOT=0xcde3;
#endif
   INT2_GOT = __builtin_divmodud(INT2_X,INT2_Y,&INT2_M_GOT);
   if (INT2_GOT != INT2_Z || INT2_M_GOT != INT2_M){
       __builtin_disi(0x3FFF); /* disable interrupts */
        my_fatal_error(3);
    }
    gCcp2Counter++;
    // WREG15 macro is deprecated so use assembler:
    asm("mov w15,%0" : "=g"(gInt2Sp));
    if (gSpMax < gInt2Sp)  gSpMax = gInt2Sp;
    asm("NOP"); // this NOP is required to trigger Trap
    asm("BCLR LATC,#0x6");
}

// at least 1 variable is volatile to not optimize out division operation
volatile uint32_t MAIN_X = 200000999;
uint16_t MAIN_Y = 20000;
uint16_t MAIN_Z = 10000; // Z = X / Y, Y and Z must fit in 16-bit numbers
uint16_t MAIN_M = 999;   // M = X % Y
uint16_t MAIN_GOT = 0;
// variable below may not be volatile:
// main.c:124:13: error: passing argument 3 of '__builtin_divmodud' discards qualifiers from pointer target type
// c:\program files\microchip\xc16\v2.10\bin\bin\../..\support\generic\h/builtins.h:107:10: note: expected 'unsigned int *' but argument is of type 'volatile uint16_t *'
uint16_t MAIN_M_GOT = 0;

/*
                         Main application
 */
int main(void)
{
    SYSTEM_Initialize();
    //Program and enable slave
    SLAVE1_Program();
    SLAVE1_Start();
    while (1)
    {
        RE0_LED1_Toggle();
        
        // wait around 1s using CCP1 Timer (Timer uses 10 kHz we divide it by 8191 = 0x1fff)
        // we don't use %100 because it will be incorrect on wrapround...
        while( (gCcp1Counter & 0x1fff) == 0x1fff){
            // ensure that we don't test past good results, if DIV skipped for any reason
            MAIN_GOT = 0xabc5;MAIN_M_GOT = 0xbcd5;
            // test hardware division in loop
            MAIN_GOT = __builtin_divmodud(MAIN_X,MAIN_Y,&MAIN_M_GOT);
            if (MAIN_GOT != MAIN_Z || MAIN_M_GOT != MAIN_M){
                my_fatal_error(0);
            }
        }
        while( (gCcp1Counter & 0x1fff) != 0x1fff){
            // any of this below will make Trap go away...
            // Too slow: IO_RB10_SetHigh();
            // enabling this will make Trap go away...
#if 0            
            // ensure that we don't test past good results, if DIV skipped for any reason
            MAIN_M_GOT = 0xbcd7; MAIN_GOT = 0xabc7;
#endif            
            // test hardware division in loop
            MAIN_GOT = __builtin_divmodud(MAIN_X,MAIN_Y,&MAIN_M_GOT);
            asm("BTG LATC,#0x8"); // must be here, otherwise trap will go away
            if (MAIN_GOT != MAIN_Z || MAIN_M_GOT != MAIN_M){
                my_fatal_error(1);
            }
        }        
        gMainCount++;
    }
    return 1; 
}
/**
 End of File
*/

