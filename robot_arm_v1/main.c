#define F_CPU 16000000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pca9685.h" // include the PCA9685 library
#include "UART.h"

// LED (상태등)
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1 PB0
#define LED2 PB1

// 스위치
#define switch1 PB2

// servo pin
#define SERVO_A(x) (x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
#define SERVO_B(x) (x + 4)		// SERVO_B(1) = 5, 6, 7, 8, 9

// pulse
#define ANGLE(x) ((10 * x) + 600)		// 600 ~ 2400



void INIT_SERVO(){
	
	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	int i;
	
	for (i = 1; i <= 5; i++){
		pca9685_pwm(SERVO_A(i), ANGLE(90));
	}
}

void move_servo(int servo, int start_angle, int end_angle){
	int angle;

	if (start_angle <= end_angle){
		for (angle = start_angle; angle <= end_angle; ) {
			if (PINB & (1 << switch1)) {
				PORTB &= ~0x02;
				pca9685_pwm(servo, angle);
				angle += 10;
			}
			else {
				PORTB |= 0x02;
				break;
			}
			_delay_ms(15);
		}
	}
	
	else {
		for (angle = start_angle; angle >= end_angle; ) {
			if (PINB & (1 << switch1)) {
				PORTB &= ~0x02;
				pca9685_pwm(servo, angle);
				angle -= 10;
			}
			else {
				PORTB |= 0x02;
				break;
			}
			_delay_ms(15);
		}
	}
}

int main(void)
{
	LED_DDR |= (1 << LED1) | (1 << LED2);
	DDRB &= ~(1 << switch1);
	
	// servo motor 초기화
	INIT_SERVO();
	_delay_ms(1000);
	
	//move_servo(SERVO_A(4), ANGLE(90), ANGLE(0));
	
	while (1){
		// LED ON
		PORTB |= (1 << LED1);
		
		
		
		if (!(PINB & (1 << switch1))) {
			_delay_ms(500);
			PORTB |= (1 << LED2);
			move_servo(SERVO_A(1), ANGLE(90), ANGLE(0));
			move_servo(SERVO_A(2), ANGLE(90), ANGLE(0));
			move_servo(SERVO_A(4), ANGLE(90), ANGLE(0));
			move_servo(SERVO_A(5), ANGLE(90), ANGLE(30));
			//move_servo(SERVO_A(4), ANGLE(0), ANGLE(90));
			move_servo(SERVO_A(2), ANGLE(30), ANGLE(90));
			//pca9685_pwm(SERVO_A(2), ANGLE(90));
			move_servo(SERVO_A(1), ANGLE(0), ANGLE(90));
			//INIT_SERVO();
			
			move_servo(SERVO_A(1), ANGLE(90), ANGLE(180));
			move_servo(SERVO_A(2), ANGLE(90), ANGLE(00));
			move_servo(SERVO_A(5), ANGLE(0), ANGLE(90));
			_delay_ms(2000);
			INIT_SERVO();
		}
		else {
			PORTB &= ~(1 << LED2);
		}
		
		// LED OFF
		PORTB &= ~(1 << LED1);
	}
	INIT_SERVO();

	return 0;
}