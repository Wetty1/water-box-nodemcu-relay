#include "controll_relay.hpp"
#include "http_api_client.hpp"
#include "global_variable.hpp"
#include "Arduino.h"

int state = -1;
extern bool isOn;
extern long unsigned countTimeToTurnOff;
extern long unsigned countTimeToNextTurnOn;
extern char *historyData;

#define timeToTurnOff 10800000    // 10800000 3h
#define timeToNextTurnOn 86000000 // 86000000 24h

void controllRelay()
{
    // no maximo 7200s - 720000ms
    // put your main code here, to run repeatedly:
    if (millis() % 5000 != 0)
        return;

    int responseState = getStateWaterBox();
    getCurrentLiters();
    bool signalToTurnOn = responseState != state && responseState == 1 && isOn == false;
    bool signalToTurnOff = responseState != state && responseState == 0;

    if (signalToTurnOn)
    {
        countTimeToTurnOff = millis();
        String newHistory = historyData;
        newHistory += "Ligado em: ";
        newHistory += getTimeNow();
        newHistory += "\n";
        newHistory.toCharArray(historyData, 24);
        registerEvent();
    }
    if (signalToTurnOff)
    {
        countTimeToTurnOff = -1;
        countTimeToNextTurnOn = millis();
        String newHistory = historyData;
        newHistory += "Desligado em: ";
        newHistory += getTimeNow();
        newHistory += "\n";
        newHistory.toCharArray(historyData, 24);
        registerEvent();
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