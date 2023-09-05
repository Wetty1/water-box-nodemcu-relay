#include <http_server.hpp>
#include <Arduino.h>
#include <global_variable.hpp>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

ESP8266WebServer server(80);

extern bool isOn;
extern long unsigned countTimeToTurnOff;
extern long unsigned countTimeToNextTurnOn;
extern char *readLitarage;

void handleRoot()
{
    String html = "<!DOCTYPE html><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>Informações do dispositivo</title></head><body>";
    html += "<h1>Informações do dispositivo</h1>";

    // Informações a serem exibidas na página

    int proximaLigacao = (86000000 / 60000) - (millis() - countTimeToNextTurnOn) / 60000;
    int forcaSinal = WiFi.RSSI();
    int tempoDesligar = (10800000 / 60000) - (millis() - countTimeToTurnOff) / 60000;

    html += "<p>Estado: ";
    if (isOn)
    {
        html += "<span style=\"color:green\">Ligado</span>";
        html += "</p>";
        html += "<p>Tempo para desligar: " + String(tempoDesligar) + " minutos</p>";
    }
    else
    {
        html += "<span style=\"color:red\">Desligado</span>";
        html += "<p>Tempo para próxima ligação: " + String(proximaLigacao) + "  minutos </p>";
    }
    html += "<p>Força do sinal Wi-Fi: " + String(forcaSinal) + " dBm</p>";
    html += "<p>" + String(readLitarage) + " Litros</p>";
    html += "<p>" + String(historyData) + "</p>";

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
    server.on("/", handleRoot);

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loopHttpServer()
{
    server.handleClient();
}
