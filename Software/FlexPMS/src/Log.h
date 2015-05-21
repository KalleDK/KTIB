#pragma once
#include <fstream>
#include <string>


class Log {
public:
    static Log* getInstance();
    void write(std::string line);
protected:
    Log(std::string filepath);
    ~Log();
private:
    std::ofstream file_;
    static Log* instance_;
};
