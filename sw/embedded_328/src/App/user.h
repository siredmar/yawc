//init user specific interrupts and other stuff here
//you can use all interrupts and timers except timer0 OVF and timer0 speed/counter-register settings

//Prototypes

void TaskA_Handler();
void TaskB_Handler();
void TaskC_Handler();
void button_press_12(void);

//Interrupt Handler

SIGNAL (TIMER2_OVF_vect)
{
tick_time();
}

SIGNAL(SIG_PIN_CHANGE0)
{
button_press_12();
}

Os_TaskType TaskA, TaskB;//, TaskC;

