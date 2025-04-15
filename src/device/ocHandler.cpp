#include "device\ocHandler.h"


OCHandler::OCHandler(WebServer *server)
{
    _server = server;
    ocDevice = new OCDevice();
    serverTransactionID = 0;
}

void OCHandler::debugServerQuery()
{
    for (int i = 0; i < _server->args(); i++) {
        Log.traceln("%d" CR, i);
        Log.traceln("%s" CR, _server->argName(i));
        Log.traceln("%s" CR, _server->arg(i));
        Log.traceln("--------------------------");
    }
}

void OCHandler::incrementServerTransID()
{
    serverTransactionID++;
}

void OCHandler::returnEmpty(String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}

void OCHandler::returnBoolValue(bool val, String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["Value"] = val;
    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}

void OCHandler::returnStringValue(String val, String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["Value"] = val;
    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}

void OCHandler::returnIntValue(int val, String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["Value"] = val;
    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}

void OCHandler::returnFloatValue(float val, String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["Value"] = val;
    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}

void OCHandler::returnJsonArrayValue(JsonArray val, String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["Value"] = val;
    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}

void OCHandler::returnDoubleValue(double val, String errMsg, int errNr)
{
    incrementServerTransID();
    DynamicJsonDocument doc(1024);

    doc["Value"] = val;
    doc["ErrorMessage"] = errMsg;
    doc["ErrorNumber"] = errNr;
    doc["ClientTransactionID"] = transID;
    doc["ServerTransactionID"] = serverTransactionID;

    String output;
    serializeJson(doc, output);
    Log.traceln(F("Returning: %s" CR), output.c_str());

    _server->send(200, "application/json", output);
}
/************************************
 * ALPACA DEVICE MANAGEMENT
**********************************/

void OCHandler::handlerMgmtVersions()
{
    Log.traceln("handlerMgmtVersions called...");
    incrementServerTransID();

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    if (_server->method() == HTTP_GET)
    {
        DynamicJsonDocument doc(1024);

        const size_t CAPACITY = JSON_ARRAY_SIZE(1);
        StaticJsonDocument<CAPACITY> devices;
        JsonArray array = devices.to<JsonArray>();
        array.add(1);
        // returnJsonArrayValue(array);
        
        doc["Value"] = array;
        doc["ErrorMessage"] = "";
        doc["ErrorNumber"] = 0;
        doc["ClientTransactionID"] = transID;
        doc["ServerTransactionID"] = serverTransactionID;

        String output;
        serializeJson(doc, output);
        Log.traceln(F("Returning: %s" CR), output.c_str());
        // return output;
        _server->send(200, "application/json", output);
        // sendDeviceSetup( returnCode, message, err );
        
    }
}

void OCHandler::handlerMgmtDescription()
{
    Log.traceln("handlerMgmtDescription called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    if (_server->method() == HTTP_GET)
    {
        incrementServerTransID();
        DynamicJsonDocument val(1024);

        val["ServerName"] = SERVER_NAME; //"TCN Mount Switch";
        val["Manufacturer"] = MANUFACTURER; // "Christian Kardach";
        val["ManufacturerVersion"] = MANUFACTURER_VERSION; //"1.0";
        val["Location"] = LOCATION; //"SE";

        const size_t CAPACITY = JSON_ARRAY_SIZE(5);
        StaticJsonDocument<CAPACITY> devices;
        JsonArray array = devices.to<JsonArray>();
        array.add(val);

        //returnJsonArrayValue(array);
        
        DynamicJsonDocument doc(1024);
        doc["Value"] = val;
        doc["ErrorMessage"] = "";
        doc["ErrorNumber"] = 0;
        doc["ClientTransactionID"] = transID;
        doc["ServerTransactionID"] = serverTransactionID;

        String output;
        serializeJson(doc, output);
        Log.traceln(F("Returning: %s" CR), output.c_str());
        _server->send(200, "application/json", output);
        
    }
}

void OCHandler::handlerMgmtConfiguredDevices()
{
    Log.traceln("handlerMgmtConfiguredDevices called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    if (_server->method() == HTTP_GET)
    {
        incrementServerTransID();
        DynamicJsonDocument val(1024);
        DynamicJsonDocument doc(1024);

        const size_t CAPACITY = JSON_ARRAY_SIZE(5);
        StaticJsonDocument<CAPACITY> devices;

        val["DeviceName"] = DEVICE_NAME; //"TCN Mount Switch";
        val["DeviceType"] = DEVICE_TYPE; //"Switch";
        val["DeviceNumber"] = DEVICE_NR; //0;
        val["UniqueID"] = UNIQUE_CLIENT_ID;
        JsonArray array = devices.to<JsonArray>();
        array.add(val);

        // returnJsonArrayValue(array);
        
        doc["Value"] = array;
        doc["ErrorMessage"] = "";
        doc["ErrorNumber"] = 0;
        doc["ClientTransactionID"] = transID;
        doc["ServerTransactionID"] = serverTransactionID;

        String output;
        serializeJson(doc, output);
        Log.traceln(F("Returning: %s" CR), output.c_str());

        _server->send(200, "application/json", output);
        
    }
}

void OCHandler::handlerConnected()
{
    Log.traceln("handlerConnected called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    if (_server->method() == HTTP_GET)
    {
        Log.traceln("handlerConnected GET called");
        returnBoolValue(ocDevice->connected, "", 0);
    }

    if (_server->method() == HTTP_PUT)
    {
        Log.traceln("handlerConnected PUT called");

        if(_server->arg("Connected") == "False")
        {
            ocDevice->connected = false;
        }
        else if(_server->arg("Connected") == "True")
        {
            ocDevice->connected = true;
        }
        returnEmpty("", 0);
    }
}

/***********************************
 * ASCOM STANDARD
 **********************************/
void OCHandler::handlerDescriptionGet()
{
    Log.traceln("handlerDescriptionGet called");
    returnStringValue(DESCRIPTION, "", 0);
}

void OCHandler::handlerDriverInfoGet()
{
    Log.traceln("handlerDriverInfoGet called");
    returnStringValue(DRIVER_INFO, "", 0);
}

void OCHandler::handlerDriverVersionGet()
{
    Log.traceln("handlerDriverVersionGet called");
    returnStringValue(DRIVER_VERSION, "", 0);
}

void OCHandler::handlerInterfaceVersionGet()
{
    Log.traceln("handlerInterfaceVersionGet called");
    returnIntValue(1, "", 0);
}

void OCHandler::handlerNameGet()
{
    Log.traceln("handlerNameGet called");
    returnStringValue(DEVICE_NAME, "", 0);
}

void OCHandler::handlerSupportedActionsGet()
{
    Log.traceln("handlerSupportedActionsGet called");

    DynamicJsonDocument val(1024);
    const size_t CAPACITY = JSON_ARRAY_SIZE(13);
    StaticJsonDocument<CAPACITY> devices;
    JsonArray array = devices.to<JsonArray>();
    
    array.add("action");
    array.add("commandblind");
    array.add("commandbool");
    array.add("commandstring");
    /*
    array.add("maxswitch");
    array.add("canwrite");
    array.add("getswitchdescription");
    array.add("getswitch");
    array.add("getswitchname");
    array.add("getswitchvalue");
    array.add("minswitchvalue");
    array.add("maxswitchvalue");
    array.add("switchstep");
    */
    returnJsonArrayValue(array, "", 0);
    
}

void OCHandler::handleAction()
{
    Log.traceln("handleAction called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    String deviceType = _server->arg("device_type");
    //u_int32_t deviceNumber = (uint32_t)_server->arg("device_number").toInt();

    String action = _server->arg("Action");
    String parameters = _server->arg("Parameters");

    returnStringValue("ok", "", 0);
}

void OCHandler::handleCommandBlind()
{
    Log.traceln("handleCommandBlind called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    String deviceType = _server->arg("device_type");
    // u_int32_t deviceNumber = (uint32_t)_server->arg("device_number").toInt();

    String command = _server->arg("Command");
    String raw = _server->arg("Raw");

    Log.traceln("%s" CR, command);
    Log.traceln("%s" CR, raw);

    returnEmpty("", 0);
}

void OCHandler::handleCommandBool()
{
    Log.traceln("handleCommandBool called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    String deviceType = _server->arg("device_type");
    // u_int32_t deviceNumber = (uint32_t)_server->arg("device_number").toInt();

    String command = _server->arg("Command");
    String raw = _server->arg("Raw");

    Log.traceln("%s" CR, command);
    Log.traceln("%s" CR, raw);

    returnBoolValue(true, "", 0);
}

void OCHandler::handleCommandString()
{
    Log.traceln("handleCommandBool called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    String deviceType = _server->arg("device_type");
    // u_int32_t deviceNumber = (uint32_t)_server->arg("device_number").toInt();

    String command = _server->arg("Command");
    String raw = _server->arg("Raw");

    Log.traceln("%s" CR, command.c_str());
    Log.traceln("%s" CR, raw.c_str());

    returnStringValue("ok", "", 0);
}

/***********************************
 * ASCOM DOME
 **********************************/

void OCHandler::handleDriver0AvaragePeriod()
{
    Log.traceln("handleDriver0AvaragePeriod called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    if (_server->method() == HTTP_GET)
    {
        Log.traceln("GET handleDriver0AvaragePeriod called");

        returnDoubleValue(ocDevice->avaragePeriod, "", 0);
    }
    else if (_server->method() == HTTP_PUT)
    {
        Log.traceln("PUT handleDriver0AvaragePeriod called");
        double val = _server->arg("AveragePeriod").toDouble();
        if(val < 0.0)
        {
            returnEmpty("", 1025);
        }
        else {
            ocDevice->avaragePeriod = val;
            returnEmpty("", 0);
        }
    }
}

void OCHandler::handleDriver0CloudCover()
{
    Log.traceln("handleDriver0CloudCover called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
   
    returnEmpty("Not Implemented", 1024);
}

void OCHandler::handleDriver0DewPoint()
{
    Log.traceln("handleDriver0DewPoint called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnDoubleValue(ocDevice->dewPoint, "", 0);
}

void OCHandler::handleDriver0CHumidity()
{
    Log.traceln("handleDriver0CHumidity called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnDoubleValue(ocDevice->humidity, "", 0);
}

void OCHandler::handleDriver0Pressure()
{
    Log.traceln("handleDriver0Pressure called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnDoubleValue(ocDevice->pressure, "", 0);
}

void OCHandler::handleDriver0RainRate()
{
    Log.traceln("handleDriver0RainRate called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnDoubleValue(ocDevice->rainRate, "", 0);
}

void OCHandler::handleDriver0SkyBrightness()
{
    Log.traceln("handleDriver0SkyBrightness called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnEmpty("Not Implemented", 1024);
}

void OCHandler::handleDriver0SkyQuality()
{
    Log.traceln("handleDriver0SkyQuality called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnEmpty("Not Implemented", 1024);
}


void OCHandler::handleDriver0SkyTemperature()
{
    Log.traceln("handleDriver0SkyTemperature called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnEmpty("Not Implemented", 1024);
}

void OCHandler::handleDriver0StarFWHM()
{
    Log.traceln("handleDriver0StarFWHM called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    returnEmpty("Not Implemented", 1024);
}

void OCHandler::handleDriver0Temperature() 
{
    Log.traceln("handleDriver0Temperature called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnDoubleValue(ocDevice->temperature, "", 0);
}

void OCHandler::handleDriver0WindDirection() 
{
    Log.traceln("handleDriver0AtHome called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    
    returnDoubleValue(ocDevice->windDirection, "", 0);
}

void OCHandler::handleDriver0WindGust() 
{
    Log.traceln("handleDriver0WindGust called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    returnDoubleValue(ocDevice->windGust, "", 0);
}

void OCHandler::handleDriver0WindSpeed() {
    // 0 = Open, 1 = Closed, 2 = Opening, 3 = Closing, 4 = Shutter status error

     Log.traceln("handleDriver0WindSpeed called");
    
    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    returnDoubleValue(ocDevice->windSpeed, "", 0);
}

void OCHandler::handleDriver0Refresh() 
{

    Log.traceln("handleDriver0Refresh called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    returnEmpty("", 0);
}

void OCHandler::handleDriver0SensorDescription() 
{
    debugServerQuery();
    Log.traceln("handleDriver0SensorDescription called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();

    String sensorName = _server->arg("SensorName");

    if(sensorName == "CloudCover")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "DewPoint")
    {
        returnStringValue("ECOWITT GW1100A - DewPoint", "", 0);
    }
    else if(sensorName == "Humidity")
    {
        returnStringValue("ECOWITT GW1100A - Humidity", "", 0);
    }
    else if(sensorName == "Pressure")
    {
        returnStringValue("ECOWITT GW1100A - Pressure", "", 0);
    }
    else if(sensorName == "RainRate")
    {
        returnStringValue("ECOWITT GW1100A - RainRate", "", 0);
    }
    else if(sensorName == "SkyBrightness")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "SkyQuality")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "StarFWHM")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "Temperature")
    {
        returnStringValue("ECOWITT GW1100A - Temperature", "", 0);
    }
    else if(sensorName == "WindDirection")
    {
        returnStringValue("ECOWITT GW1100A - WindDirection", "", 0);
    }
    else if(sensorName == "WindGust")
    {
        returnStringValue("ECOWITT GW1100A - WindGust", "", 0);
    }
    else if(sensorName == "WindSpeed")
    {
        returnStringValue("ECOWITT GW1100A - WindSpeed", "", 0);
    }
    else {
        returnEmpty("Not Implemented", 1024);
    }
    /*
    CloudCover
    DewPoint
    Humidity
    Pressure
    RainRate
    SkyBrightness
    SkyQuality
    StarFWHM
    Temperature
    WindDirection
    WindGust
    WindSpeed
    */
}

void OCHandler::handleDriver0TimeSinceLastUpdate() 
{
    debugServerQuery();
    Log.traceln("handleDriver0TimeSinceLastUpdate called");

    clientID = (uint32_t)_server->arg("ClientID").toInt();
    transID = (uint32_t)_server->arg("ClientTransactionID").toInt();
    String sensorName = _server->arg("SensorName");

    if(sensorName == "CloudCover")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "DewPoint")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "Humidity")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "Pressure")
    {
       returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "RainRate")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "SkyBrightness")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "SkyTemperature")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "SkyQuality")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "StarFWHM")
    {
        returnEmpty("Not Implemented", 1024);
    }
    else if(sensorName == "Temperature")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "WindDirection")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "WindGust")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else if(sensorName == "WindSpeed")
    {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }
    else {
        returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
    }

    //returnDoubleValue(ocDevice->timeSinceLastSync, "", 0);
}
