//
// Created by delirio on 2022/4/30.
//

#include "Sender.h"

//At first, implement your constructor here

//Defalut constructor
Sender::Sender()
    : data(""), MTU(128)
{}

//Constructor using only data
Sender::Sender(string ori_data)
    : data(ori_data), MTU(128)
{}

//Constructor using data and MTU
Sender::Sender(string ori_data, unsigned ori_MTU)
    : data(ori_data), MTU(ori_MTU)
{}

//getOriginData
string Sender::getOriginData() {
    return data;
}

//bitStuffing
string Sender::bitStuffing(string datum) {
    string jug("11111");
    if (datum.length() < 5) return datum;
    for (int i = 0; i <= int(datum.length()) - 5; ++i){
        if (datum.substr(i,5) == jug) datum.insert(i + 5, "0");
    }
    return datum;
}

//framing
string Sender::framing() {
    data = Sender::bitStuffing(data);
    string sign = "01111110";
    string res = sign;
    if (data == "") return res + sign;
    for (int i = 0; i < int(data.length()); i += MTU){
        res += data.substr(i,MTU);
        res += sign;
    }
    return res;
}
