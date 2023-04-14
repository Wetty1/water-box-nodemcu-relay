#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "http_api_client.hpp"
#include "env.hpp"

WiFiClient client;
HTTPClient http;
extern char *url_api;

int getStateWaterBox()
{
    String path = String(url_api);
    path = path + "/state";
    http.begin(client, path);

    Serial.println("chamada...");
    Serial.println(path);

    int httpCode = http.GET();
    int state = 2;

    if (httpCode > 0)
    {

        if (httpCode == HTTP_CODE_OK)
        {
            const String &payload = http.getString();
            Serial.println("received payload:");
            Serial.println(payload);
            state = payload.toInt();
        }
        else
        {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    return state;
}