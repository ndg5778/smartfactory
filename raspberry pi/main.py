import RPi.GPIO as GPIO
import time
import serial

ser = serial.Serial(port = '/dev/ttyS0',
                    baudrate = 9600,
                    timeout = 1)

# 적외선센서 GPIO pin
infrared_A = 18
infrared_B = 20

# 변수 선언
num = 0
                               
GPIO.setmode(GPIO.BCM)        # setmode를 BCM으로 설정
GPIO.setup(infrared_A, GPIO.IN) # 적외선 장애물 감지 센서 설정
GPIO.setup(infrared_B, GPIO.IN)

stop_receive = 0

def uart_canmove():
    stop_receive = 1
    


while True:
    data = ser.readline().decode('utf-8').strip()
    if (data == "====First Robot Arm===="):
        print(f"1. Received data: {data}")
        stop_receive = 1
        while (stop_receive != 0):
            
            ### database 연결
            print("num : ", end="")
            print(ser.write(f'{str(num)}$'.encode('utf-8')))
            # print(f'num : {num}')

            time.sleep(1)
            data = ser.readline().decode('utf-8').strip()
            if (data == "pass"):
                print(f">pass// num : {num}, data : {data}")
                # print(f"num : {num}")
                print("move!!")
                num += 1
                stop_receive = 0
            elif (data == "end9"):
                print(f">end// num : {num}, data : {data}")
                # print(f"num : {num}")
                print("stop!!")
                stop_receive = 0
            # elif (not data):
            #     pass
            else:
                print(f"what is Received data?: {data}")

            print(f"{data}")
            # time.sleep(1)

    elif (data == "====stepping motor===="):
        print(f"Received data: {data}")
        stop_receive = 1
        while (stop_receive != 0):
            state_A = GPIO.input(infrared_A)
            state_B = GPIO.input(infrared_B)
            if (state_A == 0):
                ser.write(b'mov$')
                print(f"send data A : {state_A}")
                # time.sleep(1)
                stop_receive = 2
            elif (stop_receive == 2 and state_B == 1):
                ser.write(b'mov$')
            elif (state_B == 0):
                ser.write(b'stp$')
                for i in range(10):
                    print(f"send data B : {state_B}")
                stop_receive = 0
    elif(data == ""):
        pass
    else:
        print(f"Received other data: {data}")

    