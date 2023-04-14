#include "controll_relay.hpp"
#include "http_api_client.hpp"
#include "global_variable.hpp"
#include "Arduino.h"

int state = -1;
extern bool isOn;
extern long unsigned countTimeToTurnOff;
extern long unsigned countTimeToNextTurnOn;

#define timeToTurnOff 7200000     // 720000
#define timeToNextTurnOn 86000000 // 86000000

void controllRelay()
{
    // no maximo 7200s - 720000ms
    // put your main code here, to run repeatedly:
    if (millis() % 5000 != 0)
        return;

    int responseState = getStateWaterBox();
    bool signalToTurnOn = responseState != state && responseState == 1;
    bool signalToTurnOff = responseState != state && responseState == 0;

    if (signalToTurnOn)
    {
        countTimeToTurnOff = millis();
    }
    if (signalToTurnOff)
    {
        countTimeToTurnOff = -1;
        countTimeToNextTurnOn = millis();
    }

    Serial.println(responseState);
    Serial.println(millis() - countTimeToNextTurnOn);
    if (responseState == 1 && millis() - countTimeToNextTurnOn > timeToNextTurnOn)
    {
        state = responseState;
    }
    if (responseState != 1)
    {
        state = responseState;
    }

    bool isTimeout = millis() - countTimeToTurnOff > timeToTurnOff && isOn == true;
    if (isTimeout)
    {
        state = 0;
        countTimeToNextTurnOn = millis();
    }

    if (state != 2)
    {
        digitalWrite(D1, state == 0 ? LOW : HIGH);
        isOn = state == 0 ? false : true;
    }
    Serial.println(state);
}