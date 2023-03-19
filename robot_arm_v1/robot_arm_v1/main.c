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

#define TERMINATOR '$'

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

void SecondRobotArm();


/*  */
// 시작할 수 있나?
int WhichCanMove(void);
// 적외선센서 값 받기
int ReceiveInfrared(void);
// 컨베이어벨트 움직이고 멈추기
int ConveyorBeltStop(void);


uint16_t move_Aarm_coord[9][servo_max_A][3] = {
	
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
uint16_t move_Barm_fst[servo_max_Bfst][3] = {
	{1, 90, 147}, {4, 90, 180}, {3, 90, -17}, {2, 90, 70}, {4, 180, 120},
	{3, -17, -12},
	{1, 147, 150}, {2, 65, 60}, {4, 120, 130}, {5, 120, 90},
	{4, 130, 180}, {2, 60, 90}, {3, 6, 30}, {3, -12, 20}
};
uint16_t move_Barm_coord[9][servo_max_B][3] = {
	{/* 09 */{1, 147, 67}, {2, 90, 25}, {3, 20, 20}, {4, 180, 130}, {5, 90, 120}, {2, 30, 90}},
	{/* 08 */{1, 147, 90}, {2, 90, 30}, {3, 20, 15}, {4, 180, 130}, {5, 90, 120}, {2, 30, 90}},
	{/* 07 */{1, 147, 105}, {2, 90, 28}, {3, 20, 17}, {4, 180, 130}, {5, 90, 120}, {2, 30, 90}},
	{/* 06 */{1, 147, 65}, {2, 90, 35}, {3, 20, 0}, {4, 180, 130}, {5, 90, 120}, {2, 32, 90}},
	{/* 05 */{1, 147, 90}, {2, 90, 45}, {3, 20, -6}, {4, 180, 130}, {5, 90, 120}, {2, 45, 90}},
	{/* 04 */{1, 147, 110}, {2, 90, 35}, {3, 20, 3}, {4, 180, 130}, {5, 90, 120}, {2, 35, 90}},
	{/* 02 */{1, 147, 90}, {2, 90, 50}, {3, 20, -17}, {4, 180, 130}, {5, 90, 120}, {2, 50, 90}},
	{/* 02 */{1, 147, 90}, {2, 90, 50}, {3, 20, -17}, {4, 180, 130}, {5, 90, 120}, {2, 50, 90}},
	{/* 01 */{1, 147, 110}, {2, 90, 42}, {3, 20, -10}, {4, 180, 150}, {5, 90, 120}, {2, 42, 90}}
};
uint8_t redbox = 0;
uint8_t bluebox = 0;
uint8_t greenbox = 0;

// uart
int uart_index = 0;				// 수신 버퍼에 저장할 위치
int process_data = 0;		// 문자열 처리
char buffer[20] = "";		// 수신 데이터 버퍼
char buffer_data[20];
char data;					// 수신 데이터


int main(void)
{
	LED_DDR |= (1 << LED1) | (1 << LED2);
	
	uint8_t move_num;
	
	// servo motor, stepping motor 초기화
	INIT_SERVO();
	INIT_STEPPER();
	UART_INIT();
	_delay_ms(1000);
	
	while (1){
		// LED ON
		PORTB |= (1 << LED1);
		
		INIT_SERVO();
		_delay_ms(1000);

		uint8_t pass = 0;
		uint8_t part = 1;

		while (pass == 0) {
			
			if (part == 1){
				/* PART1. 첫 번째 로봇팔 움직임 */
				
				// 로봇팔 움직일 수 있는지 확인
					
				UART_printString("fst\n");
				move_num = WhichCanMove();
				
				// return으로 stop을 받으면 실행을 종료한다.
				if (move_num == 9){
					PORTB |= (1 << LED2);
					return 0;
				}
				else if (0 <= move_num && move_num < 9) {
					MoveRobotArm(1, move_num);
					part = 2;
				}
			}
			
			if (part == 2) {
				
				/* PART2. 첫 번째 로봇팔 움직임이 종료된 후 적외선 센서 값 받아오기 */
				
				// 적외선센서 값 받아오기
				//UART_printString("====First Infrared====\n");
				int inf_value;
				inf_value = ReceiveInfrared();
				
				// 적외선 센서값을 받아오지 않으면 (컨베이어 벨트 위에 상자가 놓여있지 않으면) part1으로 이동
				if (inf_value == 1) {
					//UART_printString("GO_CONV\n");
					part = 3;
				}
				else if (inf_value == 0) {
					PORTB |= (1 << LED2);
					//UART_printString("STOP_CONV\n");
					part = 1;
				}
			}
			
			if (part == 3) {
				/* PART3. 컨베이어 벨트 */
				UART_printString("stm\n");

				int conv_move;
				conv_move = ConveyorBeltStop();
				
				if (conv_move == 0)	part = 4;
			}
			
			if (part == 4) {
				/* PART4. 두 번째 로봇팔 움직이기 */
				UART_printString("====Second Robot Arm====\n");
				
				// Raspberry pi의 값(상자 색) 받아오기
				// 상자 색에 맞춰 로봇팔 움직이기
				SecondRobotArm();
				part = 1;
				
			}
		}

		
		// LED OFF
		PORTB &= ~(1 << LED1);
	}

	return 0;
}

void uart_RasToAt() {
	data = UART_receive();	// 데이터 수신
	if (data != ""){
		if(data == TERMINATOR) {	// 종료 문자를 수신한 경우
			buffer[uart_index] = '\0';
			process_data = 1;		// 수신 문자열 처리 지시
			uart_index = 0;
		}
		else {
			buffer[uart_index] = data;	// 수신 버퍼에 저장
			uart_index++;
		}
	}
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

int WhichCanMove(void) {
	
	int pass = 0;
	//char temp[20];
	
	while (pass == 0){
		//// 로봇팔 움직일 수 있는지 확인
		//UART_printString("====Fisrt Robot Arm====");
		//UART_printString("\n");
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			// 0 ~ 8의 값이 들어오면
			if ((strcmp(buffer_data, "0") == 0) || (strcmp(buffer_data, "1") == 0) || (strcmp(buffer_data, "2") == 0) || (strcmp(buffer_data, "3") == 0) || (strcmp(buffer_data, "4") == 0) || (strcmp(buffer_data, "5") == 0) || (strcmp(buffer_data, "6") == 0) || (strcmp(buffer_data, "7") == 0) || (strcmp(buffer_data, "8") == 0)) {
				UART_printString("pass");
				UART_printString(buffer_data);
				UART_printString("\n");
				pass = 1;
				
				if ((strcmp(buffer_data, "0") == 0)) return 0;
				else if ((strcmp(buffer_data, "1") == 0)) return 1;
				else if ((strcmp(buffer_data, "2") == 0)) return 2;
				else if ((strcmp(buffer_data, "3") == 0)) return 3;
				else if ((strcmp(buffer_data, "4") == 0)) return 4;
				else if ((strcmp(buffer_data, "5") == 0)) return 5;
				else if ((strcmp(buffer_data, "6") == 0)) return 6;
				else if ((strcmp(buffer_data, "7") == 0)) return 7;
				else if ((strcmp(buffer_data, "8") == 0)) return 8;
			}
			// 9의 값일 들어오면 (배열이 꽉 찼음)
			else if (strcmp(buffer_data, "9") == 0) {
				UART_printString("end");
				PORTB &= ~(1 << LED1);
				return 9;
			}
			// 다른 값이 들어온다면...
			else {
				UART_printString("not a 0 to 9. : ");
			}
		}
	}
	
	
	return 0;
}

int ReceiveInfrared (void) {
	
	uint8_t pass = 1;
	
	while (pass != 0){
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			// go 값이 들어오면
			if (strcmp(buffer_data, "go") == 0) {
				//UART_printString("conveyor belt move\n");
				PORTB |= ~(1 << LED2);
				return 1;
			}
			
			else if (strcmp(buffer_data, "stp") == 0) {
				//UART_printString("conveyor belt stop\n");
				PORTB &= ~(1 << LED2);
				//strcpy(temp, "conveyor_stp");
				return 0;
			}
			// 다른 값이 들어온다면...
			else {
				UART_printString("test : ");
				//UART_transmit(buffer_data);
				UART_printString("\n");
				return 2;
			}
		}
	}
	
	return 0;
}

int ConveyorBeltStop (void) {
	
	int pass = 1;
	
	while (pass != 0) {
		
		int temp;
		temp = ReceiveInfrared();
		
		if (temp != 0)	loop_stepper();
		else if (temp == 0) return 0;
		
	}
	
	return 0;
}

void SecondRobotArm(void) {
	
	uint8_t pass = 1;
	
	while (pass != 0) {
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			if (strcmp(buffer_data, "red") == 0) {
				MoveRobotArm(2, (3 * redbox)); // 1, 4, 7
				redbox++;
				pass = 0;
				//PORTB |= ~(1 << LED2);
			}
			
			else if (strcmp(buffer_data, "blue") == 0) {
				MoveRobotArm(2, ((3 * bluebox) + 1)); // 2, 5, 8
				bluebox++;
				pass = 0;
				PORTB &= ~(1 << LED2);
			}
			
			else if (strcmp(buffer_data, "green") == 0) {
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
			MoveServo(SERVO_A(move_Aarm_coord[count][i][0]), ANGLE(move_Aarm_coord[count][i][1]), ANGLE(move_Aarm_coord[count][i][2]));
		}
		break;
		case 2:
		for(i = 0; i < servo_max_Bfst; i++){
			MoveServo(SERVO_B(move_Barm_fst[i][0]), ANGLE(move_Barm_fst[i][1]), ANGLE(move_Barm_fst[i][2]));
		}
		for(i = 0; i < servo_max_B; i++){
			MoveServo(SERVO_B(move_Barm_coord[count][i][0]), ANGLE(move_Barm_coord[count][i][1]), ANGLE(move_Barm_coord[count][i][2]));
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