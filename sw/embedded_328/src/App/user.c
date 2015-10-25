//+++Init user stuff+++
#define F_CPU 16000000UL
#define BAUD_HIGH 38400UL      /**< the high baudrate */

/** Calculated UBRR value for high baud rate */
#define UBRR_VAL_HIGH ((F_CPU+BAUD_HIGH*8)/(BAUD_HIGH*16)-1)


static void uart_init_high(uint8_t rxen, uint8_t txen, uint8_t rxcie)
{
    UCSR0B = 0x00;
    UCSR0B |= (rxen << RXEN0) | (rxcie << RXCIE0) | (txen << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UBRR0H = UBRR_VAL_HIGH >> 8;
    UBRR0L = UBRR_VAL_HIGH & 0xFF;
}

static void uart_putc(char byte)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = byte;         /* sende Zeichen */
}

void uart_puts(const char *s) {
   while (*s ) {
      uart_putc(*s);
      s++;
   }
}



static void Os_InitUserEnvironment()
{
//Create Tasks for displaying minutes, hours and time calculation
Os_TaskInsert(&TaskA, TaskA_Handler, 100);
Os_TaskInsert(&TaskB, TaskB_Handler, 100);
//task_insert(&TaskC, TaskC_Handler, 9);

uart_init_high(0, 1, 0);

}

void Os_ShutdownUserEnvironment()
{
asm("nop"); //only placebo
}

//+++Task functions+++

//this is the display task for the minutes
void TaskA_Handler()
{
while (1)
	{
        uart_puts("A");
        Os_TaskSleepForLaps(&TaskB, 1);
//        task_sleep_for_laps(&TaskA, 500);
	}
}

//this is the display task for the hours
void TaskB_Handler()
{
while (1)
	{
        uart_puts("B");
        Os_TaskSleepForLaps(&TaskA, 1);
	}
}

////this calculates the time
//void TaskC_Handler()
//{
//uint8_t h=0,m=0,s=0;
//unsigned char calc_msg;
//while (1)
//	{
//        uart_puts("C");
//        task_sleep_for_laps(&TaskC, 2000);
//	}
//}

//+++ISR - Interrupt Service Routine+++

//ISR for Timer2 - Clock timer
void tick_time()
{
static uint8_t zaehler;
zaehler++;
if (zaehler == 122)	 //Interrupt is called @ 8 Mhz / (256*256) = 122 times per second
	{
	//we have a new second so send a message to time_calc task;
	zaehler = 0;
	}
//debounce for buttons (enable button interrupts again after 0,3 seconds;
else if (!(zaehler % 30))
	{
	PCMSK0 |= _BV(PCINT1);	
	PCMSK0 |= _BV(PCINT2);	
	}

}

//ISR for Pin Change Interrupts - Buttons

void button_press_12()
{
// button1 is pressed
if (! (PINB & _BV(PB1))) 
	{
	//disable interrupt fpr debounce
	PCMSK0 &= ~_BV(PCINT1);					
	//Button 1 is pressed so send a "1" to time_calc_task
	return;
	}
// button2 is pressed
if (! (PINB & _BV(PB2))) 
	{
	//disable interrupt fpr debounce
	PCMSK0 &= ~_BV(PCINT2);		
	//Button 2 is pressed so send a "2" to time_calc_task
	return;
	}
}

