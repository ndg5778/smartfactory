//#define F_CPU 16000000L
//#include <avr/io.h> // include the AVR IO library
//#include <util/delay.h> // include the delay library
//#include "pca9685.h" // include the PCA9685 library
//
//#define servo1 (uint8_t)0
//#define servo2 (uint8_t)1
//#define servo3 (uint8_t)2
//#define servo4 (uint8_t)3
//#define servo5 (uint8_t)4
//
//#define min_angle (uint16_t)600
//#define test_angle (uint16_t)2400
//#define max_angle (uint16_t)2400
//
//// LED (상태등)
//#define LED_DDR DDRB
//#define LED_PORT PORTB
//#define LED1 PB0
//#define LED2 PB1
//
//int main(void)
//{
	//LED_DDR |= (1 << LED1) | (1 << LED2);
	////LED_PORT |= (1 << LED2);
	//
	//pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
//
	////LED_PORT |= (1 << LED1);
	//_delay_ms(5);
	//
	//uint16_t angle = 0;
	//_delay_ms(1000);
	////pca9685_servo(0, 0);
	//
	//int i;
	//for (i = 0; i < 4; i++){
		//pca9685_pwm(i, 1800);
		//_delay_ms(200);
	//}
	//pca9685_pwm(4, 1050);
	//_delay_ms(200);
	//
	////pca9685_pwm(servo2, 1700);
//
	//while (1) {
		//LED_PORT |= (1 << LED1);
		//
		////pca9685_pwm(servo3, test_angle);
		////pca9685_pwm(test2, test_angle);
		//
		////// servo1
		////for (angle = 1700; angle <= 2000; angle += 20) {
			////pca9685_pwm(servo1, angle);
			////_delay_ms(50);
		////}
		////for (angle = 2000; angle >= 1700; angle -= 20) {
			////pca9685_pwm(servo1, angle);
			////_delay_ms(50);
		////}
		////_delay_ms(100);
		////
		////// servo2
		////for (angle = 1700; angle <= 2000; angle += 20) {
			////pca9685_pwm(servo2, angle);
			////_delay_ms(50);
		////}
		////for (angle = 2000; angle >= 1700; angle -= 20) {
			////pca9685_pwm(servo2, angle);
			////_delay_ms(50);
		////}
		////_delay_ms(100);
		//
		//// servo3
		//for (angle = 700; angle <= 2300; angle += 20) {
			//pca9685_pwm(servo3, angle);
			//_delay_ms(50);
		//}
		//for (angle = 2300; angle >= 700; angle -= 20) {
			//pca9685_pwm(servo3, angle);
			//_delay_ms(50);
		//}
		//_delay_ms(100);
		//
		////// servo4
		////for (angle = 1700; angle <= 2000; angle += 20) {
			////pca9685_pwm(servo4, angle);
			////_delay_ms(50);
		////}
		////for (angle = 2000; angle >= 1700; angle -= 20) {
			////pca9685_pwm(servo4, angle);
			////_delay_ms(50);
		////}
		////_delay_ms(100);
		////
		////// servo5
		////for (angle = 600; angle <= 1050; angle += 20) {
			////pca9685_pwm(servo5, angle);
			////_delay_ms(50);
		////}
		////for (angle = 1050; angle >= 600; angle -= 20) {
			////pca9685_pwm(servo5, angle);
			////_delay_ms(50);
		////}
		////_delay_ms(100);
		//
		////for (angle = min_angle; angle <= max_angle; angle += 5) {
			////pca9685_pwm(test2, angle);
			////_delay_ms(20);
		////}
		////for (angle = max_angle; angle >= min_angle; angle -= 5) {
			////pca9685_pwm(test2, angle);
			////_delay_ms(20);
		////}
		//
		////pca9685_pwm(test, test_angle);
		//
		//LED_PORT &= ~(1 << LED1);
		//
	//}
	//return 0;
//}