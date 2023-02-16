#define F_CPU 16000000L
#include <avr/io.h> // include the AVR IO library
#include <util/delay.h> // include the delay library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pca9685.h" // include the PCA9685 library
#include "UART.h"

// �����θ���
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

// LED (���µ�)
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1 PB0
#define LED2 PB1

// ����ġ
#define switch1 PB2

// servo pin
#define SERVO_A(x) (x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
#define SERVO_B(x) (x + 4)		// SERVO_B(1) = 5, 6, 7, 8, 9

// pulse
#define ANGLE(x) ((10 * x) + 600)		// 600 ~ 2400

#define servo_max 9
#define TERMINATOR '$'

/* �Լ� */
// uart
void uart_RasToAt();
// �������� �ʱ�ȭ
void INIT_SERVO();
// �������� �����̱�
void move_servo(int, int, int);
// �κ��� ���� �����̱�
void move_robotarm(int, int);
// �����θ��� �ʱ�ȭ
void INIT_STEPPER();
// �����θ��� ������
void loop_stepper();


/* ��¼��� */
// �������� �� �ֳ�?
char uart_canmove();


int move_Aarm_coord[9][servo_max][3] = {
	{{1, 90, 0}, {2, 90, 0}, {4, 90, 0}, {5, 90, 30}, {2, 30, 90}, {1, 0, 90}, {1, 90, 180}, {2, 90, 00}, {5, 0, 90}}
};

// uart
int index = 0;				// ���� ���ۿ� ������ ��ġ
int process_data = 0;		// ���ڿ� ó��
char buffer[20] = "";		// ���� ������ ����
char buffer_data[20];
char data;					// ���� ������


int main(void)
{
	LED_DDR |= (1 << LED1) | (1 << LED2);
	DDRB &= ~(1 << switch1);
	
	int temp;
	int move_num;
	
	// servo motor, stepping motor �ʱ�ȭ
	INIT_SERVO();
	INIT_STEPPER();
	UART_INIT();
	_delay_ms(1000);
	
	//move_servo(SERVO_A(4), ANGLE(90), ANGLE(0));
	
	while (1){
		// LED ON
		PORTB |= (1 << LED1);
		
		// uart�� �κ��� ��� ������������ Ȯ��
		//temp = uart_canmove();
		int pass = 0;
		
		while (pass == 0){
			// �κ��� ������ �� �ִ��� Ȯ��
			UART_printString("====First Robot Arm====");
			UART_printString("\n");
			
			UART_printString("");
			uart_RasToAt();
			
			if(process_data == 1){
				strcpy(buffer_data, buffer);
				
				// 0 ~ 8�� ���� ������
				if ((strcmp(buffer, "0") == 0) || (strcmp(buffer_data, "1") == 0) || (strcmp(buffer_data, "2") == 0) || (strcmp(buffer_data, "3") == 0) || (strcmp(buffer_data, "4") == 0) || (strcmp(buffer_data, "5") == 0) || (strcmp(buffer_data, "6") == 0) || (strcmp(buffer_data, "7") == 0) || (strcmp(buffer_data, "8") == 0)){
					UART_printString("pass");
					UART_printString("\n");
					//UART_transmit(buffer_data);
					pass = 1;
				}
				// 9�� ���� ������ (�迭�� �� á��)
				else if (strcmp(buffer, "9") == 0) {
					UART_printString("end");
					UART_printString(buffer);
					UART_printString("\n");
					PORTB &= ~(1 << LED1);
					return 0;
				}
				// �ٸ� ���� ���´ٸ�...
				else {
					UART_printString("It's not a str from 0 to 9. : ");
					UART_printString(buffer);
					UART_printString("\n");
				}
				UART_printString("test : ");
				UART_printString(buffer);
				UART_printString("\n");
			}
		}
		
		//// return���� stop�� ������ ������ �����Ѵ�.
		//if (temp == "stop") {
			//PORTB &= ~(1 << LED2);
			//return 0;
		//}
		
		
		// stop�� return���� ������ �κ��� ������
		move_num = (int)buffer_data;
		move_robotarm(move_num + 1, 1);
		
		//if (!(PINB & (1 << switch1))) {
			//_delay_ms(500);
			//PORTB |= (1 << LED2);
			//
			//move_robotarm(1, 1);		// 1��° �κ����� 1��° �迭�� ���� �κ��� �����̱�
			//_delay_ms(1000);
			//INIT_SERVO();
			//
			//// ��ȣ�� ���� ������ �����θ��� �����̱�
			//while (PINB & (1 << switch1)) {
				//loop_stepper();
			//}
		//}
		//else {
			//PORTB &= ~(1 << LED2);
		//}
		
		// LED OFF
		PORTB &= ~(1 << LED1);
	}
	

	return 0;
}

void uart_RasToAt(){
	data = UART_receive();	// ������ ����
	if (data != ""){
		if(data == TERMINATOR) {	// ���� ���ڸ� ������ ���
			buffer[index] = '\0';
			process_data = 1;		// ���� ���ڿ� ó�� ����
			index = 0;
		}
		else {
			buffer[index] = data;	// ���� ���ۿ� ����
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

char uart_canmove() {
	
	int pass = 0;
	
	while (pass == 0){
		// �κ��� ������ �� �ִ��� Ȯ��
		UART_printString("====Fisrt Robot Arm====");
		UART_printString("\n");
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			// 0 ~ 8�� ���� ������
			if ((strcmp(buffer_data, "0") == 0) || (strcmp(buffer_data, "1") == 0) || (strcmp(buffer_data, "2") == 0) || (strcmp(buffer_data, "3") == 0) || (strcmp(buffer_data, "4") == 0) || (strcmp(buffer_data, "5") == 0) || (strcmp(buffer_data, "6") == 0) || (strcmp(buffer_data, "7") == 0) || (strcmp(buffer_data, "8") == 0)){
				UART_printString("pass");
				//UART_transmit(buffer_data);
				UART_printString("\n");
				pass = 1;
				return buffer_data;
			}
			// 9�� ���� ������ (�迭�� �� á��)
			else if (strcmp(buffer_data, "9") == 0) {
				UART_printString("end");
				PORTB &= ~(1 << LED1);
				return "stop";
			}
			// �ٸ� ���� ���´ٸ�...
			else {
				UART_printString("It's not a str from 0 to 9.");
			}
		}
	}
	
	
	return buffer_data;
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
	// Ÿ�̸�/ī���� 2���� ��� PWM ���� ����
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2A |= (1 << COM2A1);		// ����� ���
	TCCR2B |= (1 << CS22);			// ���ֺ� 64
	
	
	// stepping motor
	STEPPING_A_DDR |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN2 ) | (1 << STEPPING_A_IN3) | (1 << STEPPING_A_IN4);
	STEPPING_B_DDR |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN2 ) | (1 << STEPPING_B_IN3) | (1 << STEPPING_B_IN4);
	
	// led
	LED_DDR |= (1 << LED1) | (1 << LED2);	// led ���µ� ǥ�� PB0
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