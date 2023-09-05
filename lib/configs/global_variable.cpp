#include "Arduino.h"

bool isOn = false;
long unsigned countTimeToTurnOff = millis();
long unsigned countTimeToNextTurnOn = -86000000;
char *historyData;
char *readLitarage;