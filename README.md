# smartfactory

**팀원**
장하현 : [github](https://github.com/ndg5778)  
김요한 : [github](https://github.com/kimyohan12)  
김현민 :   
하정수 : [github](https://github.com/jungsu00)  


## 프로젝트 설명
객체인식 기술을 활용하여 물체 감지 시스템을 구축하고 해당 물체를 분류하는 서비스.

### 사용 부품
Raspberry pi4 B+, ATmega328p

### 사용 언어
C, Python

### 사용 서비스
Ubuntu(ver 22.10), YOLOv5, MySQL, Server, OpenCV, PyTorch, Database, Flask

## 프로젝트 실행
> ATmega328p (Atmel studio7) : [SmartFactory.c](https://github.com/ndg5778/smartfactory/blob/main/project/Atmega328p/SmartFactory.c)  

> Raspberry pi (ubuntu) : [project_detect.py](https://github.com/ndg5778/smartfactory/blob/main/project/raspberry%20pi/project_detect.py)  
> 실행 방법  
>   
> > 1. pytorch 다운로드  
> > 2. yolov5 다운로드  
> > 3. pymysql 다운로드  
> > ```sql
> > -- database = blog  
> > -- table = project로 생성  
> > 	create table project(  
> > 	id int(11) NOT NULL AUTO_INCREMENT,  
> > 	name varchar(40) NOT NULL,  
> > 	number int(11) NOT NULL,  
> > 	number1 int(11) NOT NULL,  
> > 	PRIMARY KEY(id));
> >  ```  
> >		
> > 4. Raspi Searial 연결 상태여야 함  
> > ``` python
> > port = '/dev/ttyAMA0'   # Raspi Searial port  
> > baudrate = 9600         # 보드 레이트 (통신 속도)  
> >```
> > 5. STK500을 사용하여 Atmega328P와 연결하여 Atmel studio사용  
> > 6. 명령어를 입력하여 실행  
> > ``` bash
> >python3 project_detect.py --weights project_best.pt --source 0 --conf 0.8
> > ```

## 프로젝트 설명

### 회로도
https://user-images.githubusercontent.com/59169163/229057838-493aee0e-893c-4f02-8198-be8c051c9ad6.png
