#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "http_api_client.hpp"
#include "global_variable.hpp"
#include "ArduinoJson.h"
#include "env.hpp"

WiFiClient client;
HTTPClient http;
extern char *url_local_sensor;
extern char *url_api;
extern bool isOn;
extern char *readLitarage;

const uint8_t fingerprint[20] = {0x5F, 0xF1, 0x0F, 0x2A, 0x27, 0x6C, 0xC0, 0x06, 0x1B, 0xBB, 0xF2, 0x61, 0x18, 0x70, 0xE0, 0xBA, 0xC9, 0x75, 0x6A, 0x8F};

int getStateWaterBox()
{
    String path = String(url_local_sensor);
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

char *getTimeNow()
{
    http.begin(client, "https://timeapi.io/api/Time/current/zone?timeZone=Brazil/East");

    Serial.println("chamada...");

    int httpCode = http.GET();
    String timestamp = "";
    char *res = "";

    if (httpCode > 0)
    {

        if (httpCode == HTTP_CODE_OK)
        {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, http.getString());

            timestamp = doc["dateTime"].as<String>();
            timestamp.toCharArray(res, 64);
        }
        else
        {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    return res;
}

void registerEvent()
{
    WiFiClientSecure clientHttps = WiFiClientSecure();
    clientHttps.setFingerprint(fingerprint);
    clientHttps.setTimeout(20000);

    Serial.print("[HTTP] begin...\n");

    int numAttempts = 0;
    bool sendRequest = false;

    while (numAttempts < 5 && !sendRequest)
    {
        if (http.begin(clientHttps, url_api))
        { // HTTP
            http.addHeader("Content-Type", "application/json");

            Serial.print("[HTTP] POST...\n");
            // start connection and send HTTP header and body
            int httpCode = http.POST("{\"literage\": " + String(readLitarage) + ", \"sinal\": " + String(isOn ? 1 : 0) + "}");

            // httpCode will be negative on error
            if (httpCode > 0)
            {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] POST... code: %d\n", httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK)
                {
                    sendRequest = true;
                    const String &payload = http.getString();
                    Serial.println("received payload:\n<<");
                    Serial.println(payload);
                    Serial.println(">>");
                }
            }
            else
            {
                Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
                numAttempts++;
            }
        }
        else
        {
            Serial.printf("[HTTP] Unable to connect\n");
            numAttempts++;
        }
        delay(3000);
    }

    http.end();
}

void getCurrentLiters()
{
    String path = String(url_local_sensor);
    path = path + "/level";
    http.setTimeout(60000);
    http.begin(client, path);

    Serial.println("chamada...");
    Serial.println(path);

    int httpCode = http.GET();
    float liters;

    if (httpCode > 0)
    {

        if (httpCode == HTTP_CODE_OK)
        {
            const String &payload = http.getString();
            Serial.println("received payload:");
            Serial.println(payload);
            payload.toCharArray(readLitarage, 8);
        }
        else
        {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}
