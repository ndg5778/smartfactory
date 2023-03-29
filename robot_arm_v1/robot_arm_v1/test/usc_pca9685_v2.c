﻿//#define F_CPU 16000000L
//#include <avr/io.h> // include the AVR IO library
//#include <util/delay.h> // include the delay library
//#include "../ServoDriver/pca9685.h" // include the PCA9685
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
//#define SERVO_A(x) (uint8_t)(x - 1)		// SERVO_A(1) = 0, 1, 2, 3, 4
//#define SERVO_B(x) (uint8_t)(x + 7)		// SERVO_B(1) = 8, 9, 10, 11, 12
//
//// pulse
//#define ANGLE(x) (uint16_t)((10 * x) + 600)		// 600 ~ 2400
//
//#define servo_max_A 18
//#define servo_max_Bfst 15
//#define servo_max_B 7
//
//uint16_t move_Aarm_coord[9][servo_max_A][3] = {
	//
	//{
		///* 01 */
		//{1, 90, 85}, {4, 90, 130}, {2, 90, 55}, {3, 90, -10}, {5, 45, 10},
		//{4, 130, 180}, {3, -10, 30}, {2, 55, 77},			
		//{1, 92, 125}, {3, 30, -15}, {4, 180, 130}, {5, 10, 45}, {4, 130, 180}, {2, 77, 90}, {1, 125, 90}
	//},	
	//
	//{
		///* 02 */
		//{1, 90, 64}, {4, 90, 180}, {2, 90, 55}, {3, 90, -15}, {4, 180, 140}, {5, 45, 10}, 
		//{4, 140, 180}, {2, 55, 77},		
		//{1, 65, 125}, {3, -12, -15}, {4, 180, 130}, {5, 10, 45}, {4, 130, 180}, {3, -25, 30}, {2, 85, 90}, {1, 125, 90}
	//},
	//
	//{
		///* 03 */
		//{1, 90, 44}, {4, 90, 180}, {2, 90, 53}, {3, 90, -7}, {4, 180, 130}, {5, 45, 10},				
		//{4, 140, 180}, /*{2, 68, 90},*/ {3, -7, 30},			
		//{1, 45, 125}, {2, 53, 77}, {4, 180, 130}, {3, 30, -15}, {5, 10, 45}, {4, 150, 180}, {2, 77, 90}, {1, 125, 90}
	//},
	//
	//{
		///* 04 */
		//{1, 90, 81}, {4, 90, 180}, {2, 90, 43}, {3, 90, 10}, {4, 180, 137}, {5, 45, 10},
		//{4, 150, 180}, {2, 43, 77}, {3, 10, 30},
		//{1, 81, 125}, {4, 180, 130}, {3, 30, -15}, {5, 10, 45}, {4, 130, 180}, {2, 77, 90}, {1, 125, 90}
	//},
	//
	//{
		///* 05 */
		//{1, 90, 64}, {4, 90, 180}, {2, 90, 45}, {3, 90, 3}, {4, 180, 130}, {5, 45, 10},
		//{4, 140, 180}, {2, 47, 77}, {3, 10, 30},
		//{1, 45, 125}, {4, 180, 130}, {3, 30, -15}, {5, 10, 45}, {4, 130, 180}, {2, 77, 90}, {1, 125, 90}
	//},
	//
	//{
		///* 06 */
		//{1, 90, 46}, {4, 90, 180}, {2, 90, 45}, {3, 90, 11}, {4, 180, 128}, {5, 45, 10},
		//{4, 130, 180}, {2, 45, 77}, {3, 8, 30},
		//{1, 52, 125}, {4, 180, 130}, {3, 30, -15}, {5, 10, 45}, {4, 130, 180}, {2, 77, 90}, {1, 125, 90}
	//},
	//
	//{
		///* 07 */
		//{1, 90, 80}, {4, 90, 180}, {2, 90, 40}, {3, 90, 20}, {4, 180, 120}, {5, 45, 10},
		//{4, 120, 180}, {3, 20, 30}, {2, 46, 77},
		//{1, 81, 126}, {3, 30, -15}, {4, 180, 130}, {5, 10, 45}, {4, 130, 180}, {2, 77, 90}, {1, 125, 90}
	//},
	//
	//
	//{
		///* 08 */
		//{1, 90, 63}, {4, 90, 180}, {2, 90, 43}, {3, 90, 17}, {4, 180, 120}, {5, 45, 10},
		//{4, 130, 180}, {2, 46, 77}, {3, 20, 30},
		//{1, 64, 125}, {4, 179, 150}, {3, 30, -15}, {5, 10, 45}, {4, 130, 180}, {2, 85, 90}, {1, 135, 90}
	//},
	//
	//{
		///* 09 */
		//{1, 90, 57}, {4, 90, 180}, {2, 90, 38}, {3, 90, 22}, {4, 180, 130}, {5, 45, 10},
		//{4, 130, 180}, {2, 38, 77}, {3, 22, 30},
		//{1, 48, 125}, {4, 179, 150}, {3, 30, -25}, {5, 10, 45}, {4, 150, 180}, {2, 85, 90}, {1, 135, 90}
	//}
//};
//uint16_t move_Barm_fst[servo_max_Bfst][3] = {
	//{1, 90, 147}, {4, 90, 180}, {3, 90, -7}, {2, 90, 70}, {4, 180, 120},
		//{3, -7, -2},
		 //{1, 147, 150}, {2, 65, 60}, {4, 120, 130}, {5, 120, 90},
	//{4, 130, 180}, {2, 60, 90}, {3, 6, 30}, {3, -2, 30}
//};
//uint16_t move_Barm_coord[9][servo_max_B][3] = {
	//{/* 09 */{1, 147, 67}, {2, 90, 25}, {3, 30, 30}, {4, 180, 130}, {5, 90, 120}, {2, 30, 90}},
	//{/* 08 */{1, 147, 90}, {2, 90, 30}, {3, 30, 25}, {4, 180, 130}, {5, 90, 120}, {2, 30, 90}},
	//{/* 07 */{1, 147, 105}, {2, 90, 28}, {3, 30, 27}, {4, 180, 130}, {5, 90, 120}, {2, 30, 90}},
	//{/* 06 */{1, 147, 65}, {2, 90, 35}, {3, 30, 10}, {4, 180, 130}, {5, 90, 120}, {2, 32, 90}},
	//{/* 05 */{1, 147, 90}, {2, 90, 45}, {3, 30, 6}, {4, 180, 130}, {5, 90, 120}, {2, 45, 90}},
	//{/* 04 */{1, 147, 110}, {2, 90, 35}, {3, 30, 13}, {4, 180, 130}, {5, 90, 120}, {2, 35, 90}},
	//{/* 02 */{1, 147, 90}, {2, 90, 50}, {3, 30, -7}, {4, 180, 130}, {5, 90, 120}, {2, 50, 90}},
	//{/* 02 */{1, 147, 90}, {2, 90, 50}, {3, 30, -7}, {4, 180, 130}, {5, 90, 120}, {2, 50, 90}},
	//{/* 01 */{1, 147, 110}, {2, 90, 42}, {3, 30, -0}, {4, 180, 150}, {5, 90, 120}, {2, 42, 90}}
//};
//
//
//
//// 서보모터 초기화
//void INIT_SERVO();
//// 서보모터 움직이기
//void MoveServo(uint8_t, uint16_t, uint16_t);
//// 로봇암 쉽게 움직이기
//void MoveRobotArm(uint8_t, uint8_t);
//
//int main(void)
//{
	//LED_DDR |= (1 << LED1) | (1 << LED2);
	//
	//int servotime = 200;
	//pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	//
	////uint16_t angle = 0;
	//_delay_ms(1000);
//
	//while (1) {
		//LED_PORT |= (1 << LED1);
		//
		//INIT_SERVO();
		////_delay_ms(1000);
		////
		////MoveRobotArm(1, 8);
		////_delay_ms(5000);
		//
		//int i;
		//for ( i = 0; i < 9; i++ ) {
			//MoveRobotArm(1, i);
			//_delay_ms(5000);
			//
			//INIT_SERVO();
		//}
		//
		////int i;
		////for (i = 1; i <= 5; i++){
			////if (i == 5) {
				////MoveServo(SERVO_A(i), ANGLE(45), ANGLE(0));
				////_delay_ms(servotime);
				////MoveServo(SERVO_A(i), ANGLE(0), ANGLE(45));
				////_delay_ms(servotime);
			////}
			////MoveServo(SERVO_A(i), ANGLE(90), ANGLE(70));
			////_delay_ms(servotime);
			////MoveServo(SERVO_A(i), ANGLE(70), ANGLE(90));
			////_delay_ms(servotime);
		////}
		////_delay_ms(servotime);
		////
		//////int i;
		////for (i = 1; i <= 5; i++){
			////if (i == 3) {
				////MoveServo(SERVO_B(i), ANGLE(130), ANGLE(110));
				////_delay_ms(servotime);
				////MoveServo(SERVO_B(i), ANGLE(110), ANGLE(130));
				////_delay_ms(servotime);
			////}
			////else {
				////MoveServo(SERVO_B(i), ANGLE(90), ANGLE(70));
				////_delay_ms(servotime);
				////MoveServo(SERVO_B(i), ANGLE(70), ANGLE(90));
				////_delay_ms(servotime);
			////}
		////}
		////_delay_ms(5000);
		//
		/////* 함수 이용하기 */
		////INIT_SERVO();
		////_delay_ms(2000);
		////MoveRobotArm(2, 8);
		//////_delay_ms(50000000000000);
		////_delay_ms(5000);
		//
		//
		//
		 //
		///* 집기 */
		////MoveServo(SERVO_A(1), ANGLE(90), ANGLE(70));
		////_delay_ms(100);
		////MoveServo(SERVO_A(1), ANGLE(70), ANGLE(90));
		////_delay_ms(100);
		////MoveServo(SERVO_A(4), ANGLE(30), ANGLE(90));
		////_delay_ms(100);
		////MoveServo(SERVO_A(4), ANGLE(90), ANGLE(30));
		////_delay_ms(100);
		////MoveServo(SERVO_B(2), ANGLE(90), ANGLE(70));
		////_delay_ms(100);
		////MoveServo(SERVO_B(4), ANGLE(180), ANGLE(120));
		////_delay_ms(100);
		////MoveServo(SERVO_B(5), ANGLE(90), ANGLE(120));
		////_delay_ms(100);
		////
		////MoveServo(SERVO_B(3), ANGLE(-17), ANGLE(-12));
		////_delay_ms(100);
		////MoveServo(SERVO_B(2), ANGLE(70), ANGLE(60));
		////_delay_ms(100);
		////MoveServo(SERVO_B(4), ANGLE(120), ANGLE(130));
		////_delay_ms(100);
		////MoveServo(SERVO_B(5), ANGLE(120), ANGLE(97));
		////_delay_ms(100);
//
		///* 들기 */
		////MoveServo(SERVO_B(4), ANGLE(130), ANGLE(180));
		////_delay_ms(100);
		////MoveServo(SERVO_B(2), ANGLE(60), ANGLE(90));
		////_delay_ms(100);
		////MoveServo(SERVO_B(3), ANGLE(6), ANGLE(30));
		////_delay_ms(100);
//
//
		///* 놓기 */
		////MoveServo(SERVO_B(3), ANGLE(-12), ANGLE(20));
		////_delay_ms(100);
		////MoveServo(SERVO_B(1), ANGLE(147), ANGLE(110));
		////_delay_ms(1000);
		////MoveServo(SERVO_B(2), ANGLE(90), ANGLE(42));
		////_delay_ms(100);
		////MoveServo(SERVO_B(3), ANGLE(20), ANGLE(-10));
		////_delay_ms(100);
		////MoveServo(SERVO_B(4), ANGLE(180), ANGLE(150));
		////_delay_ms(100);
		////MoveServo(SERVO_B(5), ANGLE(97), ANGLE(120));
		////_delay_ms(100);
		////
		////MoveServo(SERVO_B(2), ANGLE(42), ANGLE(90));
		////_delay_ms(100);
		//
		//
		////_delay_ms(50000000000000);
		//
		///* 각도 찾기 */
		////{{1, 90, 82}, {4, 90, 180}, {2, 90, 43}, {3, 90, 10}, {4, 180, 137}, {5, 45, 10},
		////1, 90, 87}, {4, 90, 180}, {2, 90, 46}, {3, 90, 6}, {4, 180, 140}, {5, 45, 23}
		////pca9685_pwm(SERVO_A(1), ANGLE(81));
		////pca9685_pwm(SERVO_A(2), ANGLE(40));
		////pca9685_pwm(SERVO_A(3), ANGLE(20));
		////pca9685_pwm(SERVO_A(4), ANGLE(130));
		////pca9685_pwm(SERVO_A(5), ANGLE(45));
		//
		//LED_PORT &= ~(1 << LED1);
		//
	//}
	//return 0;
//}
//
//void INIT_SERVO(){     
//
	//pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	//int i;
//
	//for (i = 1; i <= 4; i++){
		//if(i == 3) {
			//pca9685_pwm(SERVO_A(i), ANGLE(110));
			//pca9685_pwm(SERVO_B(i), ANGLE(120));
			//_delay_ms(20);
		//}
		//else {
			//pca9685_pwm(SERVO_A(i), ANGLE(90));
			//pca9685_pwm(SERVO_B(i), ANGLE(90));
			//_delay_ms(20);
		//}
	//}
//
	//pca9685_pwm(SERVO_A(5), ANGLE(45));
	//pca9685_pwm(SERVO_B(5), ANGLE(120));
	//_delay_ms(200);
//
//}
//
//void MoveServo(uint8_t servo, uint16_t start_angle, uint16_t end_angle) {
	//int angle;
//
	//if (start_angle <= end_angle) {
		//for (angle = start_angle; angle <= end_angle; ) {
			//if (PINB & (1 << switch1)) {
				//PORTB &= ~0x02;
				//pca9685_pwm(servo, angle);
				////angle += 10;
				//angle++;
			//}
			//else {
				//PORTB |= 0x02;
				//break;
			//}
			//_delay_ms(1);
		//}
	//}
	//
	//else {
		//for (angle = start_angle; angle >= end_angle; ) {
			//if (PINB & (1 << switch1)) {
				//PORTB &= ~0x02;
				//pca9685_pwm(servo, angle);
				////angle -= 10;
				//angle--;
			//}
			//else {
				//PORTB |= 0x02;
				//break;
			//}
			//_delay_ms(1);
		//}
	//}
//
	//_delay_ms(100);
//}
//
//void MoveRobotArm(uint8_t servo, uint8_t count) {
	//
	//int i;
	//
	//switch (servo){
		//case 1:
		//PORTB |= (1 << LED2);
		//for(i = 0; i < servo_max_A; i++){
			//MoveServo(SERVO_A(move_Aarm_coord[count][i][0]), ANGLE(move_Aarm_coord[count][i][1]), ANGLE(move_Aarm_coord[count][i][2]));
		//}
		//break;
		//case 2:
		//for(i = 0; i < servo_max_Bfst; i++){
			//MoveServo(SERVO_B(move_Barm_fst[i][0]), ANGLE(move_Barm_fst[i][1]), ANGLE(move_Barm_fst[i][2]));
		//}
		//for(i = 0; i < servo_max_B; i++){
			//MoveServo(SERVO_B(move_Barm_coord[count][i][0]), ANGLE(move_Barm_coord[count][i][1]), ANGLE(move_Barm_coord[count][i][2]));
		//}
		//break;
	//}
	//
//}