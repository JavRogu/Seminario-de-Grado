#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <WiFi.h>

void conectarWiFi(const char* ssid, const char* password)
{
  WiFi.begin(ssid, password);

  Serial.print("Conectando a ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

#endif
