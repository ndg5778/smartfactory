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
#define SERVO_A(x) (uint8_t)(x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
//#define SERVO_A(5) (uint8_t)5
#define SERVO_B(x) (uint8_t)(x + 4)		// SERVO_B(1) = 5, 6, 7, 8, 9

// pulse
#define ANGLE(x) (uint16_t)((10 * x) + 600)		// 600 ~ 2400

#define servo_max 10
#define TERMINATOR '$'

/* �Լ� */
// uart
void uart_RasToAt();
// �������� �ʱ�ȭ
void INIT_SERVO();
// �������� �����̱�
void MoveServo(uint8_t, uint16_t, uint16_t);
// �κ��� ���� �����̱�
void MoveRobotArm(uint8_t, uint8_t);
// �����θ��� �ʱ�ȭ
void INIT_STEPPER();
// �����θ��� ������
void loop_stepper();


/*  */
// ������ �� �ֳ�?
int WhichCanMove();
// ���ܼ����� �� �ޱ�
int ReceiveInfrared();
// �����̾Ʈ �����̰� ���߱�
int ConveyorBeltStop();


uint16_t move_Aarm_coord[9][servo_max][3] = {
	{
		{1, 90, 120}, {1, 120, 90}, {2, 90, 120}, {2, 120, 90}, {3, 90, 120}, {3, 120, 90}, {4, 90, 180}, {4, 180, 90}, {5, 45, 0}, {5, 0, 45}
	}
};

// uart
int uart_index = 0;				// ���� ���ۿ� ������ ��ġ
int process_data = 0;		// ���ڿ� ó��
char buffer[20] = "";		// ���� ������ ����
char buffer_data[20];
char data;					// ���� ������


int main(void)
{
	LED_DDR |= (1 << LED1) | (1 << LED2);
	DDRB &= ~(1 << switch1);
	
	//char* temp;
	int move_num;
	
	// servo motor, stepping motor �ʱ�ȭ
	INIT_SERVO();
	INIT_STEPPER();
	UART_INIT();
	_delay_ms(1000);
	
	//MoveRobotArm(1, 0);
	
	while (1){
		// LED ON
		PORTB |= (1 << LED1);
		
		INIT_SERVO();
		_delay_ms(1000);

		int pass = 0;
		int part = 1;
		int uart_right = 0;
		
		while (pass == 0) {
			
			if (part == 1){
				/* PART1. ù ��° �κ��� ������ */
				
				// �κ��� ������ �� �ִ��� Ȯ��
				uart_right = 0;
					
				UART_printString("====First Robot Arm====\n");
				move_num = WhichCanMove();
				
				// return���� stop�� ������ ������ �����Ѵ�.
				if (move_num == 9){
					PORTB |= (1 << LED2);
					return 0;
				}
				else if (0 <= move_num && move_num < 9) {
					//UART_printString("YES!!!!!!!!!!\n");
					MoveRobotArm(1, move_num);
					part = 2;
				}
			}
			
			if (part == 2) {
				
				/* PART2. ù ��° �κ��� �������� ����� �� ���ܼ� ���� �� �޾ƿ��� */
				
				// ���ܼ����� �� �޾ƿ���
				UART_printString("====First Infrared====\n");
				int inf_value;
				inf_value = ReceiveInfrared();
				
				// ���ܼ� �������� �޾ƿ��� ������ (�����̾� ��Ʈ ���� ���ڰ� �������� ������) part1���� �̵�
				if (inf_value == 1) {
					UART_printString("GO_CONV\n");
					part = 3;
				}
				else if (inf_value == 0) {
					PORTB |= (1 << LED2);
					UART_printString("STOP_CONV\n");
					part = 1;
				}
			}
			
			if (part == 3) {
				/* PART3. �����̾� ��Ʈ */
				UART_printString("====stepping motor====\n");

				int conv_move;
				conv_move = ConveyorBeltStop();
				
				if (conv_move == 0)	part = 4;
			}
			
			if (part == 4) {
				/* PART4. �� ��° �κ��� �����̱� */
				UART_printString("====Second Robot Arm====\n");
				
				// Raspberry pi�� ��(���� ��) �޾ƿ���
				// ���� ���� ���� �κ��� �����̱�
			}
		}
		
		////// stop�� return���� ������ �κ��� ������
		////move_num = (int)buffer_data;
		////MoveRobotArm(move_num + 1, 1);
		
		// LED OFF
		PORTB &= ~(1 << LED1);
	}
	

	return 0;
}

void uart_RasToAt() {
	data = UART_receive();	// ������ ����
	if (data != ""){
		if(data == TERMINATOR) {	// ���� ���ڸ� ������ ���
			buffer[uart_index] = '\0';
			process_data = 1;		// ���� ���ڿ� ó�� ����
			uart_index = 0;
		}
		else {
			buffer[uart_index] = data;	// ���� ���ۿ� ����
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



int WhichCanMove() {
	
	int pass = 0;
	//char temp[20];
	
	while (pass == 0){
		//// �κ��� ������ �� �ִ��� Ȯ��
		//UART_printString("====Fisrt Robot Arm====");
		//UART_printString("\n");
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			// 0 ~ 8�� ���� ������
			if ((strcmp(buffer_data, "0") == 0) || (strcmp(buffer_data, "1") == 0) || (strcmp(buffer_data, "2") == 0) || (strcmp(buffer_data, "3") == 0) || (strcmp(buffer_data, "4") == 0) || (strcmp(buffer_data, "5") == 0) || (strcmp(buffer_data, "6") == 0) || (strcmp(buffer_data, "7") == 0) || (strcmp(buffer_data, "8") == 0)){
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
			// 9�� ���� ������ (�迭�� �� á��)
			else if (strcmp(buffer_data, "9") == 0) {
				UART_printString("end");
				PORTB &= ~(1 << LED1);
				//strcpy(temp, "fisrt_robot_stop");
				return 9;
			}
			// �ٸ� ���� ���´ٸ�...
			else {
				UART_printString("not a 0 to 9. : ");
				
				UART_transmit(buffer_data);
				UART_printString("\n");
			}
		}
	}
	
	
	//return buffer_data;
}

int ReceiveInfrared (void) {
	
	int pass = 1;
	//char temp[20];
	
	while (pass != 0){
		
		uart_RasToAt();
		
		if(process_data == 1){
			strcpy(buffer_data, buffer);
			
			// go ���� ������
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
			// �ٸ� ���� ���´ٸ�...
			else {
				UART_printString("test : ");
				UART_transmit(buffer_data);
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
		
		//uart_RasToAt();
		//
		//if(process_data == 1){		// ���ڿ� ó��
		//strcpy(buffer_data, buffer);
		//if((strcmp(buffer_data, "mov") != 0) && (strcmp(buffer_data, "stp") != 0)){
		//UART_printString("** Unknown Command **\t: ");
		//UART_printString(buffer);
		//UART_printString("\n");
		//}
		//else if (strcmp(buffer_data, "mov") == 0) {
		//UART_printString("MOVE!");
		//UART_printString("\n");
		//loop_stepper();
		//
		//while(!(strcmp(buffer_data, "stp") == 0)){
		//UART_printString("STOP!!");
		//UART_printString("\n");
		//uart_RasToAt();
		//}
		//}
		//uart_index = 0;
		//process_data = 0;
		//}
	}
}

void MoveServo(uint8_t servo, uint16_t start_angle, uint16_t end_angle) {
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

void MoveRobotArm(uint8_t servo, uint8_t count){
	
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