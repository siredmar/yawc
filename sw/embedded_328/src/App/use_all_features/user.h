//init user specific interrupts and other stuff here
//you can use all interrupts and timers except timer0 OVF and timer0 speed/counter-register settings

//Prototypes

void display_minutes(void);
void display_hours(void);
void time_calc(void); 
void tick_time(void);
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

Task time_calc_task, display_minutes_task, display_hours_task;

Semaphor PORTC_semaphor, PORTD_semaphor;

Queue minutes_queue, hours_queue, time_calc_queue;
