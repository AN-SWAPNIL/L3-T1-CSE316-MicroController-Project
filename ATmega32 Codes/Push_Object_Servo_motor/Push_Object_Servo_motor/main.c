#define F_CPU 8000000UL    /* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>      /* Include AVR std. library file */
#include <stdio.h>      /* Include std. library file */
#include <util/delay.h>    /* Include Delay header file */

int main(void)
{
	DDRB = 0b00000000;
	DDRA = 0b11111111;
	DDRD |= (1<<PD5);  /* Make OC1A pin as output */
	TCNT1 = 0;      /* Set timer1 count zero */
	ICR1 = 2499;    /* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	unsigned char status=0;
	int current=175;
	OCR1A=current;
	while(1)
	{
		status=(PINB&1);
		
		if(status==0){
			current = 175;
		} else {
			current = 300;
		}
		OCR1A=current;
		PORTA = status;
		_delay_ms(1500);
	}
}