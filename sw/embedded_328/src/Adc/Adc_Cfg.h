/* *************************************************************************************************
 * file:        adc_cfg.h
 *
 *          The adc module address configuration file.
 *
 * author:      Armin Schlegel, Mr. L.
 * date:        09.10.2014
 * version:     0.3   worky, testing
 *
 * file history:
 *          09.10.2014  A. Schlegel file created, basic version
 *          14.10.2014  Mr. L.      renaming, add DIDR0 defines, nicify layout, add comments
 *
 * notes:
 *          - none -
 *
 * copyright:   http://creativecommons.org/licenses/by-nc-sa/3.0/
 **************************************************************************************************/
#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_
/* ============================================================================================== */

/* ------------------------------------ INCLUDES ------------------------------------------------ */

/* ------------------------------------ DEFINES ------------------------------------------------- */

/* register addresses */
#define ADC_ADCL_ADDRESS    ((uint8)0x78)
#define ADC_ADCH_ADDRESS    ((uint8)0x79)
#define ADC_ADCSRA_ADDRESS  ((uint8)0x7A)
#define ADC_ADCSRB_ADDRESS  ((uint8)0x7B)
#define ADC_ADMUX_ADDRESS   ((uint8)0x7C)
#define ADC_DIDR0_ADDRESS   ((uint8)0x7E)

/* ADMUX register bit positions */
#define ADC_REFS1 ((uint8)7)
#define ADC_REFS0 ((uint8)6)
#define ADC_ADLAR ((uint8)5)
#define ADC_MUX4  ((uint8)4)
#define ADC_MUX3  ((uint8)3)
#define ADC_MUX2  ((uint8)2)
#define ADC_MUX1  ((uint8)1)
#define ADC_MUX0  ((uint8)0)

/* ADCSRA register bit positions */
#define ADC_ADEN  ((uint8)7)
#define ADC_ADSC  ((uint8)6)
#define ADC_ADATE ((uint8)5)
#define ADC_ADIF  ((uint8)4)
#define ADC_ADIE  ((uint8)3)
#define ADC_ADPS2 ((uint8)2)
#define ADC_ADPS1 ((uint8)1)
#define ADC_ADPS0 ((uint8)0)

/* ADCSRB register bit positions */
#define ADC_ADTS2 ((uint8)2)
#define ADC_ADTS1 ((uint8)1)
#define ADC_ADTS0 ((uint8)0)

/* DIDR0 register bit positions */
#define ADC_ADC7D ((uint8)7)
#define ADC_ADC6D ((uint8)6)
#define ADC_ADC5D ((uint8)5)
#define ADC_ADC4D ((uint8)4)
#define ADC_ADC3D ((uint8)3)
#define ADC_ADC2D ((uint8)2)
#define ADC_ADC1D ((uint8)1)
#define ADC_ADC0D ((uint8)0)


/* ------------------------------------ TYPE DEFINITIONS ---------------------------------------- */

/* ------------------------------------ GLOBAL VARIABLES ---------------------------------------- */

/* ------------------------------------ PROTOTYPES ---------------------------------------------- */


/* ************************************ E O F *************************************************** */
#endif /* _ADC_CFG_H_ */
