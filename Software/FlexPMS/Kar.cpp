#include <string>
#include <sstream>
#include <cppconn/prepared_statement.h>
#include "Kar.h"

using namespace std;


void Kar::set_mwstatus(bool s) {
    sql::PreparedStatement *pstmt;
    std::string query;

    mwstatus = s;
    query = "UPDATE Kar SET mwstatus = ? WHERE id = ?";
    pstmt = db_conn_->prepareStatement(query);
    pstmt->setBoolean(1, s);
    pstmt->setInt(2, id);
    pstmt->executeUpdate();
    
    delete pstmt;
}


void Kar::set_ivalvestatus(bool s) {
    sql::PreparedStatement *pstmt;
    std::string query;

    ivalvestatus = s;
    query = "UPDATE Kar SET ivalvestatus = ? WHERE id = ?";
    pstmt = db_conn_->prepareStatement(query);
    pstmt->setBoolean(1, s);
    pstmt->setInt(2, id);
    pstmt->executeUpdate();
    
    delete pstmt;
}


void Kar::set_ovalvestatus(bool s) {
    sql::PreparedStatement *pstmt;
    std::string query;

    ovalvestatus = s;
    query = "UPDATE Kar SET ovalvestatus = ? WHERE id = ?";
    pstmt = db_conn_->prepareStatement(query);
    pstmt->setBoolean(1, s);
    pstmt->setInt(2, id);
    pstmt->executeUpdate();
    
    delete pstmt;
}
