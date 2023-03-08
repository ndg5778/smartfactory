import RPi.GPIO as GPIO
import time
import serial

ser = serial.Serial(port = '/dev/ttyAMA0',
                    baudrate = 9600,
                    timeout = 1)

# 적외선센서 GPIO pin
infrared_A = 23
infrared_B = 24

# 변수 선언
num = 0
                               
GPIO.setmode(GPIO.BCM)        # setmode를 BCM으로 설정
GPIO.setup(infrared_A, GPIO.IN) # 적외선 장애물 감지 센서 설정
GPIO.setup(infrared_B, GPIO.IN)

stop_receive = 0

def uart_canmove():
    stop_receive = 1
    

while True:
    try:
        data = ser.readline().decode().strip()
        if (data == "====First Robot Arm===="):
            for i in range(10):
                ser.write(b'First$')
            print(f"1. Receivede data: {data}")
            
            stop_receive = 1
            
            while (stop_receive != 0):
                
                #### database 연결
                #print("num : ", end="")
                #print(ser.write(f'{str(num)}$'.encode('utf-8')))
                ## print(f'num : {num}')
                
                num = input("몇 번째 어쩌구 : ")

                ser.write(f'{num}$'.encode())
                print(f'{num}$'.encode())

                time.sleep(1)
                
                data = ser.readline().decode().strip()
                
                if ("pass" in data):
                    print(f">pass// num : {num}, data : {data}")
                    # print(f"num : {num}")
                    # print("move!!")
                    # num += 1
                    stop_receive = 0
                elif ("end" in data):
                    print(f">end// num : {num}, data : {data}")
                    # print(f"num : {num}")
                    #print("stop!!")
                    stop_receive = 0
                elif (data == "===First Robot Arm==="):
                    pass
                else:
                    print(f"what is Received data?: {data}")

                print(f"{data}")
                # time.sleep(1)

        elif (data == "====First Infrared===="):
            print(f"Received data: {data}")
            pass_infrared = 1
            count = 0

            while(pass_infrared != 0):
                state = GPIO.input(infrared_A)
                if (state == 0):
                    print("box is on the convayor")
                    ser.write(b'go$')
                    pass_infrared = 0

                elif (state == 1):
                    count += 1
                    print(count)
                    time.sleep(1)
                    
                if (count >= 10):
                    ser.write(b'stp$')
                    print("stop conveyor")
                    pass_infrared = 0

        elif (data == "====stepping motor===="):
            print(f"Received data: {data}")
            stop_receive = 1
            while (stop_receive != 0):
                t = time.localtime()
                
                state_B = GPIO.input(infrared_B)
                if (state_B == 0):
                    for i in range(5):
                        ser.write(b'stp$')
                        print(f"{t.tm_hour}:{t.tm_min}:{t.tm_sec} - send data : {state_B}")
                        time.sleep(1)
                    # print(f"send data A : {state_B}")
                    time.sleep(1)
                    stop_receive = 0
                # elif (stop_receive == 2 and state_B == 1):
                #     ser.write(b'mov$')
                elif (state_B == 1):
                    ser.write(b'go$')   
                    print(f"{t.tm_hour}:{t.tm_min}:{t.tm_sec} - send data : {state_B}")
                    # print(f"send data B : {state_B}")
                    time.sleep(0.05)
        elif(data == ""):
            pass
        else:
            print(f"Received other data: {data}")

    except UnicodeDecodeError:
        pass
