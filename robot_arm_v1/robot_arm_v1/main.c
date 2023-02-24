#define F_CPU 16000000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pca9685.h" // include the PCA9685 library
#include "UART.h"

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

// 스위치
#define switch1 PB2

// servo pin
#define SERVO_A(x) (x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
#define SERVO_B(x) (x + 4)		// SERVO_B(1) = 5, 6, 7, 8, 9

// pulse
#define ANGLE(x) ((10 * x) + 600)		// 600 ~ 2400

#define servo_max 9
#define TERMINATOR '$'

/* 함수 */
// uart
void uart_RasToAt();
// 서보모터 초기화
void INIT_SERVO();
// 서보모터 움직이기
void move_servo(int, int, int);
// 로봇암 쉽게 움직이기
void move_robotarm(int, int);
// 스테핑모터 초기화
void INIT_STEPPER();
// 스테핑모터 돌리기
void loop_stepper();


/* 어쩌고들 */
// 시작할 수 있나?
char WhichCanMove();
// 적외선센서 값 받기
int ReceiveInfrared();


int move_Aarm_coord[9][servo_max][3] = {
	{{1, 90, 0}, {2, 90, 0}, {4, 90, 0}, {5, 90, 30}, {2, 30, 90}, {1, 0, 90}, {1, 90, 180}, {2, 90, 00}, {5, 0, 90}}
};

// uart
int index = 0;				// 수신 버퍼에 저장할 위치
int process_data = 0;		// 문자열 처리
char buffer[20] = "";		// 수신 데이터 버퍼
char buffer_data[20];
char data;					// 수신 데이터


int main(void)
{
	LED_DDR |= (1 << LED1) | (1 << LED2);
	DDRB &= ~(1 << switch1);
	
	char temp;
	int move_num;
	
	// servo motor, stepping motor 초기화
	INIT_SERVO();
	INIT_STEPPER();
	UART_INIT();
	_delay_ms(1000);
	
	//move_servo(SERVO_A(4), ANGLE(90), ANGLE(0));
	
	while (1){
		// LED ON
		PORTB |= (1 << LED1);
		
		int pass = 0;
		
		while (pass == 0) {
			
			/* PART1. 첫 번째 로봇팔 움직임 */
			
			// 로봇팔 움직일 수 있는지 확인
			UART_printString("====First Robot Arm====\n");
			temp = WhichCanMove();
		
			// return으로 stop을 받으면 실행을 종료한다.
			if (strcmp(temp, "stop") == 0) {
				PORTB |= (1 << LED2);
				return 0;
			}
			
			// pass를 받으면 실행
			move_num = (int)temp;
			move_robotarm(move_num + 1, 1);
			
			// 적외선센서
			UART_printString("====First Infrared====\n");
			ReceiveInfrared();
			
			/* PART2. 스테핑모터 움직이기 */
			
			
		}
		
		//// stop을 return받지 않으면 로봇팔 움직임
		//move_num = (int)buffer_data;
		//move_robotarm(move_num + 1, 1);
		
		// LED OFF
		PORTB &= ~(1 << LED1);
	}
	

	return 0;
}

void uart_RasToAt() {
	data = UART_receive();	// 데이터 수신
	if (data != ""){
		if(data == TERMINATOR) {	// 종료 문자를 수신한 경우
			buffer[index] = '\0';
			process_data = 1;		// 수신 문자열 처리 지시
			index = 0;
		}
		else {
			buffer[index] = data;	// 수신 버퍼에 저장
			index++;
		}
	}
}

void INIT_SERVO(){
	
	pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	int i;
	
	for (i = 1; i <= 5; i++){
		pca9685_pwm(SERVO_A(i), ANGLE(90));
	}
}

void move_servo(int servo, int start_angle, int end_angle) {
	int angle;

	if (start_angle <= end_angle) {
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

char WhichCanMove() {
	
	int pass = 0;
	char temp;
	
	while (pass == 0){
		//// 로봇팔 움직일 수 있는지 확인
		//UART_printString("====Fisrt Robot Arm====");
		//UART_printString("\n");
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			// 0 ~ 8의 값이 들어오면
			if ((strcmp(buffer_data, "0") == 0) || (strcmp(buffer_data, "1") == 0) || (strcmp(buffer_data, "2") == 0) || (strcmp(buffer_data, "3") == 0) || (strcmp(buffer_data, "4") == 0) || (strcmp(buffer_data, "5") == 0) || (strcmp(buffer_data, "6") == 0) || (strcmp(buffer_data, "7") == 0) || (strcmp(buffer_data, "8") == 0)){
				UART_printString("pass");
				UART_printString(buffer_data);
				UART_printString("\n");
				pass = 1;
				return buffer_data;
			}
			// 9의 값일 들어오면 (배열이 꽉 찼음)
			else if (strcmp(buffer_data, "9") == 0) {
				UART_printString("end");
				PORTB &= ~(1 << LED1);
				strcpy(temp, "stop");
				return temp;
			}
			// 다른 값이 들어온다면...
			else {
				UART_printString("not a 0 to 9. : ");
				UART_printString(buffer_data);
				UART_printString("\n");
			}
		}
	}
	
	
	return buffer_data;
}

int ReceiveInfrared (void) {
	
	return 0;
}

void move_robotarm(int servo, int count){
	
	int i;
	count -= 1;
	
	switch (servo){
		case 1:
		PORTB |= (1 << LED2);
		for(i = 0; i < servo_max; i++){
			move_servo(SERVO_A(move_Aarm_coord[count][i][0]), ANGLE(move_Aarm_coord[count][i][1]), ANGLE(move_Aarm_coord[count][i][2]));
		}
		break;
		//case 2:
		//for(i = 0; i < servo_max; i++){
		//move_servo(SERVO_B(move_Barm_coord[count][i][0]), ANGLE(move_Barm_coord[count][i][1]), ANGLE(move_Barm_coord[count][i][2]));
		//break;
		//}
	}
	
}

void INIT_STEPPER()
{
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

void loop_stepper()
{
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