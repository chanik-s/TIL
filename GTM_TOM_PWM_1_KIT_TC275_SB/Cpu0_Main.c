/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
 /*\title GTM TOM PWM generation
 * \abstract GTM TOM is used to generate a PWM signal, which is driving the intensity of an LED.
 * \description The LED is driven by pin 2 of the port 10. The state of the pin is controlled by the PWM signal
 *              generated by the TOM timer of GTM.
 *
 * \name GTM_TOM_PWM_1_KIT_TC275_SB
 * \version V1.0.1
 * \board hitex ShieldBuddy, KIT_AURIX_TC275_ARD_SB, TC27xTP_D-Step
 * \keywords AURIX, GTM, GTM_TOM_PWM_1, PWM, general timer, timer
 * \documents https://www.infineon.com/aurix-expert-training/Infineon-AURIX_GTM_TOM_PWM_1_KIT_TC275_SB-TR-v01_00_01-EN.pdf
 * \documents https://www.infineon.com/aurix-expert-training/TC27D_iLLD_UM_1_0_1_12_0.chm
 * \lastUpdated 2020-12-18
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "GTM_TOM_PWM.h"
#include "Bsp.h"

#include "Driver_Stm.h"
#include "Driver_Adc.h"

#include "IfxPort_PinMap.h"
#include "IfxPort.h"

//GPIO related
#define PCn_2_IDX 19
#define P2_IDX 2
#define PCn_1_IDX 11
#define P1_IDX 1

// ERU related
#define EXIS0_IDX 4
#define FEN0_IDX 8
#define EIEN0_IDX 11
#define INP0_IDX 12
#define IGP0_IDX 14

// SRC related
#define SRE_IDX 10
#define TOS_IDX 11



typedef struct
{
    uint32 u32nuCnt1ms;
    uint32 u32nuCnt10ms;
    uint32 u32nuCnt100ms;
    uint32 u32nuCnt1000ms;

}TestCnt;

// Task scheduling related
void AppScheduling(void);
void AppTask1ms(void);
void AppTask10ms(void);
void AppTask100ms(void);
void AppTask1000ms(void);
void initGPIO(void);
void makeSound(unsigned int soundIdx);
/***********************************************************************/
/*Variable*/
/***********************************************************************/
TestCnt stTestCnt;
uint32 adcResult=0;
uint32 adc46Result=0;
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define WAIT_TIME   2000/6              /* Number of milliseconds to wait between each duty cycle change  */
#define PWM_PERIOD          50000
#define FADE_STEPADC            PWM_PERIOD/4095                     /* PWM duty cycle for the TOM*/
IfxCpu_syncEvent g_cpuSyncEvent = 0;




int core0_main(void)
{
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    
    /* Initialize a time variable */
    Ifx_TickTime ticksFor1000ms = IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME);

    /* Initialize GTM TOM module */
    initGtmTomPwm();
    Driver_Stm_Init();
    Driver_Adc_Init();
    initGPIO();

    while(1)
    {
        //AppScheduling();
        //fadeLED();                  /* Change the intensity of the LED  */

        makeSound(6);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);     /* Delay of 10ms                    */
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(13);
        waitTime(ticksFor1000ms);
        makeSound(12);
        waitTime(ticksFor1000ms);

        makeSound(12);
        waitTime(ticksFor1000ms);
        waitTime(ticksFor1000ms);
        makeSound(12);
        waitTime(ticksFor1000ms);

        //
        makeSound(10);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(13);
        waitTime(ticksFor1000ms);
        makeSound(10);
        waitTime(ticksFor1000ms);
        makeSound(10);
        waitTime(ticksFor1000ms);

        //
        makeSound(8);//괜
        waitTime(ticksFor1000ms);
        makeSound(15);
        waitTime(ticksFor1000ms);
        makeSound(15);
        waitTime(ticksFor1000ms);

        makeSound(15);
        waitTime(ticksFor1000ms);
        makeSound(15);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        waitTime(ticksFor1000ms);
        makeSound(11);
        waitTime(ticksFor1000ms);
        makeSound(11);
        waitTime(ticksFor1000ms);
        makeSound(11);
        waitTime(ticksFor1000ms);

        //
        makeSound(8);//다
        waitTime(ticksFor1000ms);
        makeSound(9);//딜레이 길게
        waitTime(ticksFor1000ms);
        waitTime(ticksFor1000ms);
        makeSound(10);//파
        waitTime(ticksFor1000ms);


        makeSound(11);
        waitTime(ticksFor1000ms);
        makeSound(11);
        waitTime(ticksFor1000ms);
        makeSound(13);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);
        makeSound(14);
        waitTime(ticksFor1000ms);

    }
    return (1);
}
void initGPIO(void){
    P02_IOCR0.U &= ~(0x1F << PCn_1_IDX);
    P02_IOCR0.U |= 0x02 << PCn_1_IDX;

    P10_IOCR0.U &= ~(0x1F << PCn_2_IDX);
    P10_IOCR0.U |= 0x10 << PCn_2_IDX;

    P10_IOCR0.U &= ~(0x1F << PCn_1_IDX);
    P10_IOCR0.U |= 0x10 << PCn_1_IDX;
    //부저 p2.3
    P02_IOCR0.U &= ~(0x1F <<27 );
    P02_IOCR0.U |= 0x10 << 27;

    //spi
    IfxPort_setPinMode(IfxPort_P00_0.port, IfxPort_P00_0.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(IfxPort_P02_5.port, IfxPort_P02_5.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(IfxPort_P02_6.port, IfxPort_P02_6.pinIndex, IfxPort_Mode_outputPushPullGeneral);

    //adc
    IfxPort_setPinMode(IfxPort_P02_7.port, IfxPort_P02_7.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(IfxPort_P10_5.port, IfxPort_P10_5.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(IfxPort_P10_3.port, IfxPort_P10_3.pinIndex, IfxPort_Mode_outputPushPullGeneral);

    //조도센서 A1

}

void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;
}

void AppTask10ms(void)
{
    stTestCnt.u32nuCnt10ms++;
    //adc46Result=Driver_Adc4_DataObtain();
    //Driver_Adc4_ConvStart();

    adcResult=Driver_Adc0_DataObtain();
    Driver_Adc0_ConvStart();
}

void AppTask100ms(void)
{
    stTestCnt.u32nuCnt100ms++;
}

void AppTask1000ms(void)
{

    stTestCnt.u32nuCnt1000ms++;

    if(adcResult>=0&&adcResult<1300){

        fadeLEDbyADC(FADE_STEPADC*adcResult);
    }
    else if(adcResult>=1300&&adcResult<2600){

        fadeLEDbyADC(FADE_STEPADC*adcResult);
    }
    else if(adcResult>=2600&&adcResult<=4095){

        fadeLEDbyADC(FADE_STEPADC*adcResult);
    }
    else{
        ;
    }

}

void AppScheduling(void)
{
    if(stSchedulingInfo.u8nuScheduling1msFlag == 1u)
    {
        stSchedulingInfo.u8nuScheduling1msFlag = 0u;
        AppTask1ms();

        if(stSchedulingInfo.u8nuScheduling10msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling10msFlag = 0u;
            AppTask10ms();
        }

        if(stSchedulingInfo.u8nuScheduling100msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling100msFlag = 0u;
            AppTask100ms();
        }
        if(stSchedulingInfo.u8nuScheduling1000msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling1000msFlag = 0u;
            AppTask1000ms();
        }
    }
}

