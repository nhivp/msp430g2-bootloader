#include <msp430g2553.h>


static void Timer_Init(void);
unsigned int count;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    P1DIR |= BIT6;
    Timer_Init();
    count = 0;
    _enable_interrupts();

    while(1);
    return 0;
}

static void Timer_Init(void)
{
    TA0CTL |= 0x01 << 2;
    TA0CTL = (0x02 << 8) + (0x03 << 6) + (0x01 << 4); //125000
    TA0CCTL0 |= 0x01 << 4;
    TA0CCR0 = 62500; //0.5s
}

void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer_ISR()
{
    count ++;
    if(count >= 2)
    {
        P1OUT ^= BIT6;
        count = 0;
    }
}
