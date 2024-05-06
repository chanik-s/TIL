/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Asc.h"
#include "Driver_Adc.h"
#include "Ifx_Console.h"
/***********************************************************************/
/*Define*/ 
/***********************************************************************/
//isr Tx,RX,EX 우선순위 보내는게 최고 우선 + ASC3 사용
#define ISR_PRIORITY_ASC_3_TX       53
#define ISR_PRIORITY_ASC_3_RX       52
#define ISR_PRIORITY_ASC_3_EX       51

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static IfxStdIf_DPipe g_ascStandardInterface; //standard interface object

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
App_AsclinAsc g_AsclinAsc; /**< \brief Demo information.. header에서 받아온 전역변수*/

//IfxAsclin_Asc g_ascHandler3; 이게 g_AsclinAsc.drivers.adc0과 같음
/***********************************************************************/
/*Function*/ 
/***********************************************************************/
IFX_INTERRUPT(ASC_Tx3IntHandler, 0, ISR_PRIORITY_ASC_3_TX);
IFX_INTERRUPT(ASC_Rx3IntHandler, 0, ISR_PRIORITY_ASC_3_RX);
IFX_INTERRUPT(ASC_Err3IntHandler, 0, ISR_PRIORITY_ASC_3_EX);

void ASC_Tx3IntHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_AsclinAsc.drivers.asc3);

}

void ASC_Rx3IntHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_AsclinAsc.drivers.asc3);
    //IfxAsclin_Asc_read(&g_AsclinAsc.drivers.asc0, g_AsclinAsc.rxData, &g_AsclinAsc.count, TIME_INFINITE);
    
    //g_AsclinAsc.txData[0] = g_AsclinAsc.rxData[0];
    //IfxAsclin_Asc_write(&g_AsclinAsc.drivers.asc0, g_AsclinAsc.txData, &g_AsclinAsc.count, TIME_INFINITE);
}

void ASC_Err3IntHandler(void)
{
    //IfxAsclin_Asc_isrError(&g_AsclinAsc.drivers.asc0);
    while(1);
}


static void Driver_Asc3_Init(void)
{
    /* create module config */
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN3); //asc3
    // usb 시리얼 위해서 asc3 사용 + uart 레지스터 값 config 변수에 입력

    /* set the desired baudrate */
    ascConfig.baudrate.prescaler    = 1;
    ascConfig.baudrate.baudrate     = 9600; /* FDR values will be calculated in initModule */
    ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_4; //9600bps를 위한 설정작업

    /* ISR priorities and interrupt target */
    ascConfig.interrupt.txPriority    = ISR_PRIORITY_ASC_3_TX;
    ascConfig.interrupt.rxPriority    = ISR_PRIORITY_ASC_3_RX;
    ascConfig.interrupt.erPriority    = ISR_PRIORITY_ASC_3_EX;
    ascConfig.interrupt.typeOfService = (IfxSrc_Tos)IfxCpu_getCoreIndex(); //cpu0 :0
                                        //명시적으로 =IfxSrc_Tos_cpu0;
    /* FIFO configuration */
    ascConfig.txBuffer     = g_AsclinAsc.ascBuffer.tx; //tx용 버퍼
    ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE; //64개까지 담을수 있음

    ascConfig.rxBuffer     = g_AsclinAsc.ascBuffer.rx; //rx용 버퍼
    ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

    /* pin configuration */    
    const IfxAsclin_Asc_Pins pins = {
        NULL_PTR,
        IfxPort_InputMode_pullUp,        // CTS pin not used
        &IfxAsclin3_RXD_P32_2_IN, // 데이터 시트 ASC3 RX용 핀
        IfxPort_InputMode_pullUp,        // Rx pin
        NULL_PTR,
        IfxPort_OutputMode_pushPull,     //RTS pin not used
        &IfxAsclin3_TX_P15_7_OUT,  // 데이터 시트 ASC3 TX용 핀
        IfxPort_OutputMode_pushPull,     // Tx pin
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };     

    ascConfig.pins = &pins;

    /* initialize module */
    IfxAsclin_Asc_initModule(&g_AsclinAsc.drivers.asc3, &ascConfig);
    IfxAsclin_Asc_stdIfDPipeInit(&g_ascStandardInterface,&g_AsclinAsc.drivers.asc3); //standard interface 초기화
    Ifx_Console_init(&g_ascStandardInterface); //console 초기화

    g_AsclinAsc.count = 2;    
}

void Driver_Asc_Init(void)
{
    Driver_Asc3_Init();
}

void Driver_Asc3_TxTest(void)
{   
    uint16_t u16nuTemp = 0u;
    
    g_AsclinAsc.count = 2; //data stream size로 2개씩 보낼수 있다 가정
    
    g_AsclinAsc.txData[0] = 'a';
    g_AsclinAsc.txData[1] = 'b';

    /* Transmit data */
    IfxAsclin_Asc_write(&g_AsclinAsc.drivers.asc3, g_AsclinAsc.txData, &g_AsclinAsc.count, TIME_INFINITE);
}

void _out_uart3(const unsigned char chr){
    IfxAsclin_Asc_blockingWrite(&g_AsclinAsc.drivers.asc3,chr); //문자 쓰기
}
unsigned char _in_uart3(void){
    return IfxAsclin_Asc_blockingRead(&g_AsclinAsc.drivers.asc3);
}
