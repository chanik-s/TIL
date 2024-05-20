#ifndef DRIVER_ADC
#define DRIVER_ADC

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxVadc.h"
#include "IfxVadc_Adc.h"

/***********************************************************************/
/*Typedef*/
/***********************************************************************/
typedef struct
{
    IfxVadc_Adc vadc; /* VADC handle */
    IfxVadc_Adc_Group adcGroup;
} App_VadcAutoScan;


/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*External Variable*/
/***********************************************************************/
IFX_EXTERN App_VadcAutoScan g_VadcAutoScan;
IFX_EXTERN App_VadcAutoScan g_VadcAutoScan6;

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/
extern void Driver_Adc_Init(void);
extern void Driver_Adc0_ConvStart(void);
extern uint32 Driver_Adc0_DataObtain(void);

extern void Driver_Adc4_ConvStart(void);
extern uint32 Driver_Adc4_DataObtain(void);


#endif /* DRIVER_STM */
