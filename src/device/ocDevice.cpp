#include "device\ocDevice.h"
#include <ArduinoJson.h>

OCDevice::OCDevice()
{

}

void OCDevice::update(String payload)
{
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    temperature = doc["outdoor_temperature"];
    humidity = doc["outdoor_humidity"];
    dewPoint = doc["dew_point"];
    windSpeed = doc["wind_speed"];
    windGust = doc["gust_speed"];
    //double day_max_wind = doc["day_max_wind"];
    //double light_lux = doc["light_lux"];
    //double uvi = doc["uvi"];
    windDirection = doc["wind_direction"];
    //double rain_event = doc["rain_event"];
    rainRate = doc["rain_rate"];
    //double rain_day = doc["rain_day"];
    //double rain_week = doc["rain_week"];
    //double rain_month = doc["rain_month"];
    //double rain_year = doc["rain_year"];
    //double intemp = doc["intemp"];
    //double inhumi = doc["inhumi"];
    pressure = doc["pressure_abs"];
    //double pressure_rel = doc["pressure_rel"];

    /*
    Serial.println(temperature, 1);
    Serial.println(humidity, 1);
    Serial.println(dewPoint, 1);
    Serial.println(windSpeed, 1);
    Serial.println(windGust, 1);
    
    //Serial.println(light_lux, 1);
    Serial.println(windDirection, 1);
    //Serial.println(uvi, 1);
    //Serial.println(rain_event, 1);
    Serial.println(rainRate, 1);
    //Serial.println(rain_day, 1);
    //Serial.println(rain_week, 1);
    //Serial.println(rain_month, 1);
    //Serial.println(rain_year, 1);

    //Serial.println(intemp, 1);
    //Serial.println(inhumi, 1);
    Serial.println(pressure, 1);
    //Serial.println(pressure_rel, 1);
    */

}