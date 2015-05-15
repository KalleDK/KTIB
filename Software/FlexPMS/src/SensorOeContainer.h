#pragma once
#include <cppconn/driver.h>
#include "SensorOe.h"
#include "DBContainer.h"


class SensorOeContainer : public DBContainer<SensorOe> {
public:
    SensorOeContainer(sql::Connection* db_conn) : DBContainer(db_conn) {};
    void reload();
};
