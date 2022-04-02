#include "lights-controller.h"

using namespace std;

lightsController::lightsController()
{
    this->initMqttConnection();
}

lightsController::~lightsController()
{
}

bool lightsController::initMqttConnection()
{
    try {
        cout << "Init Mqtt Connection..." << endl;
        mqtt::client client = mqtt::client(SERVERURI, "2");
    }
    catch (mqtt::exception& e)
    {
        cout << "error code: " << e.get_reason_code() << endl;
        cout << "error message: " << e.what() << endl;
        return false;
    }
    return true;
}
