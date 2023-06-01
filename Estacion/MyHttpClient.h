#ifndef MYHTTPCLIENT_H
#define MYHTTPCLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class MyHttpClient {
public:
  static void postData(const String& url, const String& apikey, int& id, float accX, float accY, float accZ, String Date, double lati, double longi, double alti);
};

#endif  // MYHTTPCLIENT_H
