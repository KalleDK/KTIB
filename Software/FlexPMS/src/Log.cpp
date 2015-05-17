#include <iostream>
#include <sstream>
#include <time.h>
#include "Log.h"
#include "defines.h"


using namespace std;


Log* Log::instance_ = NULL;


Log* Log::getInstance() {
    if(Log::instance_ == NULL)
        Log::instance_ = new Log(LOG_FILE_PATH);
    return Log::instance_;
}


void Log::write(string line) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    stringstream ss;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, 80, "%F %H:%M:%S", timeinfo);
    ss << "[" << buffer << "] "<< line << endl;
    
    file_ << ss.str();
    file_.flush();
    
    cout << ss.str();
}


Log::Log(string filepath) {
    file_.open(filepath, ios::out | ios::app);
}


Log::~Log() {
    file_.close();
}
