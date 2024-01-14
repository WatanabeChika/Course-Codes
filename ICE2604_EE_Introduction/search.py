from elasticsearch import Elasticsearch
import json
es = Elasticsearch()
body={
  "query": {
    "bool": {
      "should": [
        {"match": {
          "keyword": "adaptive Huffman coding"
        }},
        {"match": {
          "content": "adaptive Huffman coding"
        }
        }
      ]
    }
  },
  "size":50
}
aim = input()
body["query"]["bool"]["should"][0]["match"]["keyword"] =aim
body["query"]["bool"]["should"][1]["match"]["content"] =aim
res = es.search(index='final_work_index',body=body)
num = res["hits"]["max_score"]
if(num):
    lim_num = num*0.5
    for c in res["hits"]["hits"]:
      if(c["_score"]>lim_num):
          print(c["_id"],c["_source"]["keyword"])