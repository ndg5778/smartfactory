#define F_CPU 16000000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ServoDriver/pca9685.h" // include the PCA9685 library
#include "UART/UART.h"

// 스테핑모터
#define STEPPING_A_DDR DDRD
#define STEPPING_A PORTD
#define STEPPING_B_DDR DDRC
#define STEPPING_B PORTC

#define STEPPING_A_IN1 PD2
#define STEPPING_A_IN2 PD3
#define STEPPING_A_IN3 PD4
#define STEPPING_A_IN4 PD5

#define STEPPING_B_IN1 PC0
#define STEPPING_B_IN2 PC1
#define STEPPING_B_IN3 PC2
#define STEPPING_B_IN4 PC3

#define motor_time 20000

// LED (상태등)
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1 PB0
#define LED2 PB1

// servo pin
#define SERVO_A(x) (uint8_t)(x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
#define SERVO_B(x) (uint8_t)(x + 7)		// SERVO_B(1) = 8, 9, 10, 11, 12

// pulse
#define ANGLE(x) (uint16_t)((10 * x) + 600)		// 600 ~ 2400

#define servo_max_A 18
#define servo_max_Bfst 15
#define servo_max_B 7

/* 함수 */
// uart
void uart_RasToAt();
// 서보모터 초기화
void INIT_SERVO();
// 서보모터 움직이기
void MoveServo(uint8_t, uint16_t, uint16_t);
// 로봇암 쉽게 움직이기
void MoveRobotArm(uint8_t, uint8_t);
// 스테핑모터 초기화
void INIT_STEPPER();
// 스테핑모터 돌리기
void loop_stepper();


/*  */
// 시작할 수 있나?
int WhichCanMove(void);
// second robot arm
void SecondRobotArm();


uint16_t move_Aarm_coord[9][servo_max_A][2] = {
	
	{
		/* 01 */
		{1, 83}, {4, 135}, {2, 60}, {3, -10}, {4, 110}, {5, 30},
		{4, 180}, {3, 30}, {2, 77},
		{1, 125}, {3, -15}, {4, 130}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	},
	
	{
		/* 02 */
		{1, 64}, {4, 180}, {2, 60}, {3, -15}, {4, 120}, {5, 10},
		{4, 150}, {2, 77},
		{1, 125},  {4, 130}, {5, 45}, {4, 180}, {3, 30}, {2, 90}, {1, 90}
	},
	
	{
		/* 03 */
		{1, 44.5}, {4, 180}, {2, 60}, {3, -10}, {4, 120}, {5, 10},
		{4, 150}, /*{2, 90},*/ {3, 30},
		{1, 125}, {2, 77}, {4, 130}, {3, -15}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	},
	
	{
		/* 04 */
		{1, 82}, {4, 180}, {2, 52}, {3, 8}, {4, 100}, {5, 10},
		{4, 150}, {2, 77}, {3, 30},
		{1, 125}, {4, 130}, {3, -15}, {5, 50}, {4, 180}, {2, 90}, {1, 90}
	},
	
	{
		/* 05 */
		{1, 64}, {4, 180}, {2, 52}, {3, 0}, {4, 130}, {5, 10},
		{4, 180}, {2, 77}, {3, 30},
		{1, 125}, {4, 130}, {3, -15}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	},
	
	{
		/* 06 */
		{1, 46}, {4, 180}, {2, 50}, {3, 5}, {4, 120}, {5, 0},
		{4, 150}, {2, 77}, {3, 30},
		{1, 125}, {4, 130}, {3, -15}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	},
	
	{
		/* 07 */
		{1, 80}, {4, 180}, {2, 45}, {3, 20}, {4, 102}, {5, 10},
		{4, 150}, {3, 30}, {2, 77},
		{1, 126}, {3, -15}, {4, 130}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	},
	
	
	{
		/* 08 */
		{1, 64}, {4, 180}, {2, 45}, {3, 17}, {4, 100}, {5, 10},
		{4, 150}, {2, 77}, {3, 30},
		{1, 125}, {4, 130}, {3, -15}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	},
	
	{
		/* 09 */
		{1, 47}, {4, 180}, {2, 38}, {3, 22}, {4, 125}, {5, 10},
		{4, 150}, {2, 77}, {3, 30},
		{1, 125}, {4, 130}, {3, -15}, {5, 45}, {4, 180}, {2, 90}, {1, 90}
	}
};
uint16_t move_Barm_fst[servo_max_Bfst][2] = {
	{1, 136}, {4, 180}, {2, 90}, {3, 0},  {4, 110}, {5, 80}, {5, 80},
		{4, 180}, {3, 30}, {2, 90}, {1, 70}
};
uint16_t move_Barm_coord[9][servo_max_B][2] = {
	{/* 09 */{1, 67}, {2, 25}, {3, 20}, {4, 130}, {5, 120}, {2, 90}},
	{/* 08 */{1, 90}, {2, 30}, {3, 15}, {4, 130}, {5, 120}, {2, 90}},
	{/* 07 */{1, 105}, {2, 28}, {3, 17}, {4, 130}, {5, 120}, {2, 90}},
	{/* 06 */{1, 65}, {2, 35}, {3, 0}, {4, 130}, {5, 120}, {2, 90}},
	{/* 05 */{1, 90}, {2, 45}, {3, -6}, {4, 130}, {5, 120}, {2, 90}},
	{/* 04 */{1, 110}, {2, 35}, {3, 3}, {4, 130}, {5, 120}, {2, 90}},
	{/* 02 */{1, 90}, {2, 50}, {3, -17}, {4, 130}, {5, 120}, {2, 90}},
	{/* 02 */{1, 90}, {2, 50}, {3, -17}, {4, 130}, {5, 120}, {2, 90}},
	{/* 01 */{1, 110}, {2, 42}, {3, -10}, {4, 150}, {5, 120}, {2, 90}}
};
uint8_t redbox	= 0;
uint8_t bluebox	= 0;
uint8_t greenbox= 0;

// uart
int uart_index		= 0;			// 수신 버퍼에 저장할 위치
int process_data	= 0;		// 문자열 처리
char buffer[20]		= "";		// 수신 데이터 버퍼
char data;					// 수신 데이터


 int main(void)
 {
 	uint8_t move_num;
 	
 	// servo motor, stepping motor 초기화
 	INIT_SERVO();
 	INIT_STEPPER();
 	UART_INIT();
 	_delay_ms(1000);
 	
 	while (1) {

 		uart_RasToAt();
 		
 		if(strcmp(buffer, "s") == 0) {
 			move_num = WhichCanMove();
 			if (move_num == 9)	return 0;
 			else if (0 <= move_num && move_num < 9) {
				MoveRobotArm(1, move_num);
				_delay_ms(1000);
				INIT_SERVO();
			}
 		}
 		else if(strcmp(buffer, "c") == 0) {
 			while (1) {
 				loop_stepper();
				 
 				uart_RasToAt();
 				if(strcmp(buffer, "t") == 0) {
					 MoveRobotArm(2, 0);
					 break;
				 }
 			}
 		}
 		else {
 			
 			UART_transmit(data);
 			uart_index = 0;
 		}
 	}
 
 	return 0;
 }


void uart_RasToAt() {

	data = UART_receive();	// 데이터 수신
	buffer[uart_index] = data;
		
}

void INIT_SERVO(){

	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	int i;

	for (i = 1; i <= 4; i++){
		if(i == 3) {
			pca9685_pwm(SERVO_A(i), ANGLE(110));
			pca9685_pwm(SERVO_B(i), ANGLE(120));
			_delay_ms(20);
		}
		else {
			pca9685_pwm(SERVO_A(i), ANGLE(90));
			pca9685_pwm(SERVO_B(i), ANGLE(90));
			_delay_ms(20);
		}
	}

	pca9685_pwm(SERVO_A(5), ANGLE(45));
	pca9685_pwm(SERVO_B(5), ANGLE(120));
	_delay_ms(200);


}

int WhichCanMove(void) {
	
	int pass = 0;
	
	while (pass == 0){
		
		uart_RasToAt();
			
		// 0 ~ 8의 값이 들어오면
		if ((strcmp(buffer, "0") == 0) || (strcmp(buffer, "1") == 0) || (strcmp(buffer, "2") == 0) || (strcmp(buffer, "3") == 0) || (strcmp(buffer, "4") == 0) || (strcmp(buffer, "5") == 0) || (strcmp(buffer, "6") == 0) || (strcmp(buffer, "7") == 0) || (strcmp(buffer, "8") == 0) || (strcmp(buffer, "9") == 0)) {
			//UART_printString(buffer);
			pass = 1;
				
			if ((strcmp(buffer, "0") == 0)) return 0;
			else if ((strcmp(buffer, "1") == 0)) return 1;
			else if ((strcmp(buffer, "2") == 0)) return 2;
			else if ((strcmp(buffer, "3") == 0)) return 3;
			else if ((strcmp(buffer, "4") == 0)) return 4;
			else if ((strcmp(buffer, "5") == 0)) return 5;
			else if ((strcmp(buffer, "6") == 0)) return 6;
			else if ((strcmp(buffer, "7") == 0)) return 7;
			else if ((strcmp(buffer, "8") == 0)) return 8;
			else if ((strcmp(buffer, "9") == 0)) return 9;
		}
		// 다른 값이 들어온다면...
		else {
			continue;
		}	
	}
	return 0;
}


void SecondRobotArm(void) {
	
	uint8_t pass = 1;
	
	while (pass != 0) {
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer, buffer);
			
			if (strcmp(buffer, "r") == 0) {
				MoveRobotArm(2, (3 * redbox)); // 1, 4, 7
				redbox++;
				pass = 0;
			}
			
			else if (strcmp(buffer, "b") == 0) {
				MoveRobotArm(2, ((3 * bluebox) + 1)); // 2, 5, 8
				bluebox++;
				pass = 0;
				PORTB &= ~(1 << LED2);
			}
			
			else if (strcmp(buffer, "g") == 0) {
				MoveRobotArm(2, ((3 * greenbox) + 2)); // 3, 6, 9
				greenbox++;
				pass = 0;
				PORTB &= ~(1 << LED2);
			}
		}
		
	}
}

void MoveServo (uint8_t servo, uint16_t start_angle, uint16_t end_angle) {
	int angle;

	if (start_angle <= end_angle) {
		for (angle = start_angle; angle <= end_angle; angle++) {
			pca9685_pwm(servo, angle);
			_delay_ms(1);
		}
	}
	
	else {
		for (angle = start_angle; angle >= end_angle; angle--) {
			pca9685_pwm(servo, angle);
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
		for(i = 0; i < servo_max_A; i++){
			pca9685_pwm(SERVO_A(move_Aarm_coord[count][i][0]), ANGLE(move_Aarm_coord[count][i][1]));
			_delay_ms(1000);
		}
		break;
		case 2:
		for(i = 0; i < servo_max_Bfst; i++){
			pca9685_pwm(SERVO_B(move_Barm_fst[i][0]), ANGLE(move_Barm_fst[i][1]));
			_delay_ms(1000);
		}
		for(i = 0; i < servo_max_B; i++){
			pca9685_pwm(SERVO_B(move_Barm_coord[count][i][0]), ANGLE(move_Barm_coord[count][i][1]));
			_delay_ms(1000);
		}
		break;
	}
	
}

void INIT_STEPPER() {
	
	// 타이머/카운터 2번을 고속 PWM 모드로 설정
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2A |= (1 << COM2A1);		// 비반전 모드
	TCCR2B |= (1 << CS22);			// 분주비 64
	
	
	// stepping motor
	STEPPING_A_DDR |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN2 ) | (1 << STEPPING_A_IN3) | (1 << STEPPING_A_IN4);
	STEPPING_B_DDR |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN2 ) | (1 << STEPPING_B_IN3) | (1 << STEPPING_B_IN4);
	
	// led
	LED_DDR |= (1 << LED1) | (1 << LED2);	// led 상태등 표시 PB0
}

void loop_stepper() {
	
	STEPPING_A &= ~((1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN4));		// low
	STEPPING_A |= (1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN3);		// high
	STEPPING_B &= ~((1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN4));
	STEPPING_B |= (1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN3);
	_delay_us(motor_time);
	

	STEPPING_A &= ~((1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN3));
	STEPPING_A |= (1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN4);
	STEPPING_B &= ~((1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN3));
	STEPPING_B |= (1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN4);
	_delay_us(motor_time);
	
	STEPPING_A &= ~((1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN3));
	STEPPING_A |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN4);
	STEPPING_B &= ~((1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN3));
	STEPPING_B |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN4);
	_delay_us(motor_time);

	STEPPING_A &= ~((1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN4));
	STEPPING_A |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN3);
	STEPPING_B &= ~((1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN4));
	STEPPING_B |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN3);
	_delay_us(motor_time);
}
