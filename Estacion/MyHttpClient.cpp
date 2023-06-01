#include "MyHttpClient.h"

void MyHttpClient::postData(const String& url, const String& apikey, int& id,float accX, float accY, float accZ, String Date, double lati, double longi, double alti) {
  HTTPClient client;

  client.begin(url);
  client.addHeader("apikey", apikey);
  client.addHeader("Content-Type", "application/json");

  // Crear el JSON con los datos
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["id"] = id;
  jsonDoc["X"] = accX;
  jsonDoc["Y"] = accY;
  jsonDoc["Z"] = accZ;
  jsonDoc["LAT"] = lati;
  jsonDoc["LON"] = longi;
  jsonDoc["ALT"] = alti;
  //jsonDoc["date"] = fecha;
  jsonDoc["Date"] = Date;
  
  // Convertir el objeto JSON a una cadena
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  id = id + 1;

  int httpCode = client.POST(jsonString);

  if (httpCode > 0) {
    String payload = client.getString();
    Serial.println("\nStatuscode: " + String(httpCode));
    Serial.println(payload);

    client.end();
  }
  else {
    Serial.println("Error en la solicitud http");
  }
}
