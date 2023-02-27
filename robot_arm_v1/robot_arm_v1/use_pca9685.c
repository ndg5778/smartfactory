//#define F_CPU 16000000L
//#include <avr/io.h> // include the AVR IO library
//#include <util/delay.h> // include the delay library
//#include "pca9685.h" // include the PCA9685 library
//
//#define test (uint8_t)5
//#define min_angle (uint16_t)1200
//#define max_angle (uint16_t)1800
//
//int main(void)
//{
	//pca9685_init(0x00, 50); // start PCA9685 device 0x00 at 50 Hz output
	//_delay_ms(5);
	//
	//uint16_t angle = 0;
	//pca9685_servo(test, 0);
	//_delay_ms(1000);
//
	//while (1) {
		//pca9685_pwm(test, 0);
		//for (angle = min_angle; angle <= max_angle; angle += 20) {
			//pca9685_pwm(test, angle);
			//_delay_ms(100);
		//}
		//for (angle = max_angle; angle >= min_angle; angle -= 20) {
			//pca9685_pwm(test, angle);
			//_delay_ms(100);
		//}
		//
	//}
	//return 0;
//}