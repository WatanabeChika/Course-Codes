from pymysql.converters import escape_string
import pymysql

kwargs = {
    'host': "localhost",
    'user': "root",
    'password': "20030804wly",
    'charset': 'utf8mb4',
    'database': 'dianlei'    
}

db = pymysql.connect(**kwargs)

cur = db.cursor()
cur.execute('set global wait_timeout=86400;')

users = [
    ['ljq', '123456'],
    ['wly', '123456'],
    ['lj', '123456'],
    ['yjj', '123456'],
    ['djl', '123456'],
]

if __name__ == '__main__':
    cur = db.cursor()
    cur.execute('DROP DATABASE IF EXISTS dianlei;')
    cur.execute('CREATE DATABASE dianlei;')
    cur.execute('USE dianlei;')

    cur.execute(
    '''   
    CREATE TABLE rec(
    id INT AUTO_INCREMENT PRIMARY KEY, 
    name varchar(100),
    sumy mediumtext,
    cont mediumtext
    )AUTO_INCREMENT = 0 default charset=utf8mb4;
    '''
    )

    keyword = open('data/keyword.txt', 'r', encoding='utf-8').read().split() 

    l=0; r=1120
    for i in range(l, r+1):
        cur.execute(
            f'''
            INSERT INTO rec(name, cont, sumy)
            VALUES (
                '{escape_string(keyword[i])}' ,
                '{escape_string(open(f"data/mydata_{i}.html", "r", encoding="utf-8").read())}',
                '{escape_string(open(f"data/mytext_{i}.txt", "r", encoding="utf-8").read())}'
            );
            '''
        )
    cur.execute(
    '''   
    CREATE TABLE user(
        id INT AUTO_INCREMENT PRIMARY KEY,
        name varchar(100),
        passwd varchar(100)
    )AUTO_INCREMENT = 0 default charset=utf8mb4;
    '''
    )
    for user in users:
        cur.execute(
            f'''
            INSERT INTO user(name,passwd)
            VALUES (
                '{escape_string(user[0])}',
                '{escape_string(user[1])}'
            );
            '''
        )
    cur.execute(
    '''   
    CREATE TABLE favoritelist(
        id INT AUTO_INCREMENT PRIMARY KEY,
        username varchar(100),
        recid int
    )AUTO_INCREMENT = 0 default charset=utf8mb4;
    '''
    )
    db.commit()

def all():
    cur.execute(f'SELECT name,id from rec')
    return cur.fetchall()

def search(key):
    cur.execute(f'SELECT name,id,left(sumy,400) from rec WHERE name LIKE "%{key}%";')
    return cur.fetchall()

def id(id):
    cur.execute(f'SELECT name, cont from rec WHERE id = {id};')
    return cur.fetchone()

def like(name, id):
    if name == None: return 0
    return cur.execute(f'SELECT * from favoritelist WHERE username = "{escape_string(name)}" and recid = {id};')
    # return 0

def check(name, passwd):
    return cur.execute(f'SELECT * from user WHERE name = "{escape_string(name)}" and passwd = "{escape_string(passwd)}";')

def choosefrom(name):
    cur.execute(f'''
        select r.name,r.id from 
        (favoritelist f INNER JOIN rec r ON f.username='{name}' and f.recid=r.id);
    ''')
    return cur.fetchall()

def removelike(name, id):
    cur.execute(f'''
        delete from favoritelist
        where username = "{escape_string(name)}" and recid = {id};
    ''')

def addlike(name, id):
    if not like(name, id):
        cur.execute(f'''
            insert into favoritelist(username, recid)
            values ( "{escape_string(name)}", {id});
        ''')    