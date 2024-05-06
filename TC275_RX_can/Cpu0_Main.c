/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "Driver_Watchdog.h"
#include "Driver_Stm.h"
#include "Driver_Port.h"
#include "Driver_Adc.h"
#include "Driver_Asc.h"
#include "Driver_Gtm.h"
#include "Driver_Can.h"
#include "AppScheduling.h"
#include "Bsp.h"

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/

/***********************************************************************/
/*Function*/ 
/***********************************************************************/
//lcd 관련 설정 코드
void delay_us(unsigned int m){
    unsigned int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<2;j++){
            ;
        }
    }
}
void delay_ms1(unsigned int m){
    unsigned int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<18200;j++){
            ;
        }
    }
}
//4 portg 0  p11_9
//5       1  p11_11
//6       2  p11_2
void write_instruction(unsigned char i){

    //PORTG=0x04; --> 100  관련 초기화 init gpio에서

    MODULE_P11.OUT.U=0x00000004;
    //MODULE_P11.OUT.B.P2=1;
    //MODULE_P11.OUT.B.P11=0;
    //MODULE_P11.OUT.B.P9=0;


    delay_us(10);

    MODULE_P00.OUT.U=i;
    delay_us(10);

    MODULE_P11.OUT.U=0x0;
    //MODULE_P11.OUT.B.P2=0;
    //MODULE_P11.OUT.B.P11=0;
    //MODULE_P11.OUT.B.P9=0;
    delay_us(100);
}
void write_data(char d){

   MODULE_P11.OUT.U=0x00000204;
   //MODULE_P11.OUT.B.P2 = 1;
   //MODULE_P11.OUT.B.P11 = 0;
   //MODULE_P11.OUT.B.P9 = 1;

   delay_us(10);
   MODULE_P00.OUT.U =d;//d
   delay_us(10);

   MODULE_P11.OUT.U=0x00000200;
   //MODULE_P11.OUT.B.P2 = 0;
   //MODULE_P11.OUT.B.P11 = 0;
   //MODULE_P11.OUT.B.P9 = 1;

   delay_us(100);
}


void init_lcd(void){

    delay_ms1(400);
    //write_instruction(0x30);
    write_instruction(0x01); //display clear
    delay_ms1(100);
    write_instruction(0x38);
    delay_ms1(100);
    //write_instruction(0x38);
    //delay_ms1(1000);
    //write_instruction(0x38);
    delay_ms1(100);
    //write_instruction(0x3c);
    //delay_ms1(1000);
   // write_instruction(0x08);
    write_instruction(0x0e);
    delay_ms1(100);


    write_instruction(0x06);
    delay_ms1(100);
    //write_instruction(0x0c);
    //delay_ms1(10);
}
int core0_main(void)
{   
    /*Driver Init*/
    Driver_Watchdog_Init();
    Driver_Port_Init();
    Driver_Stm_Init();
    Driver_Adc_Init();
    Driver_Asc_Init(); //uart 통신 위한
    Driver_Gtm_Init();
    Driver_Can_Init();
    init_lcd();
    /*Global Interrupt Enable*/
    IfxCpu_enableInterrupts();

    //unsigned char c;

    while(1)
    {

        //AppScheduling();
        DrvAdc_GetAdcRawGroup0(); //adc 받기
        SensorAdcRaw* pstSensorAdcRaw =MidAdc_GetAdcGroup0SenseRaw();
        uint32 ULSraw=pstSensorAdcRaw->UlSSense1_Raw;
        led_blink();
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, ULSraw));
    }
//        c=_in_uart3(); //입력
//        _out_uart3(c); //전송
//        if(c=='\r') {
//            _out_uart3('\n');
//        }
//        if(c=='q'){
//            IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
//        }
//        if(c=='w'){
//            IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
//        }

//       delay_ms1(1000);
//       write_instruction(0x80);
//        delay_ms1(1000);
//       write_data('a'); //0x61
//       delay_ms1(1000);
//       write_data('b');//0x62
//       delay_ms1(1000);
//       write_instruction(0xc0);
//       delay_ms1(1000);
//        write_data('c');//0x63
//        delay_ms1(1000);
//        write_data('d');//0x64
        //AppScheduling();
       // Driver_Can_TxTest();


    return (0);
}
