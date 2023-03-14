#define F_CPU 16000000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include "pca9685.h" // include the PCA9685

// LED (상태등)
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1 PB0
#define LED2 PB1

// 스위치
#define switch1 PB2

#define SERVO_A(x) (uint8_t)(x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
#define SERVO_B(x) (uint8_t)(x + 7)		// SERVO_B(1) = 8, 9, 10, 11, 12

// pulse
#define ANGLE(x) (uint16_t)((10 * x) + 600)		// 600 ~ 2400

#define servo_max 18

uint16_t move_Aarm_coord[9][servo_max][3] = {
	{
		/* 01 */
		{1, 90, 93}, {4, 90, 140}, {2, 90, 65}, {3, 90, -23}, {5, 45, 20},			
		{4, 140, 180}, {3, -23, 30}, {2, 65, 85},			
		{1, 92, 135}, {3, 30, -25}, {4, 180, 150}, {5, 20, 45},{4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},	
	
	{
		/* 02 */
		{1, 90, 68}, {4, 90, 180}, {2, 90, 60}, {3, 90, -25}, {4, 180, 160}, {5, 45, 23},
		{4, 160, 180}, {2, 68, 85},		
		{1, 68, 135}, {4, 180, 150}, {5, 23, 45}, {4, 150, 180}, {3, -25, 30}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 03 */
		{1, 90, 48}, {4, 90, 180}, {2, 90, 63}, {3, 90, -20}, {4, 180, 140}, {5, 45, 20},				
		{4, 140, 180}, /*{2, 68, 90},*/ {3, -20, 30},			
		{1, 48, 136}, {2, 68, 85}, {4, 180, 150}, {3, 30, -25}, {5, 20, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 04 */
		{1, 90, 91}, {4, 90, 180}, {2, 90, 50}, {3, 90, -3}, {4, 180, 150}, {5, 45, 23},
		{4, 150, 180}, {2, 50, 85}, {3, -20, 30},
		{1, 92, 136},/* {2, 70, 85},*/ {4, 180, 150}, {3, 30, -25}, {5, 23, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 05 */
		{1, 90, 70}, {4, 90, 180}, {2, 90, 56}, {3, 90, -10}, {4, 180, 140}, {5, 45, 23},
		{4, 140, 180}, {2, 56, 70}, {3, -20, 30},
		{1, 70, 136}, {2, 70, 85}, {4, 180, 150}, {3, 30, -25}, {5, 23, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 06 */
		{1, 90, 52}, {4, 90, 180}, {2, 90, 50}, {3, 90, -2}, {4, 180, 140}, {5, 45, 23},
		{4, 130, 180}, {2, 56, 70}, {3, -20, 30},
		{1, 52, 136}, {2, 70, 85}, {4, 180, 150}, {3, 30, -25}, {5, 23, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 07 */
		{1, 90, 87}, {4, 90, 180}, {2, 90, 46}, {3, 90, 6}, {4, 180, 140}, {5, 45, 23},
		{4, 140, 180}, {3, 6, 30}, {2, 46, 85},
		{1, 87, 135}, {3, 30, 30}, {4, 180, 150}, {3, 30, -25}, {5, 23, 45},{4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 08 */
		{1, 90, 70}, {4, 90, 180}, {2, 90, 46}, {3, 90, 6}, {4, 180, 140}, {5, 45, 23},
		{4, 140, 180}, {2, 46, 84}, {3, 6, 30},
		{1, 71, 136}, {4, 179, 150}, {3, 30, -25}, {5, 23, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	},
	
	{
		/* 09 */
		{1, 90, 52}, {4, 90, 180}, {2, 90, 45}, {3, 90, 10}, {4, 180, 140}, {5, 45, 23},
		{4, 140, 180}, {2, 46, 84}, {3, 6, 30},
		{1, 52, 136}, {4, 179, 150}, {3, 30, -25}, {5, 23, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
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
	
	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	
	//uint16_t angle = 0;
	_delay_ms(1000);

	while (1) {
		LED_PORT |= (1 << LED1);
		
		INIT_SERVO();
		_delay_ms(1000);
		
		//int i;
		//for ( i = 0; i < 9; i++ ) {
			//MoveRobotArm(1, i);
			//_delay_ms(2000);
			//
			//INIT_SERVO();
		//}
		
		/* 함수 이용하기 */
		//INIT_SERVO();
		//_delay_ms(1000);
		//MoveRobotArm(1, 7);
		//_delay_ms(50000000000000);
		
		
		/* 집기 */
		MoveServo(SERVO_B(1), ANGLE(90), ANGLE(150));
		_delay_ms(100);
		MoveServo(SERVO_B(4), ANGLE(90), ANGLE(180));
		_delay_ms(100);
		MoveServo(SERVO_B(3), ANGLE(90), ANGLE(-12));
		_delay_ms(100);
		MoveServo(SERVO_B(2), ANGLE(90), ANGLE(59));
		_delay_ms(100);
		MoveServo(SERVO_B(4), ANGLE(180), ANGLE(130));
		_delay_ms(100);
		MoveServo(SERVO_B(5), ANGLE(90), ANGLE(55));
		_delay_ms(100);

		/* 들기 */
		MoveServo(SERVO_B(4), ANGLE(140), ANGLE(180));
		_delay_ms(100);
		MoveServo(SERVO_B(2), ANGLE(46), ANGLE(84));
		_delay_ms(100);
		MoveServo(SERVO_B(3), ANGLE(6), ANGLE(30));
		_delay_ms(100);


		/* 놓기 */
		MoveServo(SERVO_B(1), ANGLE(150), ANGLE(90));
		_delay_ms(1000);
		MoveServo(SERVO_B(4), ANGLE(180), ANGLE(150));
		_delay_ms(100);
		MoveServo(SERVO_B(3), ANGLE(30), ANGLE(-25));
		_delay_ms(100);
		MoveServo(SERVO_B(5), ANGLE(55), ANGLE(90));
		_delay_ms(100);
		//MoveServo(SERVO_B(2), ANGLE(85), ANGLE(90));
		//_delay_ms(100);
		//MoveServo(SERVO_B(1), ANGLE(135), ANGLE(90));
		//_delay_ms(100);
		
		_delay_ms(50000000000000);		
		// {1, 90, 70}, {4, 90, 180}, {2, 90, 46}, {3, 90, 6}, {4, 180, 140}, {5, 45, 23},
		
		/* 각도 찾기 */
		//pca9685_pwm(SERVO_B(1), ANGLE(150));
		//pca9685_pwm(SERVO_B(2), ANGLE(57));
		//pca9685_pwm(SERVO_B(3), ANGLE(-10));
		//pca9685_pwm(SERVO_B(4), ANGLE(130));
		//pca9685_pwm(SERVO_B(5), ANGLE(90));
		
		LED_PORT &= ~(1 << LED1);
		
	}
	return 0;
}

void INIT_SERVO(){     

	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	int i;

	for (i = 1; i <= 4; i++){
		pca9685_pwm(SERVO_A(i), ANGLE(90));
		pca9685_pwm(SERVO_B(i), ANGLE(90));
		_delay_ms(20);
	}

	pca9685_pwm(SERVO_A(5), ANGLE(45));
	pca9685_pwm(SERVO_B(3), ANGLE(120));
	pca9685_pwm(SERVO_B(5), ANGLE(90));
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