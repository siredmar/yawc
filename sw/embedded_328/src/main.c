#include "Std_Types.h"
#include "nano_os.h"

int main()
{
    uint8 cnt = 0;
    kernel();

    sei(); /* Enable the interrupts */
    while(1)
    {
        /* do nothing */
    }
    return 0;
}
