/* *************************************************************************************************
 * file:        Adc_lcfg.c
 *
 *          The adc module linktime configuration.
 *
 * author:      Armin Schlegel, Mr. L.
 * date:        09.10.2014
 * version:     0.3   worky, testing
 *
 * file history:
 *          09.10.2014  A. Schlegel file created, basic version
 *          14.10.2014  Mr. L.      update linktimeconfig, nicify layout, add comments
 *
 * notes:
 *          - none -
 *
 * copyright:   http://creativecommons.org/licenses/by-nc-sa/3.0/
 **************************************************************************************************/

/* ------------------------------------ INCLUDES ------------------------------------------------ */

#include "Adc_Lcfg.h"

#include "Adc.h"


/* ------------------------------------ DEFINES ------------------------------------------------- */

/* ------------------------------------ TYPE DEFINITIONS ---------------------------------------- */

/* ------------------------------------ PROTOTYPES ---------------------------------------------- */

extern void Adc_Callback(uint16 adcResult_ui16);


/* ------------------------------------ GLOBAL VARIABLES ---------------------------------------- */

/* ------------------------------------ PRIVATE VARIABLES --------------------------------------- */

/* the config structure has to be filled with the given types defined in adc.h.
 * no validity check is applied at the time of setting the registers. porting is faster
 * done but with the trade-off that one has to explicitly know his target architecture.
 */
static const Adc_ConfigType Adc_initialConfiguration_s =
{
        ADC_MODULE_ENABLED,                 // enableState_e;
        ADC_INTERRUPT_DISABLED,             // interruptState_e;
        ADC_CLOCK_PRESCALER_64,             // prescalerControl_e;
        ADC_TRIGGER_SINGLE_SHOT,            // triggerControl_e;
        ADC_REFERENCE_AVCC,                 // referenceControl_e;
        ADC_CHANNEL_7,                      // defaultChannel_e;
        ADC_DIGITAL_INPUT_DISABLE_NONE,     // digitalInputDisable_e;
        ADC_CALLBACK_NULL_PTR,              // callbackFunc_pv;
        ADC_AVERAGE_4_SAMPLES               // averageControl_e;
};


/* ------------------------------------ GLOBAL FUNCTIONS ---------------------------------------- */

const void *Adc_GetLcfgData(void)
{
   return ((const void*) &Adc_initialConfiguration_s);
}


/* ------------------------------------ PRIVATE FUNCTIONS --------------------------------------- */

/* ************************************ E O F *************************************************** */
