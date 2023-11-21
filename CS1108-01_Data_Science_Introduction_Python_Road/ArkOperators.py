import requests
from lxml import etree
import json
import time

# 请求头
# proxy = {'http': 'http://61.216.185.88:60808/', 'https': 'http://61.216.185.88:60808/'}

header = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36 Edg/118.0.2088.46'}

def get_data(url, header):
    response = requests.get(url, headers=header)
    response.encoding = 'utf-8'
    return response
    
# 萌娘百科干员图鉴页面
originalUrl = 'https://zh.moegirl.org.cn/%E6%98%8E%E6%97%A5%E6%96%B9%E8%88%9F/%E5%B9%B2%E5%91%98%E5%9B%BE%E9%89%B4'

# 获取每个干员个人页面的URL
originalHTML = etree.HTML(get_data(originalUrl, header).text)
nextUrls = originalHTML.xpath('//div[@class="akIll"]/span[@class="akIllLayer7"]/a/@href')

final_data = []

# 获取每个干员的基本资料
for j in nextUrls:
    fullUrl = 'https://zh.moegirl.org.cn' + j
    time.sleep(10)
    response = get_data(fullUrl, header)
    while response.status_code != 200 : response = get_data(fullUrl, header) # 防止网页崩溃访问不了
    nextHTML = etree.HTML(response.text)

    mydic = dict()
    for i in range(3, 20):
        data = nextHTML.xpath('//div[@class="infotemplatebox"]/table/tbody/tr[{0}]/td/text() | //div[@class="infotemplatebox"]/table/tbody/tr[{0}]/td//*[name(.)!="a"]/text()\
                            | //div[@class="infotemplatebox"]/table/tbody/tr[{0}]/td//a[@title]/text()'.format(i))
        id = nextHTML.xpath('//div[@class="infotemplatebox"]/table/tbody/tr[{}]/th//text()'.format(i))
        if not data: break

        id = ''.join([x.strip() for x in id])

        if id == '代号':
            mydic['代号'] = str(data[0]).strip()
            if (len(data) > 1 and len(data[1]) > 1):
                mydic['英文代号'] = str(data[1]).strip()
            elif (len(data) > 4 and len(data[4]) > 1):
                mydic['英文代号'] = str(data[4]).strip()
        elif id == '本名':
            mydic['本名'] = str(data[0]).strip()
        else:
            mydic[id] = ''.join([x.strip() for x in data])
    final_data.append(mydic)

# 缩进导入到json文件里
with open ('ArkOperators_origin.json', 'w', encoding='utf-8') as f:
    json.dump(final_data, f, ensure_ascii=False, indent = 2)


# 单个页面测试用
# response = get_data('https://zh.moegirl.org.cn/%E6%98%8E%E6%97%A5%E6%96%B9%E8%88%9F:%E5%90%BD', header)
# nextHTML = etree.HTML(response.text)
# mydic = dict()
# for i in range(3, 20):
#     data = nextHTML.xpath('//div[@class="infotemplatebox"]/table/tbody/tr[{0}]/td/text() | //div[@class="infotemplatebox"]/table/tbody/tr[{0}]/td//*[name(.)!="a"]/text()\
#                          | //div[@class="infotemplatebox"]/table/tbody/tr[{0}]/td//a[@title]/text()'.format(i))
#     id = nextHTML.xpath('//div[@class="infotemplatebox"]/table/tbody/tr[{}]/th//text()'.format(i))
#     if not data: break

#     id = ''.join([x.strip() for x in id])
#     print(data)

#     if id == '代号':
#         mydic['代号'] = str(data[0]).strip()
#         if (len(data) > 1 and len(data[1]) > 1):
#             mydic['英文代号'] = str(data[1]).strip()
#         elif (len(data) > 4 and len(data[4]) > 1):
#             mydic['英文代号'] = str(data[4]).strip()
#     elif id == '本名':
#         mydic['本名'] = str(data[0]).strip()
#     else:
#         mydic[id] = ''.join([x.strip() for x in data])
# print(mydic)



