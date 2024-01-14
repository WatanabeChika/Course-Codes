import requests
from lxml import etree
import json
import time

header = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0'}

def get_data(url, header):
    time.sleep(1)
    response = requests.get(url, headers=header)
    response.encoding = 'utf-8'
    return response


originalUrl = 'https://zh.moegirl.org.cn/%E6%98%8E%E6%97%A5%E6%96%B9%E8%88%9F/%E5%B9%B2%E5%91%98%E5%9B%BE%E9%89%B4'

originalHTML = etree.HTML(get_data(originalUrl, header).text)
nextUrls = originalHTML.xpath('//div[@class="akIll"]/span[@class="akIllLayer7"]/a/@href')

final_data = []

for j in nextUrls:
    request_times = 0
    fullUrl = 'https://zh.moegirl.org.cn' + j
    response = get_data(fullUrl, header)
    if response.status_code == 404:
        continue
    while response.status_code != 200 :
        print(response.status_code)
        time.sleep(10)
        response = get_data(fullUrl, header)
        request_times += 1
        if request_times > 10:
            break
    if request_times > 10:
        continue

    nextHTML = etree.HTML(response.text)
    mydic = dict()

    metadata = nextHTML.xpath('//div[@class="aoc-data aoc-fullwidth-only"]//text()')
    artist = nextHTML.xpath('//div[@class="aoc-artist"]//text()')


    pre_metadata = []
    for i in metadata:
        if i != ' ' and i[0] != '(' and i[0] != '（' and i[-1] != '）' and i[-1] != ')' and i[0] != '[' and i[-1] != ']':
            pre_metadata.append(i)

    mydic[artist[0][:-1]] = artist[1]
    for i in range(len(pre_metadata)):
        if i % 2 != 0:
            mydic[pre_metadata[i-1]] = pre_metadata[i]
    print(mydic)
    final_data.append(mydic)


with open ('new_ark.json', 'a', encoding='utf-8') as f:
    json.dump(final_data, f, ensure_ascii=False, indent = 2)



# fullUrl = 'https://zh.moegirl.org.cn/%E7%99%BD%E9%87%91(%E6%98%8E%E6%97%A5%E6%96%B9%E8%88%9F)#'

# response = get_data(fullUrl, header)
# nextHTML = etree.HTML(response.text)

# mydic = dict()

# metadata = nextHTML.xpath('//div[@class="aoc-data aoc-fullwidth-only"]//text()')
# artist = nextHTML.xpath('//div[@class="aoc-artist"]//text()')


# pre_metadata = []
# for i in metadata:
#     if i != ' ' and i[0] != '(':
#         pre_metadata.append(i)

# mydic[artist[0][:-1]] = artist[1]
# for i in range(len(pre_metadata)):
#     if i % 2 == 0:
#         mydic[pre_metadata[i]] = pre_metadata[i+1]

# print(mydic)

