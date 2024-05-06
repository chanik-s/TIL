#ifndef DRIVER_ADC
#define DRIVER_ADC

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxVadc.h"
#include "IfxVadc_Adc.h"


#define MidAdc_GetAdcGroup0SenseRaw() (&stSensorAdcRaw)
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    IfxVadc_Adc vadc; /* VADC handle */
    IfxVadc_Adc_Group adcGroup;
} App_VadcAutoScan;


typedef enum{
    ADC_GROUP0_CH0=0u,
    ADC_GROUP0_CH1,
    ADC_GROUP0_CH2,
    ADC_GROUP0_CH3,
    ADC_GROUP0_CH4,
    ADC_GROUP0_CH5,
    ADC_GROUP0_CH6,
    ADC_GROUP0_CH7,
    ADC_GROUP0_MAX
}ADC_GROUP0;

typedef struct{ //센싱 변수
        uint32 UlSSense1_Raw;
        uint32 UlSSense2_Raw;
        uint32 UlSSense3_Raw;
}SensorAdcRaw;

extern SensorAdcRaw stSensorAdcRaw; //전력 센싱 변수


/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
IFX_EXTERN App_VadcAutoScan g_VadcAutoScan;
extern volatile uint32_t adcDataResult[5];

/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
extern void Driver_Adc_Init(void);
extern void Driver_Adc0_ConvStart(void);
extern void Driver_Adc0_DataObtain(void);

extern void DrvAdc_GetAdcRawGroup0(void);


#endif /* DRIVER_STM */
