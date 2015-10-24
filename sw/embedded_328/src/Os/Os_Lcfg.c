/**
* COPYRIGHT : GNU GPL V2 -
*
* This file contains the task configuration
*
* Author: Armin Schlegel
* based on project eRTK (https://github.com/eRTK/eRTK)
* Creation date: 2015-09-21
* Last change: 2015-09-21
*
* Moduleversion: 0.0.1
****************************************************************************** */
#include <Os.h>
#include <Os_Cfg.h>
#include <Os_Lcfg.h>

void UartHelloTask(void);
void UartWorldTask(void);

const Os_TaskConfigType Os_LcfgTaskConfig[OS_MAX_NUMBER_OF_TASKS] =
{
        /* Id, Handler, prio, parameter0, parameter1 */
        /* Task 1 */
        {
                UartHelloTask,
                10,
                0,
                0
        },
        /* Task 2 */
        {
                UartWorldTask,
                10,
                0,
                0
        },
//        /* Task 3 */
//        {
//                tskUART,
//                10,
//                2,
//                "UART3"
//        },
//        /* Task 4 */
//        {
//                tskUART,
//                10,
//                3,
//                "UART4"
//        },
//        /* Task 5 */
//        {
//                tskHighPrio,
//                20,
//                1,
//                "highp"
//        },
//        /* Task 4 */
//        {
//                tskUART,
//                10,
//                3,
//                "UART4"
//        }
};


const void *Os_GetLcfgData(void)
{
   return ((void*) &Os_LcfgTaskConfig);
}
