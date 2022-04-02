#pragma once
#include "mqtt/client.h"
#include "mqtt/exception.h"

class lightsController
{

public:
    lightsController();
    ~lightsController();

private:
    const std::string SERVERURI = "192.168.0.1";

    bool initMqttConnection();

};