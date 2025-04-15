#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include "configuration.hpp"

class OCDevice
{
    public:
        OCDevice();

        bool connected = false;
        String uniqueID = "";
        double avaragePeriod = 1.0;
        double dewPoint = 0.0;
        double humidity = 0.0;
        double pressure = 0.0;
        double rainRate = 0.0;
        double temperature = 0.0;
        double windDirection = 0.0;
        double windGust = 0.0;
        double windSpeed = 0.0;
        double timeSinceLastSync = 0.0;

        void update(String payload);
        
    private:
        
};
