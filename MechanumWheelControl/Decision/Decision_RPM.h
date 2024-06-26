/*
 * Decision_RPM.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: jaeeun
 *  필요 함수 및 기능
 *    float32 FUNC();
 *     - 가변저항의 값(0~1023)을 읽어 RPM 단위로 변환하여 return함
 *     - RPM 의 범위는 실험을 통해 설정하기
 *     -> Driver_Potentiometer.h 사용
 */
#ifndef DECISION_DECISION_RPM_H_
#define DECISION_DECISION_RPM_H_

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Driver_Potentiometer.h>
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
uint32 get_rpm_reference(void);



#endif /* DECISION_DECISION_RPM_H_ */
