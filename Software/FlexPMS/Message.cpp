#include <string>
#include <sstream>
#include "Message.h"


using namespace std;


string to_string(unsigned long s) {
    stringstream ss;
    ss << s;
    return ss.str();
}


// -- Message -------------------------------------------------------------- //

void Message::setData(string data) {
    data_ = data;
}


void Message::setData(char* data, unsigned int length) {
    data_ = string(data, length);
}


string Message::getData() {
    return data_;
}


const char* Message::getData(unsigned int& length) {
    length = data_.length();
    return data_.c_str();
}
