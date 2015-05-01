#include <string>
#include <cppconn/prepared_statement.h>
#include "SensorOe.h"


using namespace std;


void SensorOe::set_valvestatus(bool s) {
    sql::PreparedStatement *pstmt;
    std::string query;

    valvestatus = s;
    query = "UPDATE SensorOe SET valvestatus = ? WHERE id = ?";
    pstmt = db_conn_->prepareStatement(query);
    pstmt->setBoolean(1, s);
    pstmt->setInt(2, id);
    pstmt->executeUpdate();
    
    delete pstmt;
}
