#ifndef DRIVER_ASC
#define DRIVER_ASC

//asynchronous 비동기 통신
/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxAsclin_Asc.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/
#define ASC_TX_BUFFER_SIZE 64
#define ASC_RX_BUFFER_SIZE 64


/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    uint8 tx[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
    uint8 rx[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
} AppAscBuffer;

typedef struct
{
    AppAscBuffer ascBuffer;                     /**< \brief ASC interface buffer */
    struct
    {
        IfxAsclin_Asc asc3;                     /**< \brief ASC interface */
    }         drivers;

    uint8     txData[20];
    uint8     rxData[20];
    Ifx_SizeT count;
} App_AsclinAsc;


/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
IFX_EXTERN App_AsclinAsc g_AsclinAsc;


/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
extern void Driver_Asc_Init(void);
extern void Driver_Asc3_TxTest(void);
extern void _out_uart3(const unsigned char chr);
extern unsigned char _in_uart3(void);
#endif /* DRIVER_STM */
