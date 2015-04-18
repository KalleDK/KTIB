#include <string>
#include <sstream>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Kar.h"

using namespace std;


void Kar::set_mwstatus(bool mwstatus) {
    sql::PreparedStatement *pstmt;
    std::string query;

    mwstatus_ = mwstatus;
    query = "UPDATE Kar SET mwstatus = ? WHERE id = ?";
    pstmt = db_conn_->prepareStatement(query);
    pstmt->setBoolean(1, mwstatus);
    pstmt->setInt(2, id);
    pstmt->executeUpdate();
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
    q << "UPDATE Kar SET ovalvestatus = " << s << " WHERE id = " << id << ";";
    
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
    q << "UPDATE Kar SET ivalvestatus = " << s << " WHERE id = " << id << ";";
    
    stmt = db_conn_->createStatement();
    stmt->executeQuery(q.str());
}


bool Kar::get_ivalvestatus() {
    return ovalvestatus_;
}
