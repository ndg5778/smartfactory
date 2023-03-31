import RPi.GPIO as GPIO          # Raspi GPIO선 사용 코드
import time                      # time.sleep을 사용가능하게 함

import serial

# Set up the serial port
py_serial = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)


infrared1 = 20                   # 적외선1 out선을 GPIO 20번에 연결
infrared2 = 21                   # 적외선2 out선을 GPIO 21번에 연결

GPIO.setmode(GPIO.BCM)           # setmode를 BCM으로 설정
GPIO.setup(infrared1, GPIO.IN)   # 적외선 장애물 감지 센서1 설정
GPIO.setup(infrared2, GPIO.IN)   # 적외선 장애물 감지 센서2 설정

state1 = GPIO.input(infrared1)
state2 = GPIO.input(infrared2)

first_box_count = 0
time_count = 20
pass_while = 1

while state1 == 1 and state2 == 1:
    state1 = GPIO.input(infrared1)
    state2 = GPIO.input(infrared2)

    py_serial.write(b'sta$')
    print('sta')
    time.sleep(1)
    data = py_serial.readline().decode().strip()
    print(f'data : {data}')

    if(data == "fst"):
        pass_whlie = 1
        while (pass_while == 1):
            state2 = GPIO.input(infrared2)
            if(state2 == 0):
                for i in range(10):
                    py_serial.write(b'stp$')
                    print(f'stp{i}')
                    time.sleep(0.5)
                break
                print(pass_whlie)
            elif (state2 == 1):
                py_serial.write(b'go$')
                print('go')
                time.sleep(0.05)


        print('welcome')
        pass_while = 1
        for i in range(10):
            py_serial.write(f'{first_box_count}$'.encode())
            print(first_box_count)
            time.sleep(0.5)
        
        while(pass_while == 1):

            # 첫 번째 적외선센서가 20초동안 인식이 안 되면 (컨베이어 벨트에 상자가 없으면) 로봇팔 재가동
            # 첫 번째 적외선센서가 인식되면 스테핑모터 돌리기
            if(state1 == 1):
                time_count -= 1
                time.sleep(1)

            elif(state1 == 0):
                py_serial.write(b'go$')
                time_count = 20
                pass_while = 0
            
            if (time_count == 0):
                py_serial.write(b'stp$')
                time_count = 20
                pass_while = 0

    # 적외선 센서가 인식되기 전까지 컨베이어 벨트가 돌아가는 코드
    elif (data == "stm"):
        pass_whlie = 1
        while (pass_while == 1):
            state2 = GPIO.input(infrared2)
            if(state2 == 0):
                for i in range(10):
                    py_serial.write(b'stp$')
                    print(f'stp{i}')
                    time.sleep(0.5)
                pass_whlie = 0
                print(pass_while)
            elif (state2 == 1):
                py_serial.write(b'go$')
                print('go')
                time.sleep(0.05)

    else:
        pass
