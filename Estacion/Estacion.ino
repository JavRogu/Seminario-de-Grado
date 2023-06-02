#include "aceleracion.h"
#include <WiFi.h>
#include "wifi_helper.h"
#include "api_rest_esp32.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include "MyHttpClient.h"
#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "SPIFFS.h"

String url = "https://hyfdlfwcammsfnrexcqe.supabase.co/rest/v1/Datos";
String apikey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Imh5ZmRsZndjYW1tc2ZucmV4Y3FlIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODU0MTA3NDMsImV4cCI6MjAwMDk4Njc0M30.t-dpApRpyn7CVXrr1ktWHf3lkT0fGzBrQK_uLcV3nrc";
int id = 2;

AsyncWebServer server(80);

TinyGPSPlus gps;
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);

char datoCmd = 0;



unsigned long previousTime = 0;

double latitud = 0.0;
double longitud = 0.0;
double altitud = 0.0;
int dia = 0;
int mes = 0;
int ano = 0;
int hora = 0;
int minuto = 0;
int segundo = 0;
String DateTime;

const char* ssid = "Redmi Note 8";
const char* password = "jav199806";

// Pin analógico al que está conectado el acelerómetro
const int PIN_X = 34;
const int PIN_Y = 35;
const int PIN_Z = 32;

// Sensibilidad del acelerómetro según el rango
const float sensibilidad = 1000.0; // 1000mV/g para el rango de +-2g
//const float sensibilidad = 333.0; // 333mV/g para el rango de +-6g

// Rango máximo de la conversión analógica a digital (12 bits)
const int escala = 4095;

//Funcion encargada de generar el JSON de respuesta para el servidor web
String readAccelerations() {
  
  // Obtener los valores de aceleración en los ejes X, Y y Z
  float accelerationX = calcularAceleracion(PIN_X, sensibilidad, escala);
  float accelerationY = calcularAceleracion(PIN_Y, sensibilidad, escala);
  float accelerationZ = calcularAceleracion(PIN_Z, sensibilidad, escala);
  latitud = gps.location.lat();
  longitud = gps.location.lng();
  altitud = gps.altitude.meters();
  String fechaHora = String(gps.date.year()) + "-" + twoDigits(gps.date.month()) + 
                            "-" + twoDigits(gps.date.day()) + "T" + twoDigits(gps.time.hour()) + 
                            ":" + twoDigits(gps.time.minute()) + ":" + twoDigits(gps.time.second()) + "Z";

  // Crear un objeto JSON con los datos de aceleración
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["acceleracionX"] = accelerationX;
  jsonDoc["acceleracionY"] = accelerationY;
  jsonDoc["acceleracionZ"] = accelerationZ;
  jsonDoc["latitude"] = latitud;
  jsonDoc["longitude"] = longitud;
  jsonDoc["altitude"] = altitud;
  //jsonDoc["date"] = fecha;
  jsonDoc["time"] = fechaHora;
  

  // Convertir el objeto JSON a una cadena
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  return jsonString;
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting GPS");
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  conectarWiFi(ssid, password);
  
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  

  Serial.println("Starting Webserver");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/accelerations", HTTP_GET, [](AsyncWebServerRequest *request) {
  request->send(200, "application/json", readAccelerations());
});

  // Start server
  server.begin();

}

void loop() {

  // Leer el valor de aceleración
  float aceleracionX = calcularAceleracion(PIN_X, sensibilidad, escala);
  float aceleracionY = calcularAceleracion(PIN_Y, sensibilidad, escala);
  float aceleracionZ = calcularAceleracion(PIN_Z, sensibilidad, escala);
  Serial.println(aceleracionX);
  Serial.println(aceleracionY);
  Serial.println(aceleracionZ);

   if (neogps.available())
  {
    while (neogps.available())
    {
      datoCmd = (char)neogps.read();
      Serial.print(datoCmd);
    }
  }
  else
  {
    boolean newData = false;
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
      while (neogps.available())
      {
        if (gps.encode(neogps.read()))
        {
          newData = true;
        }
      }
    }

    if (newData == true)
    {
      newData = false;
      if (gps.location.isValid() == 1)
      {
        Serial.print("Latitud: "); latitud = gps.location.lat();
        Serial.println(latitud, 6);
        Serial.print("Longitud: "); longitud = gps.location.lng();
        Serial.println(longitud, 6);
        Serial.print("Altitud: "); altitud = gps.location.lng();
        Serial.println(altitud, 0);
        Serial.print("Satélites: "); 
        Serial.println(gps.satellites.value());

        // Generar string con formato ISO 8601 para fecha y hora
        DateTime = String(gps.date.year()) + "-" + twoDigits(gps.date.month()) + 
                          "-" + twoDigits(gps.date.day()) + "T" + twoDigits(gps.time.hour()) + 
                          ":" + twoDigits(gps.time.minute()) + ":" + twoDigits(gps.time.second()) + "Z";
        Serial.print("Fecha y hora: ");
        Serial.println(DateTime);

        Serial.println("---------------------------");
      }
      else
      {
        Serial.println("Sin señal GPS");
      }
    }
  }

  unsigned long currentTime = millis();
  if ((WiFi.status()== WL_CONNECTED)) {
    
    if (currentTime - previousTime >= 5000) {
      
      float aceleracionX = calcularAceleracion(PIN_X, sensibilidad, escala);
      float aceleracionY = calcularAceleracion(PIN_Y, sensibilidad, escala);
      float aceleracionZ = calcularAceleracion(PIN_Z, sensibilidad, escala);

      if (gps.location.isValid() == 1)
      {
      DateTime = String(gps.date.year()) + "-" + twoDigits(gps.date.month()) + "-" + twoDigits(gps.date.day()) +
                             "T" + twoDigits(gps.time.hour()) + ":" + twoDigits(gps.time.minute()) + ":" + twoDigits(gps.time.second()) + "Z";
      }else{
        DateTime = "Sin señal GPS";
      }
      MyHttpClient::postData(url, apikey, id, aceleracionX, aceleracionY, aceleracionZ, DateTime, latitud, longitud, altitud);
      
      // Actualiza el tiempo anterior al tiempo actual
      previousTime = currentTime;
      }
     }
     else{
      Serial.println("Conexión perdida");
     }
}
// Función auxiliar para formatear dígitos a dos dígitos con ceros iniciales
String twoDigits(int number)
{
  if (number < 10)
  {
    return "0" + String(number);
  }
  return String(number);
}
