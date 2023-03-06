//#define F_CPU 16000000L
//#include <avr/io.h> // include the AVR IO library
//#include <util/delay.h> // include the delay library
//#include "pca9685.h" // include the PCA9685 library
//
//#define test (uint8_t)0
//#define test2 (uint8_t)1
//#define min_angle (uint16_t)600
//#define test_angle (uint16_t)1800
//#define max_angle (uint16_t)1800
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
	//LED_PORT |= (1 << LED2);
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
	//while (1) {
		//LED_PORT |= (1 << LED1);
		////pca9685_pwm(test, min_angle);
		//for (angle = min_angle; angle <= max_angle; angle += 20) {
			//pca9685_pwm(test, angle);
			//_delay_ms(50);
		//}
		//for (angle = max_angle; angle >= min_angle; angle -= 20) {
			//pca9685_pwm(test, angle);
			//_delay_ms(50);
		//}
		////for (angle = min_angle; angle <= max_angle; angle += 5) {
			////pca9685_pwm(test2, angle);
			////_delay_ms(20);
		////}
		////for (angle = max_angle; angle >= min_angle; angle -= 5) {
			////pca9685_pwm(test2, angle);
			////_delay_ms(20);
		////}
		//
		//pca9685_pwm(test, test_angle);
		//
		//LED_PORT &= ~(1 << LED1);
		//
	//}
	//return 0;
//}
//
