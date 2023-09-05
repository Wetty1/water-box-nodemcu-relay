#include <Wifi_service.hpp>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <env.hpp>

const char *ssid = stassid;
const char *password = stapsk;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 100, 202);

void setupWifi()
{
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.mode(WIFI_STA);
    WiFi.config(apIP, IPAddress(192, 168, 100, 1), IPAddress(255, 255, 255, 0));
    WiFi.begin(ssid, password);

    Serial.println(ssid);

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
        digitalWrite(LED_BUILTIN, HIGH);
    }

    if (MDNS.begin("espotarelay", apIP))
    {
        Serial.println("MDNS responder started");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("WIFI Ready");
    digitalWrite(LED_BUILTIN, LOW);
}

void verifyWifi()
{
    MDNS.update();
    if (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Desconectado do WiFi. Tentando reconectar...");
        WiFi.disconnect();
        delay(1000);
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(1000);
        }
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Conectado novamente ao WiFi");
    }
}
