#define F_CPU 1000000UL
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#define OUTPUT_SIZE 3

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int apple = 0, orange = 0, banana = 0, total = 0;
char output[OUTPUT_SIZE+1];
int i, temp;

void lcdDisplay(int pos, int cursor, int number) {
	Lcd4_Set_Cursor(pos,cursor);
	output[OUTPUT_SIZE]='\0';
	i=OUTPUT_SIZE-1, temp=number;
	if(!temp) output[i--]='0';
	while(temp&&i>=0) {
		output[i--]=temp%10 + '0';
		temp/=10;
	}
	while(i>=0) output[i--]=' ';
	Lcd4_Write_String(output);
}

void updateLcd() {
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Ap:   0, Or:   0"); //4-6, 13-15
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Ba:   0, To:   0"); //4-6, 13-15
	lcdDisplay(1,4,apple);
	lcdDisplay(1,13,orange);
	lcdDisplay(2,4,banana);
	lcdDisplay(2,13,total);
}

void displayName(int t) {
	Lcd4_Set_Cursor(1,0);
	if(t==1)
	Lcd4_Write_String("Orange          ");
	else if(t==2)
	Lcd4_Write_String("Apple           ");
	else if(t==3)
	Lcd4_Write_String("Banana          ");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("                ");
	_delay_ms(1000);
}

int main(void)
{
	//_delay_ms(5000);
	DDRB = 0x00;
	DDRD = 0xFF;
	DDRC = 0xFF;
	Lcd4_Init();
	
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Ap:   0, Or:   0"); //4-6, 13-15
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Ba:   0, To:   0"); //4-6, 13-15
	
	int status = 5, prev= 5;
	
	while(1)
	{
		int tmp = PINB;
		prev = status;
		status = tmp&3;
		if(status!=prev&&status) {
			total++;
			if (status==1)
			orange++;
			else if (status==2)
			apple++;
			else if (status==3)
			banana++;
			else
			total--;
			displayName(status);
			updateLcd();
		}
		
		_delay_ms(50);
		
	}
	return 0;
}