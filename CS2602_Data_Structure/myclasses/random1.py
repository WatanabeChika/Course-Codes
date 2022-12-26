import random
import time

so = random.seed(time.time)

st = input("start: ")
en = input("end: ")

res = random.randint(int(st), int(en))

print(res)
