#pragma once
#include <map>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "Kar.h"


class KarContainer {
public:
    KarContainer(sql::Connection* db_conn);
    ~KarContainer();
    Kar* get(unsigned int id);
    bool contains(unsigned int id);
    const unsigned int size();
    void reload();
    void iter();
    Kar* next();
private:
    std::map<unsigned int, Kar*> map_;
    std::map<unsigned int, Kar*>::iterator iterator_;
    sql::Connection* db_conn_;
};
