//#define F_CPU 16000000L
//#include <avr/io.h> // include the AVR IO library
//#include <util/delay.h> // include the delay library
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "pca9685.h" // include the PCA9685 library
//#include "UART.h"
//
//// 스테핑모터
//#define STEPPING_A_DDR DDRD
//#define STEPPING_A PORTD
//#define STEPPING_B_DDR DDRC
//#define STEPPING_B PORTC
//
//#define STEPPING_A_IN1 PD2
//#define STEPPING_A_IN2 PD3
//#define STEPPING_A_IN3 PD4
//#define STEPPING_A_IN4 PD5
//
//#define STEPPING_B_IN1 PC0
//#define STEPPING_B_IN2 PC1
//#define STEPPING_B_IN3 PC2
//#define STEPPING_B_IN4 PC3
//
//#define motor_time 20000
//
//// LED (상태등)
//#define LED_DDR DDRB
//#define LED_PORT PORTB
//#define LED1 PB0
//#define LED2 PB1
//
//// 스위치
//#define switch1 PB2
//
//// servo pin
//#define SERVO_A(x) (x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
//#define SERVO_B(x) (x + 4)		// SERVO_B(1) = 5, 6, 7, 8, 9
//
//// pulse
//#define ANGLE(x) ((10 * x) + 600)		// 600 ~ 2400
//
//#define servo_max 9
//
//
//// 서보모터 초기화
//void INIT_SERVO();
//// 서보모터 움직이기
//void move_servo(int, int, int);
//// 로봇암 쉽게 움직이기
//void move_robotarm(int, int);
//// 스테핑모터 초기화
//void INIT_STEPPER();
//// 스테핑모터 돌리기
//void loop_stepper();
//
//
//int move_Aarm_coord[9][servo_max][3] = {
	//{{1, 90, 0}, {2, 90, 0}, {4, 90, 0}, {5, 90, 30}, {2, 30, 90}, {1, 0, 90}, {1, 90, 180}, {2, 90, 00}, {5, 0, 90}}
//};
//
//
//int main(void)
//{
	//LED_DDR |= (1 << LED1) | (1 << LED2);
	//DDRB &= ~(1 << switch1);
	//
	//// servo motor, stepping motor 초기화
	//INIT_SERVO();
	//INIT_STEPPER();
	//_delay_ms(1000);
	//
	////move_servo(SERVO_A(4), ANGLE(90), ANGLE(0));
	//
	//while (1){
		//// LED ON
		//PORTB |= (1 << LED1);
		//
		//if (!(PINB & (1 << switch1))) {
			//_delay_ms(500);
			//PORTB |= (1 << LED2);
			//
			//move_robotarm(1, 1);		// 1번째 로봇팔을 1번째 배열에 맞춰 로봇팔 움직이기
			//_delay_ms(1000);
			//INIT_SERVO();
			//
			//// 신호를 받을 때까지 스테핑모터 움직이기
			//while (PINB & (1 << switch1)) {
				//loop_stepper();
			//}
		//}
		//else {
			//PORTB &= ~(1 << LED2);
		//}
		//
		//// LED OFF
		//PORTB &= ~(1 << LED1);
	//}
	//INIT_SERVO();
//
	//return 0;
//}
//
//void INIT_SERVO(){
	//
	//pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	//int i;
	//
	//for (i = 1; i <= 5; i++){
		//pca9685_pwm(SERVO_A(i), ANGLE(90));
	//}
//}
//
//void move_servo(int servo, int start_angle, int end_angle){
	//int angle;
//
	//if (start_angle <= end_angle){
		//for (angle = start_angle; angle <= end_angle; ) {
			//if (PINB & (1 << switch1)) {
				//PORTB &= ~0x02;
				//pca9685_pwm(servo, angle);
				//angle += 10;
			//}
			//else {
				//PORTB |= 0x02;
				//break;
			//}
			//_delay_ms(15);
		//}
	//}
	//
	//else {
		//for (angle = start_angle; angle >= end_angle; ) {
			//if (PINB & (1 << switch1)) {
				//PORTB &= ~0x02;
				//pca9685_pwm(servo, angle);
				//angle -= 10;
			//}
			//else {
				//PORTB |= 0x02;
				//break;
			//}
			//_delay_ms(15);
		//}
	//}
//}
//
//void move_robotarm(int servo, int count){
	//
	//int i;
	//count -= 1;
	//
	//switch (servo){
		//case 1:
		//PORTB |= (1 << LED2);
		//for(i = 0; i < servo_max; i++){
			//move_servo(SERVO_A(move_Aarm_coord[count][i][0]), ANGLE(move_Aarm_coord[count][i][1]), ANGLE(move_Aarm_coord[count][i][2]));
		//}
		//break;
		////case 2:
		////for(i = 0; i < servo_max; i++){
		////move_servo(SERVO_B(move_Barm_coord[count][i][0]), ANGLE(move_Barm_coord[count][i][1]), ANGLE(move_Barm_coord[count][i][2]));
		////break;
		////}
	//}
	//
//}
//
//void INIT_STEPPER()
//{
	//// 타이머/카운터 2번을 고속 PWM 모드로 설정
	//TCCR2A |= (1 << WGM21) | (1 << WGM20);
	//TCCR2A |= (1 << COM2A1);		// 비반전 모드
	//TCCR2B |= (1 << CS22);			// 분주비 64
	//
	//
	//// stepping motor
	//STEPPING_A_DDR |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN2 ) | (1 << STEPPING_A_IN3) | (1 << STEPPING_A_IN4);
	//STEPPING_B_DDR |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN2 ) | (1 << STEPPING_B_IN3) | (1 << STEPPING_B_IN4);
	//
	//// led
	//LED_DDR |= (1 << LED1) | (1 << LED2);	// led 상태등 표시 PB0
//}
//
//void loop_stepper()
//{
	//STEPPING_A &= ~((1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN4));		// low
	//STEPPING_A |= (1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN3);		// high
	//STEPPING_B &= ~((1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN4));
	//STEPPING_B |= (1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN3);
	//_delay_us(motor_time);
//
	//STEPPING_A &= ~((1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN3));
	//STEPPING_A |= (1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN4);
	//STEPPING_B &= ~((1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN3));
	//STEPPING_B |= (1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN4);
	//_delay_us(motor_time);
	//
	//STEPPING_A &= ~((1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN3));
	//STEPPING_A |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN4);
	//STEPPING_B &= ~((1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN3));
	//STEPPING_B |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN4);
	//_delay_us(motor_time);
//
	//STEPPING_A &= ~((1 << STEPPING_A_IN2) | (1 << STEPPING_A_IN4));
	//STEPPING_A |= (1 << STEPPING_A_IN1) | (1 << STEPPING_A_IN3);
	//STEPPING_B &= ~((1 << STEPPING_B_IN2) | (1 << STEPPING_B_IN4));
	//STEPPING_B |= (1 << STEPPING_B_IN1) | (1 << STEPPING_B_IN3);
	//_delay_us(motor_time);
//}