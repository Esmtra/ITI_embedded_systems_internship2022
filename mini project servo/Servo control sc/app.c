/******************************************************************************
 *
 * Module: Servo Motor control
 *
 * File Name:Servo.c
 *
 * Description: Source file for Servo Motor control
 *
 * Author: Eslam Mtra
 *
 * Created on: Sep 13, 2022
 *
 *******************************************************************************/

#include <avr/io.h>		/* Include AVR std. library file */
#include <stdio.h>		/* Include std. library file */
#include <util/delay.h>		/* Include Delay header file */

#define F_CPU 8000000UL		/* Define CPU Frequency e.g. here its 8MHz */


/********************************************************/
/* Functions  */
/********************************************************/


/* ADC Initialization function */
/********************************************************/
void ADC_Init()
{
	DDRA=0x00;		/* Make ADC port as input */
	ADCSRA = 0x87;		/* Enable ADC, with freq/128  */
	ADMUX = 0x40;		/* Vref: Avcc, ADC channel: 0 */
}

/* ADC Read function */
/********************************************************/
int ADC_Read(char channel)
{
	ADMUX = 0x40 | (channel & 0x07);/* set input channel to read */
	ADCSRA |= (1<<ADSC);	/* Start ADC conversion */

	/* Wait until end of conversion by polling ADC interrupt flag */
	while (!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<<ADIF);	/* Clear interrupt flag */
	_delay_ms(1);		/* Wait a little bit */
	return ADCW;		/* Return ADC word */
}


/********************************************************/
/* main Function  */
/********************************************************/
int main(void)
{
	ADC_Init();		/* Initialize ADC */
	DDRD |= (1<<PD5);	/* Make OC1A pin as output */
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	while(1)
	{
	   OCR1A = 65 + (ADC_Read(0)/4.35);
	   /* Read ADC channel 0 and make count in between 65-300 i.e. 0.5-2.4 mSec duty cycle variation */
	}
}
