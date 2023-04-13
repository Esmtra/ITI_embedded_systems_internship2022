/******************************************************************************
 *
 * Module: application
 *
 * File Name: application.c
 *
 * Description: Source file for the application
 *
 * Author: Eslam Mtra
 *
 *******************************************************************************/
#include "keypad.h"
#include "gpio.h"
#include "lcd.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "adc.h"
#include "lm35_sensor.h"
#include <avr/delay.h>
/********************************************************/
/* Functions  */
/********************************************************/



/* Description: A function to get the password from user and store it in a given array
/********************************************************/
void getPassword(uint8 *arrayName) {
	LCD_moveCursor(1, 0);
	uint8 i = 0;
	uint8 key;

	while (i != 4) {
		key = KEYPAD_getPressedKey();
		if (key >= 0 && key <= 9) {
			LCD_displayCharacter('*');
			*(arrayName + i) = key;
			i++;
		}
		_delay_ms(500);
	}

	while (KEYPAD_getPressedKey() != 35)
		;
	key = 0;

}


/*Description: a function to compare the stored pass with the received one
/********************************************************/

int compare_passwords(uint8 *user_pass) {

	getPassword(user_pass);

	if (user_pass[0] == 0 && user_pass[1] == 0) {
		if (user_pass[2] == 0 && user_pass[3] == 0) {
			return 1;
		}
	}
	return 0;
}



/* Description: a function to control the duty cycle of the output PWM signal (motor speed) based on the temperature value
/********************************************************/

void update_motor_speed(uint8_t temp) {

	if (temp >= 120) {

		DcMotor_Rotate(DC_MOTOR_CW, 100); /* Rotates the motor with 100% from its speed */
	} else if (temp >= 90) {

		DcMotor_Rotate(DC_MOTOR_CW, 75); /* Rotates the motor with 75% from its speed */
	} else if (temp >= 60) {

		DcMotor_Rotate(DC_MOTOR_CW, 50); /* Rotates the motor with 50% from its speed */
	} else if (temp >= 30) {

		DcMotor_Rotate(DC_MOTOR_CW, 25); /* Rotates the motor with 25% from its speed */
	} else {

		DcMotor_Rotate(DC_MOTOR_STOP, 0); /* Stop the motor */
	}
}


/********************************************************/
/* main Function*/
/********************************************************/

int main(void) {

/* initialization part */
/********************************************************/

	LCD_init();
	DcMotor_Init();
	DcMotor_Rotate(DC_MOTOR_STOP, 0);
	BZR_init();
	ADC_ConfigType ADC_Configurations = { INTERNAL_VOLT, F_CPU_8 };
	ADC_init(&ADC_Configurations);


/* get password from user at startup & compare it with stored password */
/********************************************************/

	int user_pass[4];
	int g_wrong_passwords = 0;
	;
	while (!compare_passwords(user_pass)) {
		LCD_clearScreen();
		LCD_displayString("Wrong,try again");
		_delay_ms(1000);
		LCD_clearScreen();
		g_wrong_passwords++;
/* check wrong trail and take action over 3 times  */
/********************************************************/
		if (g_wrong_passwords == 3) {
			BZR_ON();
			_delay_ms(1500);
			BZR_OFF();
			g_wrong_passwords = 0; /* reset the counter */
		}
	}

/* start motor control system based on the temperature value  */
/********************************************************/
	LCD_clearScreen();
	LCD_displayString("Weclome");

	_delay_ms(1000);

	uint8_t temp;
	while (1) {
		LCD_clearScreen();
		temp = LM35_getTemperature();
		LCD_intgerToString(temp);
		update_motor_speed(temp);
		_delay_ms(1000);
	}
}
