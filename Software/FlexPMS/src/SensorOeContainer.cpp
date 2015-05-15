#include "SensorOeContainer.h"


void SensorOeContainer::reload() {
    sql::Statement *stmt;
    sql::ResultSet *res;
    SensorOe* oe;
    
    stmt = db_conn_->createStatement();
    res = stmt->executeQuery("SELECT * FROM SensorOe;");
    
    while(res->next()) {
        oe = new SensorOe(db_conn_);
        oe->id = res->getInt("id");
        oe->kar_id = res->getInt("karid");
        oe->address = res->getInt("address");
        oe->valvestatus = res->getBoolean("valvestatus");
        
        map_[oe->address] = oe;
    }
    
    delete res;
    delete stmt;
}
