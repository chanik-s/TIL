/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "AppScheduling.h"
#include "Driver_Stm.h"
#include "Driver_Adc.h"
#include "Driver_Asc.h"
#include "Driver_Gtm.h"
#include "AppMode.h"
#include "Driver_Port.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    uint32_t u32nuCnt1ms;
    uint32_t u32nuCnt10ms;
    uint32_t u32nuCnt100ms;
}TestCnt;


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void AppTask1ms(void);
static void AppTask10ms(void);
static void AppTask100ms(void);

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
TestCnt stTestCnt;
uint32 mypwmcnt1ms;
uint32 mypwmcnt1s;
//App_GtmTomTimer g_GtmTomTimer;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/
static void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;
//    if(g_GtmTomTimer.isrCounter.slotOneMs%2==0) //0.5m 주기
//    {
//        mypwmcnt1ms++;
//    }
    //mypwmcnt1s=mypwmcnt1ms*0.001;

    if(stTestCnt.u32nuCnt1ms<5000){
        //pwm duty 0
        Driver_Gtm_Tom0Test(0);
        GtmTomPwmHl_run();
    }
    else if(stTestCnt.u32nuCnt1ms>=5000&&stTestCnt.u32nuCnt1ms<=10000){
        //pwm duty 30%
        Driver_Gtm_Tom0Test(0.5);
        GtmTomPwmHl_run();
    }
    else{
        //pwm duty 0
        Driver_Gtm_Tom0Test(0);
        GtmTomPwmHl_run();
    }
}

static void AppTask10ms(void)
{
    
    stTestCnt.u32nuCnt10ms++;
    //Unit_Mode();

    /*ADC Test*/
    //Driver_Adc0_ConvStart();

    /*ASC Test*/
    //Driver_Asc0_TxTest();

    /*PWM Test*/
    //Driver_Gtm_Tom0Test();
}

static void AppTask100ms(void)
{
    stTestCnt.u32nuCnt100ms++;
    IfxPort_togglePin(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
   // if(stTestCnt.u32nuCnt100ms%10==0){
   //     mypwmcnt1s=mypwmcnt1ms;
   // }
}
static void AppNoTask(){
    //엔코더 설정

}
void AppScheduling(void)
{
    AppNoTask();
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
    }
}
