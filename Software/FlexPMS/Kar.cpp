#include <sstream>
#include <cppconn/statement.h>
#include "Kar.h"


void Kar::set_mwstatus(bool mwstatus) {
    sql::Statement *stmt;
    std::stringstream q;
    int s;
    
    mwstatus_ = mwstatus;
    s = (mwstatus) ? 1 : 0;
    q << "UPDATE Kar SET mwstatus" << s << " WHERE id = " << id;
    
    stmt = db_conn_->createStatement();
    stmt->executeQuery(q.str());
}


bool Kar::get_mwstatus() {
    return mwstatus_;
}


void Kar::set_ovalvestatus(bool ovalvestatus) {
    sql::Statement *stmt;
    std::stringstream q;
    int s;
    
    ovalvestatus_ = ovalvestatus;
    s = (ovalvestatus) ? 1 : 0;
    q << "UPDATE Kar SET ovalvestatus" << s << " WHERE id = " << id;
    
    stmt = db_conn_->createStatement();
    stmt->executeQuery(q.str());
}


bool Kar::get_ovalvestatus() {
    return ivalvestatus_;
}


void Kar::set_ivalvestatus(bool ivalvestatus) {
    sql::Statement *stmt;
    std::stringstream q;
    int s;
    
    ivalvestatus_ = ivalvestatus;
    s = (ivalvestatus) ? 1 : 0;
    q << "UPDATE Kar SET ivalvestatus" << s << " WHERE id = " << id;
    
    stmt = db_conn_->createStatement();
    stmt->executeQuery(q.str());
}


bool Kar::get_ivalvestatus() {
    return ovalvestatus_;
}
