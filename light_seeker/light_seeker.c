#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include <util/delay.h>
#include "uart.h"
#include "uart.c"
#include <avr/interrupt.h>


void forward()
{

sbi(PORTB,3);   //high ON PORTB 3

sbi(PORTB,0);    //HIGH ON PORTB 0
cbi(PORTD,7);    //LOW ON PORTD 7

sbi(PORTB,5);   //HIGH ON PORTB 5
cbi(PORTB,4);   //LOW ON PORTB 4
}

void reverse()
{
sbi(PORTB,3);   //high ON PORTB 3

cbi(PORTB,0);    //LOW ON PORTB 0
sbi(PORTD,7);    //HIGH ON PORTD 7

cbi(PORTB,5);   //LOW ON PORTB 5
sbi(PORTB,4);   //HIGH ON PORTB 4
}


void left()
{
sbi(PORTB,3);   //high ON PORTB 3

sbi(PORTB,0);    //HIGH ON PORTB 0
cbi(PORTD,7);    //LOW ON PORTD 7


cbi(PORTB,5);   //LOW ON PORTB 5
sbi(PORTB,4);   //HIGH ON PORTB 4
}

void right()
{
sbi(PORTB,3);   //high ON PORTB 3

sbi(PORTB,5);   //HIGH ON PORTB 5
cbi(PORTB,4);   //LOW ON PORTB 4


cbi(PORTB,0);    //LOW ON PORTB 0
sbi(PORTD,7);    //HIGH ON PORTD 7
}

void stop()
{
cbi(PORTB,3);    //LOW ON PORTB 3

cbi(PORTB,4);   //LOW ON PORTB 4
cbi(PORTB,0);    //LOW ON PORTB 0

cbi(PORTB,5);   //LOW ON PORTB 5
cbi(PORTD,7);    //LOW ON PORTD 7
}
void rightforward()
{
		sbi(PORTB,3);
		
		cbi(PORTD,7);		
		sbi(PORTB,0);
		
		cbi(PORTB,5);
		cbi(PORTB,4);
}



void leftforward()
{
		sbi(PORTB,3);
		
		cbi(PORTD,7);		
		cbi(PORTB,0);
		
		sbi(PORTB,5);
		cbi(PORTB,4);

}
void motorinit()
{

		sbi(DDRB,3); //enable
		
		sbi(DDRD,7);  //input1		
		sbi(DDRB,0);  //input2
		
		sbi(DDRB,5);   //input3
		sbi(DDRB,4);   //input4
		

}



int readADC(int channel){
	int w;
		
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

	ADMUX = channel;
	
	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	//ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

	
	//ADCSRA |= (1 << ADFR);  // Se++t ADC to Free-Running 4
	ADCSRA |= (1 << ADEN);  // Enable ADC
	
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
	
	
	 //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   ADCSRA|=(1<<ADIF);	
	
	 w=ADCL;
     w|=(ADCH<<8);
 
     return w;
}


void main()
{
   
DDRC=0x0f;
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	sei();	
	int a,b;	
	char str[4],str1[4];
	motorinit();
	
	_delay_ms(3000);
	
	uart_puts("INITIALIZE THE SYSTEM");
	
while(1)	
	{
		uart_puts("WELCOE\n\r");
		
		a= readADC(5);
		b= readADC(4);
		itoa(a,str,10);
		itoa(b,str1,10);
		uart_puts(str);
		uart_puts("\t");
		uart_puts(str1);
		uart_puts("\n\r");
		uart_puts("\n\r");

		//_delay_ms(200);
		
		
		
		
		
		 if((a<=80)&&(b>70))	
		{
		leftforward();
		uart_puts("leftfor\n\r");
		
		}
		
		else if((a>=80)&&(b<=50))
		{
		
		rightforward();
		uart_puts("rigthfor\n\r");	
		
			
		}
	/*	else if(((a>=60)&&(b<=70))&&((b>70)&&(b<=80)))
		{
		forward();		
		}*/
		
		
		else 
		{
		forward();
		uart_puts("stop\n\r");
		
		}
		_delay_ms(200);

	}
}