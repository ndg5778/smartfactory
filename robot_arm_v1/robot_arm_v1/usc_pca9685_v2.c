#define F_CPU 16000000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include "pca9685.h" // include the PCA9685

#define servo1 (uint8_t)0
#define servo2 (uint8_t)1
#define servo3 (uint8_t)2
#define servo4 (uint8_t)3
#define servo5 (uint8_t)4

// LED (상태등)
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1 PB0
#define LED2 PB1

// 스위치
#define switch1 PB2

#define SERVO_A(x) (uint8_t)(x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
#define SERVO_B(x) (uint8_t)(x + 4)		// SERVO_B(1) = 5, 6, 7, 8, 9

// pulse
#define ANGLE(x) (uint16_t)((10 * x) + 600)		// 600 ~ 2400

#define servo_max 18

uint16_t move_Aarm_coord[9][servo_max][3] = {
	{
		{1, 90, 92}, {4, 90, 140}, {2, 90, 65}, {3, 90, -23}, {5, 45, 15},
		{4, 140, 180}, {3, -23, 30}, {4, 180, 90}, {3, 30, 90}, {2, 65, 90},
		{1, 92, 135}, {2, 90, 85}, {3, 90, 30}, {4, 90, 150}, {3, 30, -25}, {5, 15, 45}, {2, 85, 90}, {1, 135, 90}
			
	},
	
	{
		{1, 90, 72}, {4, 90, 180}, {2, 90, 68}, {3, 90, -25}, {4, 180, 130}, {5, 45, 15},
		{4, 130, 180}, {2, 68, 90}, {4, 180, 90}, {3, -25, 90},
		{1, 72, 135}, {2, 90, 85}, {3, 90, 30}, {4, 90, 150}, {3, 30, -25}, {5, 15, 45}, {2, 85, 90}, {1, 135, 90}
	}
};


// 서보모터 초기화
void INIT_SERVO();
// 서보모터 움직이기
void MoveServo(uint8_t, uint16_t, uint16_t);
// 로봇암 쉽게 움직이기
void MoveRobotArm(uint8_t, uint8_t);

int main(void)
{
	LED_DDR |= (1 << LED1) | (1 << LED2);
	//LED_PORT |= (1 << LED2);
	
	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output

	//LED_PORT |= (1 << LED1);
	_delay_ms(5);
	
	//uint16_t angle = 0;
	_delay_ms(1000);

	//INIT_SERVO();

	while (1) {
		LED_PORT |= (1 << LED1);
		
		
		INIT_SERVO();
		_delay_ms(1000);
		
		/* 함수 이용하기 */
		MoveRobotArm(1, 1);
	
		///* 집기 */
		//MoveServo(SERVO_A(1), ANGLE(90), ANGLE(48));
		//_delay_ms(100);
		//MoveServo(SERVO_A(4), ANGLE(90), ANGLE(180));
		//_delay_ms(100);
		//MoveServo(SERVO_A(2), ANGLE(90), ANGLE(63));
		//_delay_ms(100);
		//MoveServo(SERVO_A(3), ANGLE(90), ANGLE(-20));
		//_delay_ms(100);
		//MoveServo(SERVO_A(4), ANGLE(180), ANGLE(140));
		//_delay_ms(100);
		//MoveServo(SERVO_A(5), ANGLE(45), ANGLE(15));
		//_delay_ms(100);
		//
//
		///* 들기 */
		//MoveServo(SERVO_A(4), ANGLE(140), ANGLE(180));
		//_delay_ms(100);
		//MoveServo(SERVO_A(2), ANGLE(68), ANGLE(90));
		//_delay_ms(100);
		//MoveServo(SERVO_A(4), ANGLE(180), ANGLE(90));
		//_delay_ms(100);
		//MoveServo(SERVO_A(3), ANGLE(-20), ANGLE(90));
		//_delay_ms(100);


		/* 놓기 */
		//MoveServo(SERVO_A(1), ANGLE(48), ANGLE(136));
		//_delay_ms(100);
		//MoveServo(SERVO_A(2), ANGLE(90), ANGLE(85));
		//_delay_ms(100);
		//MoveServo(SERVO_A(3), ANGLE(90), ANGLE(30));
		//_delay_ms(100);
		//MoveServo(SERVO_A(4), ANGLE(90), ANGLE(150));
		//_delay_ms(100);
		//MoveServo(SERVO_A(3), ANGLE(30), ANGLE(-25));
		//_delay_ms(100);
		//MoveServo(SERVO_A(5), ANGLE(15), ANGLE(45));
		//_delay_ms(100);
		//MoveServo(SERVO_A(2), ANGLE(85), ANGLE(90));
		//_delay_ms(100);
		//MoveServo(SERVO_A(1), ANGLE(135), ANGLE(90));
		//_delay_ms(100);
		
		_delay_ms(50000);
		
		/* 각도 찾기 */
		//pca9685_pwm(SERVO_A(1), ANGLE(49));
		//pca9685_pwm(SERVO_A(2), ANGLE(63));
		//pca9685_pwm(SERVO_A(3), ANGLE(-20));
		//pca9685_pwm(SERVO_A(4), ANGLE(140));
		//pca9685_pwm(SERVO_A(5), ANGLE(45));
		
		LED_PORT &= ~(1 << LED1);
		
	}
	return 0;
}

void INIT_SERVO(){

	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	int i;

	for (i = 1; i <= 4; i++){
		pca9685_pwm(SERVO_A(i), ANGLE(90));
		_delay_ms(20);
	}

	pca9685_pwm(SERVO_A(5), ANGLE(45));
	_delay_ms(200);

}

void MoveServo(uint8_t servo, uint16_t start_angle, uint16_t end_angle) {
	int angle;

	if (start_angle <= end_angle) {
		for (angle = start_angle; angle <= end_angle; ) {
			if (PINB & (1 << switch1)) {
				PORTB &= ~0x02;
				pca9685_pwm(servo, angle);
				//angle += 10;
				angle++;
			}
			else {
				PORTB |= 0x02;
				break;
			}
			_delay_ms(1);
		}
	}
	
	else {
		for (angle = start_angle; angle >= end_angle; ) {
			if (PINB & (1 << switch1)) {
				PORTB &= ~0x02;
				pca9685_pwm(servo, angle);
				//angle -= 10;
				angle--;
			}
			else {
				PORTB |= 0x02;
				break;
			}
			_delay_ms(1);
		}
	}

	_delay_ms(100);
}

void MoveRobotArm(uint8_t servo, uint8_t count) {
	
	int i;
	
	switch (servo){
		case 1:
		PORTB |= (1 << LED2);
		for(i = 0; i < servo_max; i++){
			MoveServo(SERVO_A(move_Aarm_coord[count][i][0]), ANGLE(move_Aarm_coord[count][i][1]), ANGLE(move_Aarm_coord[count][i][2]));
		}
		break;
		//case 2:
		//for(i = 0; i < servo_max; i++){
		//MoveServo(SERVO_B(move_Barm_coord[count][i][0]), ANGLE(move_Barm_coord[count][i][1]), ANGLE(move_Barm_coord[count][i][2]));
		//break;
		//}
	}
	
}