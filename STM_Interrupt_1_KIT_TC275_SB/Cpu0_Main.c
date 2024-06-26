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
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "IfxPort.h"
#include "IfxPort_PinMap.h"

#include "Driver_Stm.h"

#define DIO IfxPort_P00_0
#define SCLK
#define RLCK

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


/***********************************************************************/
/*Variable*/
unsigned char _LED_0F[10]={0xC0,0xF9,0xA4,0xB0,0x99, 0x92,0x82,0xF8,0x80,0x90};
/***********************************************************************/
TestCnt stTestCnt;

IfxCpu_syncEvent g_cpuSyncEvent = 0;


void initGPIO(void);
void initERU(void);
void send(unsigned char X);
void send_port(unsigned char X, unsigned char port);



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

    initGPIO();
    Driver_Stm_Init();
    int n4;
    while(1)
    {

       AppScheduling();
       n4= ((int)stTestCnt.u32nuCnt1000ms/10)%10;
       send_port(_LED_0F[stTestCnt.u32nuCnt10ms%10],0x1);
       send_port(_LED_0F[stTestCnt.u32nuCnt100ms%10],0x2);
      send_port(_LED_0F[stTestCnt.u32nuCnt1000ms%10]&0x7f,0x4);
        send_port(_LED_0F[n4],0x8);
        //send_port(0xC0,0x8);
    }
    return (1);
}

void initGPIO(void){
    P02_IOCR0.U &= ~(0x1F << PCn_1_IDX);
    P02_IOCR0.U |= 0x02 << PCn_1_IDX;

    P10_IOCR0.U &= ~(0x1F << PCn_2_IDX);
    P10_IOCR0.U |= 0x10 << PCn_2_IDX; //blue

    P10_IOCR0.U &= ~(0x1F << PCn_1_IDX);
    P10_IOCR0.U |= 0x10 << PCn_1_IDX; //red

    //SCLK RLCK DIO output 설정
    //DIO
    //P23_IOCR0.U&=~(0x1F<<19); //23.2
    //P23_IOCR0.U|=(0x10<<19); //23.2
//    P00_IOCR0.U&=~(0x1F<<3); //0.0
//    P00_IOCR0.U|=(0x10<<3); //0.0
//    //d7 RLCK p2.5
//    P02_IOCR4.U&=~(0x1F<<11);
//    P02_IOCR4.U|=0x10<<11;
//    //d8 SCLK p2.6
//    P02_IOCR4.U&=~(0x1F<<19);
//    P02_IOCR4.U|=0x10<<19;
    IfxPort_setPinMode(IfxPort_P00_0.port, IfxPort_P00_0.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(IfxPort_P02_5.port, IfxPort_P02_5.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(IfxPort_P02_6.port, IfxPort_P02_6.pinIndex, IfxPort_Mode_outputPushPullGeneral);
}
void send(unsigned char X)
{

  for (int i = 8; i >= 1; i--)
  {
    if (X & 0x80)
    {
        //P23_OUT.U=0x1<<2; //dio
        ///P00_OUT.U=0x1<<0; //dio
        IfxPort_setPinHigh(IfxPort_P00_0.port, IfxPort_P00_0.pinIndex);
    }
    else
    {
        //P23_OUT.U=0x0<<2; //dio
        //P00_OUT.U=0x0<<0; //dio
        IfxPort_setPinLow(IfxPort_P00_0.port, IfxPort_P00_0.pinIndex);
        //IfxPort_setPinHigh(port, pinIndex);
    }
    X <<= 1;
    IfxPort_setPinLow(IfxPort_P02_6.port, IfxPort_P02_6.pinIndex);
    IfxPort_setPinHigh(IfxPort_P02_6.port, IfxPort_P02_6.pinIndex);
    //P02_OUT.U=0x0<<6;
    //P02_OUT.U=0x1<<6;
  }
}
void send_port(unsigned char X, unsigned char port)
{
  send(X);
  send(port);
  IfxPort_setPinLow(IfxPort_P02_5.port, IfxPort_P02_5.pinIndex);
     IfxPort_setPinHigh(IfxPort_P02_5.port, IfxPort_P02_5.pinIndex);
  //P02_OUT.U=0x0<<5;
 // P02_OUT.U=0x1<<5;

}

void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;
}

void AppTask10ms(void)
{
    stTestCnt.u32nuCnt10ms++;
}

void AppTask100ms(void) //0.1초
{
    static int flag100ms=0;
    if(flag100ms == 0)
       {
           IfxPort_setPinLow(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //red
           flag100ms = 1;
       }
       else
       {
           IfxPort_setPinHigh(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex);
           flag100ms = 0;
       }
    stTestCnt.u32nuCnt100ms++;
}

void AppTask1000ms(void) //1초때 flag바꾸면서 LED 상태 바꾸는 TASK
{
    static int flag = 0;
    if(flag == 0)
    {
        IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
        flag = 1;
    }
    else
    {
        IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
        flag = 0;
    }
    stTestCnt.u32nuCnt1000ms++;

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
