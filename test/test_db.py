# 1. 실행하기
import pymysql

# 2. connect
db = pymysql.connect(
    host="localhost",
    user="root",
    passwd="1234",
    db="blog",
    charset="utf8")

# conn = pymysql.connect(host='localhost', port=3306,  
#                        user='root', password='1234', 
#                        db='iwanttogohome', charset='utf8',
#                        autocommit=True, # 결과 DB 반영
#                        cursorclass=pymysql.cursors.DictCursor # DB조회시 컬럼명을 동시에 보여줌
#                        ) 

# 3. curosr 사용하기
cursor = db.cursor()
# cursor = conn.cursor() 


## DB 닫아주기 
db.close()
