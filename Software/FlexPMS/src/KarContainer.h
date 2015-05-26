#pragma once
#include <cppconn/driver.h>
#include "Kar.h"
#include "DBContainer.h"


class KarContainer : public DBContainer<Kar> {
public:
    KarContainer(sql::Connection* db_conn) : DBContainer(db_conn) {};
    void reload();
};
