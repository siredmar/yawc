//+++Init user stuff+++
static void init_user_environment()
{
//Create Tasks for displaying minutes, hours and time calculation
Os_TaskInsert(&display_minutes_task, display_minutes, Low);
Os_TaskInsert(&display_hours_task, display_hours, Low);
Os_TaskInsert(&time_calc_task, time_calc, Low);

//init Queues

Os_QueueInit(&minutes_queue,1,2);
Os_QueueInit(&hours_queue,1,1);
Os_QueueInit(&time_calc_queue,1,1);

semaphore_init(&PORTC_semaphor);
semaphore_init(&PORTD_semaphor);

//set DDRB and DDRC as output
DDRC=0xFF;
DDRD=0xFF;

//setup timer2 
TCCR2B = _BV(CS22) | _BV(CS21); // div by 256
// We will overflow 122 times a second, and call an interrupt
// enable interrupt
TIMSK2 = _BV(TOIE2);

//Enable pullup resistor on PIN PB1 and PB2 
PORTB = _BV(PB1) | _BV(PB2);

//These are the interrupts for the buttons
PCICR = _BV(PCIE0);
PCMSK0 = _BV(PCINT1) | _BV(PCINT2);
}

void shutdown_user_environment() 
{
asm("nop"); //only placebo
}

//+++Task functions+++

//this is the display task for the minutes
void display_minutes()
{
if (!(Os_QueueExists(&minutes_queue)) || !(Os_MemoryGetFreeBlocks()))
	Os_TaskRemove(&display_minutes_task);
unsigned char* min_ptr = Os_MemoryAllocateBlock();
if (min_ptr == 0)
	Os_TaskRemove(&display_minutes_task);
while (1)
	{
	//Wait for new minute message
	Os_QueueWaitForMessage(&minutes_queue);
	//set PORTD to new minute value
	Os_QueueReadItem(&minutes_queue,min_ptr,0,1);								
	semaphore_wait(&PORTD_semaphor);
	semaphore_set(&PORTD_semaphor);
	PORTD = min_ptr[1];
	semaphore_free(&PORTD_semaphor);
	Os_TaskSleepForLaps(&display_minutes_task, 20);
	}
}

//this is the display task for the hours
void display_hours()
{
if (!(Os_QueueExists(&hours_queue)) || !(Os_MemoryGetFreeBlocks()))
	Os_TaskRemove(&display_hours_task);
unsigned char* hrs_ptr = Os_MemoryAllocateBlock();
if (hrs_ptr == 0)
	Os_TaskRemove(&display_hours_task);
while (1)
	{
	//Wait for new hour message
	Os_QueueWaitForMessage(&hours_queue);
	//set PORTC to new hour value
	*hrs_ptr = Os_QueueReadByte(&hours_queue,0,1);
	semaphore_wait(&PORTC_semaphor);
	semaphore_set(&PORTC_semaphor);
	PORTC = *hrs_ptr;
	semaphore_free(&PORTC_semaphor);
	Os_TaskSleepForLaps(&display_hours_task, 20);
	}
}

//this calculates the time
void time_calc()
{
uint8_t h=0,m=0,s=0,a[2];
unsigned char calc_msg;
while (1)
	{
	//wait for message. A message appears every second or when a button is pressed. 
	Os_QueueWaitForMessage(&time_calc_queue);
	calc_msg = Os_QueueReadByte(&time_calc_queue,0,1);
	// if button 1 is pressed increase hours
	if (calc_msg == 1)
		{
		if (h < 23)
			h++;
		else
			h = 0;
		}
	// if button 2 is pressed increase minutes
	else if (calc_msg == 2)
		{
		if (m < 59)
			m++;
		else
			m = 0;
		}
	// else it must be a new second
	else
		{
		TASK_ENTER_CRITICAL;
		s++;
		if (s == 60)
			{
			s = 0;
			m++;
			if (m == 60)
				{
				m = 0;
				h++;
				if (h == 24)
					h = 0;
				}
			}
		TASK_LEAVE_CRITICAL; 
		}
	//send time to display tasks;
	a[0] = 60-m;
	a[1] = m;
	Os_QueueWriteItem(&minutes_queue,a);
	Os_QueueWriteByte(&hours_queue,h);
	}
}		 

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
	Os_QueueWriteByte(&time_calc_queue, 128); //anything but 1 or 2
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
	Os_QueueWriteByte(&time_calc_queue, 1);
	return;
	}
// button2 is pressed
if (! (PINB & _BV(PB2))) 
	{
	//disable interrupt fpr debounce
	PCMSK0 &= ~_BV(PCINT2);		
	//Button 2 is pressed so send a "2" to time_calc_task					
	Os_QueueWriteByte(&time_calc_queue, 2);
	return;
	}
}

