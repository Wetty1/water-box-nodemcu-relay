#include <Arduino.h>
#include <http_api_client.hpp>
#include <http_server.hpp>
#include <Wifi_service.hpp>
#include <ota_service.hpp>
#include <controll_relay.hpp>

void setup()
{
  Serial.begin(115200);
  Serial.printf("Starting");

  setupWifi();
  // put your setup code here, to run once:
  pinMode(D1, OUTPUT);
  setupHttpServer();
  setupOtaService();
}

void loop()
{
  verifyWifi();
  controllRelay();
  loopHttpServer();
  otaLoopHandle();
}