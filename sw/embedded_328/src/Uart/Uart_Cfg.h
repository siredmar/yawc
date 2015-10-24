/*
 * Uart_Cfg.h
 *
 *  Created on: Oct 18, 2015
 *      Author: armin
 */

#ifndef UART_UART_CFG_H_
#define UART_UART_CFG_H_

#define MAXUARTSTR (250u)

#define UART_NUMBER_OF_UARTS (1)

/* UCSRnC */
#define UART_UCPOL_BIT_POSITION_UI8 ((uint8)0)
#define UART_UCPHA_BIT_POSITION_UI8 ((uint8)1)
#define UART_UCSZ0_BIT_POSITION_UI8 ((uint8)1)
#define UART_UDORD_BIT_POSITION_UI8 ((uint8)2)
#define UART_UCSZ1_BIT_POSITION_UI8 ((uint8)2)
#define UART_USBS_BIT_POSITION_UI8 ((uint8)3)
#define UART_UPM0_BIT_POSITION_UI8 ((uint8)4)
#define UART_UPM1_BIT_POSITION_UI8 ((uint8)5)
#define UART_UMSEL0_BIT_POSITION_UI8 ((uint8)6)
#define UART_UMSEL1_BIT_POSITION_UI8 ((uint8)7)

/* UCSRnB */
#define UART_TXB8_BIT_POSITION_UI8 ((uint8)0)
#define UART_RXB8_BIT_POSITION_UI8 ((uint8)1)
#define UART_UCSZ2_BIT_POSITION_UI8 ((uint8)2)
#define UART_TXEN_BIT_POSITION_UI8 ((uint8)3)
#define UART_RXEN_BIT_POSITION_UI8 ((uint8)4)
#define UART_UDRIE_BIT_POSITION_UI8 ((uint8)5)
#define UART_TXCIE_BIT_POSITION_UI8 ((uint8)6)
#define UART_RXCIE_BIT_POSITION_UI8 ((uint8)7)

/* UCSRnA */
#define UART_MPCM_BIT_POSITION_UI8 ((uint8)0)
#define UART_U2X_BIT_POSITION_UI8 ((uint8)1)
#define UART_UPE_BIT_POSITION_UI8 ((uint8)2)
#define UART_DOR_BIT_POSITION_UI8 ((uint8)3)
#define UART_FE_BIT_POSITION_UI8 ((uint8)4)
#define UART_UDRE_BIT_POSITION_UI8 ((uint8)5)
#define UART_TXC_BIT_POSITION_UI8 ((uint8)6)
#define UART_RXC_BIT_POSITION_UI8 ((uint8)7)

#define UART_UDR0_ADDRESS    ((uint8)0xC6)
#define UART_UCSR0A_ADDRESS  ((uint8)0xC0)
#define UART_UCSR0B_ADDRESS  ((uint8)0xC1)
#define UART_UCSR0C_ADDRESS  ((uint8)0xC2)
#define UART_UBRR0L_ADDRESS  ((uint8)0xC4)
#define UART_UBRR0H_ADDRESS  ((uint8)0xC5)

#if UART_NUMBER_OF_UARTS >= 2
#define UART_UDR1_ADDRESS    ((uint8)TODO)
#define UART_UCSR1A_ADDRESS  ((uint8)TODO)
#define UART_UCSR1B_ADDRESS  ((uint8)TODO)
#define UART_UCSR1C_ADDRESS  ((uint8)TODO)
#define UART_UBRR1L_ADDRESS  ((uint8)TODO)
#define UART_UBRR1H_ADDRESS  ((uint8)TODO)
#endif
#if UART_NUMBER_OF_UARTS >= 3
#define UART_UDR2_ADDRESS    ((uint8)TODO)
#define UART_UCSR2A_ADDRESS  ((uint8)TODO)
#define UART_UCSR2B_ADDRESS  ((uint8)TODO)
#define UART_UCSR2C_ADDRESS  ((uint8)TODO)
#define UART_UBRR2L_ADDRESS  ((uint8)TODO)
#define UART_UBRR2H_ADDRESS  ((uint8)TODO)
#endif
#if UART_NUMBER_OF_UARTS >= 4
#define UART_UDR3_ADDRESS    ((uint8)TODO)
#define UART_UCSR3A_ADDRESS  ((uint8)TODO)
#define UART_UCSR3B_ADDRESS  ((uint8)TODO)
#define UART_UCSR3C_ADDRESS  ((uint8)TODO)
#define UART_UBRR3L_ADDRESS  ((uint8)TODO)
#define UART_UBRR3H_ADDRESS  ((uint8)TODO)
#endif


#endif /* UART_UART_CFG_H_ */
