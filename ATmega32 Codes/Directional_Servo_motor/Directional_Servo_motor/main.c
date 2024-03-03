#define F_CPU 8000000UL    /* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>      /* Include AVR std. library file */
#include <stdio.h>      /* Include std. library file */
#include <util/delay.h>    /* Include Delay header file */

int main(void)
{
	DDRB = 0b00000000;
	DDRD |= (1<<PD5);  /* Make OC1A pin as output */
	TCNT1 = 0;      /* Set timer1 count zero */
	ICR1 = 2499;    /* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	unsigned char status=0;
	int current=190;
	OCR1A=current;
	int prev=180;
	while(1)
	{
		status=(PINB);
		status=(status&3);
		
		if(status==1){
			current = 135;		/* Set servo shaft at -30 position */
		} else if(status==2){
			current = 180;		/* Set servo shaft at 0 position */
		} else if(status==3){
			current = 225;		/* Set servo at +30 position */
		}
		
		if(prev!=current)
		OCR1A=current;
		prev=current;
		_delay_ms(1500);
	}
}