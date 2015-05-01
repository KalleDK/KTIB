#pragma once
#include <map>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


template <typename T>
class DBContainer {
public:
    DBContainer(sql::Connection* db_conn) : db_conn_(db_conn) {};
    
    ~DBContainer() {
        T* item;
        iter();
        while(item = next())
            delete item;
    };
    
    T* get(unsigned int id) {
        if(contains(id))
            return map_[id];
        else
            return NULL;
    };
    
    bool contains(unsigned int id) {
        return (map_.count(id)) ? true : false;
    };
    
    const unsigned int size() {
        return map_.size();
    };
    
    void iter() {
        iterator_ = map_.begin();
    };
    
    T* next() {
        if(iterator_ == map_.end())
            return NULL;
        
        T* item = map_[iterator_->first];
        iterator_++;
        return item;
    };
    
    virtual void reload() = 0;
protected:
    typename std::map<unsigned int, T*> map_;
    typename std::map<unsigned int, T*>::iterator iterator_;
    sql::Connection* db_conn_;
};
