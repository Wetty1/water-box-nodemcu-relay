#include <http_server.hpp>
#include <Arduino.h>
#include <global_variable.hpp>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>

ESP8266WebServer server(80);

extern bool isOn;
extern long unsigned countTimeToTurnOff;
extern long unsigned countTimeToNextTurnOn;

void handleRoot()
{
    String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Informações do dispositivo</title></head><body>";
    html += "<h1>Informações do dispositivo</h1>";

    // Informações a serem exibidas na página

    int proximaLigacao = +(millis() - countTimeToNextTurnOn) / 60000 - 1433;
    int forcaSinal = WiFi.RSSI();
    int tempoDesligar = +(millis() - countTimeToTurnOff) / 60000 - 120;

    html += "<p>Estado: ";
    if (isOn)
    {
        html += "<span style=\"color:green\">Ligado</span>";
        html += "</p>";
        html += "<p>Força do sinal Wi-Fi: " + String(forcaSinal) + " dBm</p>";
        html += "<p>Tempo para desligar: " + String(tempoDesligar) + " segundos</p>";
    }
    else
    {
        html += "<span style=\"color:red\">Desligado</span>";
        html += "<p>Tempo para próxima ligação: " + String(proximaLigacao) + "  minutos </p>";
    }
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setupHttpServer()
{
    // CONFIG WEB SERVER
    if (MDNS.begin("esp8266"))
    {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loopHttpServer()
{
    server.handleClient();
    MDNS.update();
}
