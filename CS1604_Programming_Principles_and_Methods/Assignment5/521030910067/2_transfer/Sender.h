//
// Created by delirio on 2022/4/30.
//

#ifndef TRANSFER_SENDER_H
#define TRANSFER_SENDER_H
#include <string>
using namespace std;

class Sender {
// set members

public:
// design the constructor here
    Sender();
    Sender(string ori_data);
    Sender(string ori_data, unsigned int ori_MTU);
// some methods to be implemented
    string getOriginData();
    string bitStuffing (string datum);
    string framing();
private:
    string data;
    unsigned int MTU;
};

#endif //TRANSFER_SENDER_H
