#include "KarContainer.h"


void KarContainer::reload() {
    sql::Statement *stmt;
    sql::ResultSet *res;
    Kar* kar;
    
    stmt = db_conn_->createStatement();
    res = stmt->executeQuery("SELECT * FROM Kar;");
    
    while(res->next()) {
        kar = new Kar(db_conn_);
        kar->id = res->getInt("id");
        kar->name = res->getString("name");
        kar->address = res->getInt("address");
        kar->ph = res->getDouble("ph");
        kar->volumen = res->getUInt("volumen");
        kar->humidity = res->getUInt("humidity");
        
        kar->mwstatus = res->getBoolean("mwstatus");
        kar->ivalvestatus = res->getBoolean("ivalvestatus");
        kar->ovalvestatus = res->getBoolean("ovalvestatus");
        
        map_[kar->id] = kar;
    }
    
    delete res;
    delete stmt;
}
