#pragma once
#include <string>
#include <time.h>


typedef enum {
    ONLINE,
    OFFLINE,
} KarStatus;


class Kar {
public:
    unsigned int id;
    std::string name;
    unsigned int address;
    double ph;
    unsigned int volumen;
    unsigned int humidity;
    unsigned int unanswered_pings;
    time_t last_ping_sent;
    KarStatus status;
    
    Kar();
};

