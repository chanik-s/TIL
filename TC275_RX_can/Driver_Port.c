/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Port.h"
#include "Bsp.h"
/***********************************************************************/
/*Define*/ 
/***********************************************************************/

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
void Driver_Port_Init(void)
{
    /*P10_2    Digital Output led*/
    IfxPort_setPinModeOutput(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);

    /*P20_6    Digital Output*/
    IfxPort_setPinModeOutput(IfxPort_P20_6.port, IfxPort_P20_6.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinLow(IfxPort_P20_6.port, IfxPort_P20_6.pinIndex);

    /*LCD  output setting*/
        MODULE_P00.IOCR0.B.PC0=0x10;
        MODULE_P00.IOCR0.B.PC1=0x10;
        MODULE_P00.IOCR0.B.PC2=0x10;
        MODULE_P00.IOCR0.B.PC3=0x10;
        MODULE_P00.IOCR4.B.PC4=0x10;
        MODULE_P00.IOCR4.B.PC5=0x10;
        MODULE_P00.IOCR4.B.PC6=0x10;
        MODULE_P00.IOCR4.B.PC7=0x10;
        MODULE_P11.IOCR0.B.PC2=0x10;
        MODULE_P11.IOCR8.B.PC11=0x10;
        MODULE_P11.IOCR8.B.PC9=0x10;
}
void led_blink(){
    IfxPort_togglePin(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);

}
