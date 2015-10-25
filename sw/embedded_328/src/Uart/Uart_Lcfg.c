/*
 * Uart_Lcfg.c
 *
 *  Created on: Oct 19, 2015
 *      Author: armin
 */


#include "Uart_Lcfg.h"
#include "Uart.h"

extern void Uart0_RxCallback(uint16 data);

const Uart_ConfigType Uart_InitialConfiguration_s =
{
        16000000ul,
        {
                UART_HWUNIT_0,
                115200UL,
                UART_CHAR_8_BIT,
                UART_PARITY_DISABLED,
                UART_STOP_BITS_1,
                UART_DOUBLE_TX_SPEED_ENABLED,
                UART_RECEPTION_ENABLED,
                UART_TRANSMISSION_ENABLED,
                UART_RX_INTERRUPT_ENABLED,
                UART_TX_INTERRUPT_ENABLED,
                Uart0_RxCallback,
                NULL_PTR
        },
//        {
//                UART_HWUNIT_1,
//                115200UL,
//                UART_CHAR_8_BIT,
//                UART_PARITY_DISABLED,
//                UART_STOP_BITS_1,
//                UART_DOUBLE_TX_SPEED_ENABLED,
//                UART_RECEPTION_ENABLED,
//                UART_TRANSMISSION_ENABLED,
//                UART_RX_INTERRUPT_ENABLED,
//                UART_TX_INTERRUPT_ENABLED,
//                Uart0_RxCallback,
//                NULL_PTR
//        },
//        {
//                UART_HWUNIT_2,
//                115200UL,
//                UART_CHAR_8_BIT,
//                UART_PARITY_DISABLED,
//                UART_STOP_BITS_1,
//                UART_DOUBLE_TX_SPEED_ENABLED,
//                UART_RECEPTION_ENABLED,
//                UART_TRANSMISSION_ENABLED,
//                UART_RX_INTERRUPT_ENABLED,
//                UART_TX_INTERRUPT_ENABLED,
//                Uart0_RxCallback,
//                NULL_PTR
//        },
//        {
//                UART_HWUNIT_3,
//                115200UL,
//                UART_CHAR_8_BIT,
//                UART_PARITY_DISABLED,
//                UART_STOP_BITS_1,
//                UART_DOUBLE_TX_SPEED_ENABLED,
//                UART_RECEPTION_ENABLED,
//                UART_TRANSMISSION_ENABLED,
//                UART_RX_INTERRUPT_ENABLED,
//                UART_TX_INTERRUPT_ENABLED,
//                Uart0_RxCallback,
//                NULL_PTR
//        }
};

const void *Uart_GetLcfgData(void)
{
   return ((void*) &Uart_InitialConfiguration_s);
}
