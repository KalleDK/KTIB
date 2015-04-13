#include "KarContainer.h"


KarContainer::KarContainer(sql::Connection* db_conn) {
    db_conn_ = db_conn;
    reload();
}


KarContainer::~KarContainer() {
    Kar* kar;
    iter();
    while(kar = next())
        delete kar;
}


/*
 * Returns a Kar-object with KarID = id.
 * Returns a NULL-pointer if ID not found.
 */
Kar* KarContainer::get(unsigned int id) {
    if(contains(id))
        return map_[id];
    else
        return NULL;
}


/*
 * Check whether a specific Kar is in the container
 */
bool KarContainer::contains(unsigned int id) {
    return (map_.count(id)) ? true : false;
}


/*
 * Returns amount of Kar in the container
 */
const unsigned int KarContainer::size() {
    return map_.size();
}


/*
 * Load all from database and save to self
 */
void KarContainer::reload() {
    sql::Statement *stmt;
    sql::ResultSet *res;
    
    stmt = db_conn_->createStatement();
    res = stmt->executeQuery("SELECT * FROM Kar;");
    
    Kar* kar;
    
    while(res->next()) {
        kar = new Kar;
        kar->id = res->getInt("id");
        kar->name = res->getString("name");
        kar->address = res->getInt("address");
        kar->ph = res->getInt("ph");
        kar->volumen = res->getInt("id");
        kar->humidity = res->getInt("id");
        map_[kar->id] = kar;
    }
    
    delete res;
    delete stmt;
}


/*
 * Reset iteration
 */
void KarContainer::iter() {
    iterator_ = map_.begin();
}


/*
 * Get next Kar-object in iteration
 */
Kar* KarContainer::next() {
    if(iterator_ == map_.end())
        return NULL;
    
    Kar* kar = map_[iterator_->first];
    iterator_++;
    return kar;
}

