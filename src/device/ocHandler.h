#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>
#include <HTTPUpdateServer.h>
#include <ArduinoJson.h>
#include "configuration.hpp"
#include "OCDevice.h"

class OCHandler
{
    WebServer* _server;

    public:
        OCHandler(WebServer* server);
        OCDevice* ocDevice;
        
        void handlerMgmtVersions();
        void handlerMgmtDescription();
        void handlerMgmtConfiguredDevices();

        void handlerConnected();
        void handlerDescriptionGet();
        void handlerDriverInfoGet();
        void handlerDriverVersionGet();
        void handlerInterfaceVersionGet();
        void handlerNameGet();
        void handlerSupportedActionsGet();

        void handleAction();
        void handleCommandBlind();
        void handleCommandBool();
        void handleCommandString();

        void handlerDriver0AtHome();

        void handleDriver0AvaragePeriod();
        void handleDriver0CloudCover();
        void handleDriver0DewPoint();
        void handleDriver0CHumidity();
        void handleDriver0Pressure();
        void handleDriver0RainRate();
        void handleDriver0SkyBrightness();
        void handleDriver0SkyQuality();
        void handleDriver0SkyTemperature();
        void handleDriver0StarFWHM();
        void handleDriver0Temperature();
        void handleDriver0WindDirection();
        void handleDriver0WindGust();
        void handleDriver0WindSpeed();
        void handleDriver0Refresh();
        void handleDriver0SensorDescription();
        void handleDriver0TimeSinceLastUpdate();

    
    private:
        uint32_t transID;
        uint32_t clientID;
        uint32_t serverTransactionID = 0;
        String uniqueID = UNIQUE_CLIENT_ID;

        void incrementServerTransID();

        void returnEmpty(String errMsg, int errNr);

        void returnStringValue(String val, String errMsg, int errNr);
        void returnBoolValue(bool val, String errMsg, int errNr);
        void returnIntValue(int val, String errMsg, int errNr);
        void returnFloatValue(float val, String errMsg, int errNr);
        void returnJsonArrayValue(JsonArray val, String errMsg, int errNr);
        void returnDoubleValue(double val, String errMsg, int errNr);

        void debugServerQuery();
        
};
