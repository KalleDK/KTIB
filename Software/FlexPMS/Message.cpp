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

string Message::getData() {
    return data_;
}

