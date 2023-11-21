import json
import pymysql

with open ('ArkOperators.json', 'r', encoding='utf-8') as f:
    reslis = json.load(f)

titlelis = ["代号", "英文代号", "性别", "本名", "别号", "发色", "瞳色", "身高", "生日", "种族", 
            "职业", "职能", "专精", "萌点", "出身地区", "活动范围", "所属团体", "个人状态", "角色EP"]

# print([reslis[0].get(title, '/') for title in titlelis])
# sql = 'INSERT INTO ark_operators VALUES({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, \
#             {14}, {15}, {16}, {17}, {18})'.format(*[reslis[0].get(title, '/') for title in titlelis])
# print(sql)


# 导入进数据库
db = pymysql.connect(host='127.0.0.1', port=3306, user='root', passwd='20030804wly', db='mydata', charset='utf8')

cursor = db.cursor()
cursor.execute("CREATE TABLE IF NOT EXISTS `ark_operators`(`name` VARCHAR(100) NOT NULL, `English_name` VARCHAR(100) NULL, \
                `sex` VARCHAR(100) NOT NULL, `real_name` VARCHAR(100) NULL, `nick_name` VARCHAR(500) NULL, \
                `hair_color` VARCHAR(100) NULL, `pupil_color` VARCHAR(100) NULL, `height` VARCHAR(100) NULL, \
                `birthday` VARCHAR(100) NULL, `race` VARCHAR(100) NULL, `category` VARCHAR(100) NULL, \
                `career` VARCHAR(100) NULL, `professional` VARCHAR(100) NULL, `moe_point` VARCHAR(500) NULL, \
                `hometown` VARCHAR(100) NULL, `live_scope` VARCHAR(100) NULL, `group` VARCHAR(100) NULL, \
                `personal_status` VARCHAR(100) NULL, `EP` VARCHAR(100) NULL, \
                PRIMARY KEY(`name`)) ENGINE=InnoDB, DEFAULT CHARACTER SET=utf8mb4")

for i in reslis:
    sql = 'INSERT INTO ark_operators VALUES("{0}", "{1}", "{2}", "{3}", "{4}", "{5}", "{6}", "{7}", "{8}", "{9}", \
            "{10}", "{11}", "{12}", "{13}", "{14}", "{15}", "{16}", "{17}", "{18}")'.format(*[i.get(title, '/') for title in titlelis])
    cursor.execute(sql)
    db.commit()

db.close()