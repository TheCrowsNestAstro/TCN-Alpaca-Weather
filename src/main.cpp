
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPUpdateServer.h>
#include <HTTPClient.h>
#include <arduino-timer.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>

#include "arduino_secrets.h"
#include "configuration.hpp"

#include "device\ocHandler.h"


int status = WL_IDLE_STATUS;
///////enter your sensitive data in the Secret tab/arduino_secrets.h
char hostname[] = _HOSTNAME;
char ssid[] = _SSID;     // your network SSID (name)
char pass[] = _PASSWORD; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;        // your network key Index number (needed only for WEP)

unsigned int localPort = 32227;  // The Alpaca Discovery test port
unsigned int alpacaPort = 11111; // The (fake) port that the Alpaca API would be available on

char packetBuffer[255]; // buffer to hold incoming packet

// MQTT Broker
IPAddress mqtt_server = MQTT_HOST;
const char *topic = MQTT_TOPIC;
const int mqtt_port = MQTT_PORT; //1883;
const char *mqtt_user = MQTT_USER; //"chris";
const char *mqtt_password = MQTT_PASS; // "wandasee2me3";
// end of MQTT

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// ----------------------------------------------------------------------

WebServer *server = new WebServer(alpacaPort);

HTTPUpdateServer updater;
WiFiUDP Udp;

HTTPClient http;

// MQTT
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


OCHandler *device = new OCHandler(server);


auto timer = timer_create_default();

void CheckForDiscovery()
{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Log.traceln("Received packet of size: %d" CR, packetSize);
    IPAddress remoteIp = Udp.remoteIP();
    Log.traceln("From %s , on port %d" CR, remoteIp.toString().c_str(), Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0)
    {
      // Ensure that it is null terminated
      packetBuffer[len] = 0;
    }
    Log.traceln("Contents: %s" CR, packetBuffer);

    // No undersized packets allowed
    if (len < 16)
    {
      return;
    }

    if (strncmp("alpacadiscovery1", packetBuffer, 16) != 0)
    {
      return;
    }

    char response[36] = {0};
    sprintf(response, "{\"AlpacaPort\": %d}", alpacaPort);

    uint8_t buffer[36] = "{\"AlpacaPort\": 11111}";

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(buffer, 36);
    Udp.endPacket();
  }
}

// MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = _HOSTNAME;
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password))
    {
      Serial.println("connected");
      // client.publish(MQTT_TOPIC, "Switch is running");
      // client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Log.traceln(F("SSID: %s" CR), WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Log.traceln(F("IP Address: %s" CR), ip.toString().c_str());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Log.traceln("signal strength (RSSI): %l dBm" CR, rssi);
}


void handleMgmtVersions() { device->handlerMgmtVersions(); }
void handleMgmtDescription() { device->handlerMgmtDescription(); }
void handleMgmtConfiguredDevices() { device->handlerMgmtConfiguredDevices(); }

void handleConnected() { device->handlerConnected(); }
void handleDescriptionGet() { device->handlerDescriptionGet(); }
void handleDriverInfoGet() { device->handlerDriverInfoGet(); }
void handleDriverVersionGet() { device->handlerDriverVersionGet(); }
void handleInterfaceVersionGet() { device->handlerInterfaceVersionGet(); }
void handleNameGet() { device->handlerNameGet(); }
void handleSupportedActionsGet() { device->handlerSupportedActionsGet(); }

void handleAction() { device->handleAction(); }
void handleCommandBlind() { device->handleCommandBlind(); }
void handleCommandBool() { device->handleCommandBool(); }
void handleCommandString() { device->handleCommandString(); }

void handleDriver0AvaragePeriod() { device->handleDriver0AvaragePeriod(); }
void handleDriver0CloudCover() { device->handleDriver0CloudCover(); }
void handleDriver0DewPoint() { device->handleDriver0DewPoint(); }
void handleDriver0CHumidity() { device->handleDriver0CHumidity(); }
void handleDriver0Pressure() { device->handleDriver0Pressure(); }
void handleDriver0RainRate() { device->handleDriver0RainRate(); }
void handleDriver0SkyBrightness() { device->handleDriver0SkyBrightness(); }
void handleDriver0SkyQuality() { device->handleDriver0SkyQuality(); }
void handleDriver0SkyTemperature() { device->handleDriver0SkyTemperature(); }
void handleDriver0StarFWHM() { device->handleDriver0StarFWHM(); }
void handleDriver0Temperature() { device->handleDriver0Temperature(); }
void handleDriver0WindDirection() { device->handleDriver0WindDirection(); }
void handleDriver0WindGust() { device->handleDriver0WindGust(); }
void handleDriver0WindSpeed() { device->handleDriver0WindSpeed(); }
void handleDriver0Refresh() { device->handleDriver0Refresh(); }
void handleDriver0SensorDescription() { device->handleDriver0SensorDescription(); }
void handleDriver0TimeSinceLastUpdate() { device->handleDriver0TimeSinceLastUpdate(); }



/******************************************
 * SETUP
 ******************************************/
void setup()
{
  Serial.begin(115200);

  // Initialize with log level and log output.
  Log.begin(LOG_LEVEL_ERROR, &Serial);

  Log.infoln("Connecting to WIFI...");

  // Some ESP8266 modules broadcast their own network, this turns that off
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);

  // attempt to connect to the Wifi network defined in arduino_secrets.h
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Log.traceln(".");
  }

  Log.infoln("Connected to wifi");
  printWifiStatus();

  // Management API
  server->on("/management/apiversions", HTTP_GET, handleMgmtVersions);
  server->on("/management/v1/description", HTTP_GET, handleMgmtDescription);
  server->on("/management/v1/configureddevices", HTTP_GET, handleMgmtConfiguredDevices);

  server->on("/api/v1/observingconditions/0/connected", HTTP_ANY, handleConnected);
  server->on("/api/v1/observingconditions/0/description", HTTP_GET, handleDescriptionGet);
  server->on("/api/v1/observingconditions/0/driverinfo", HTTP_GET, handleDriverInfoGet);
  server->on("/api/v1/observingconditions/0/driverversion", HTTP_GET, handleDriverVersionGet);
  server->on("/api/v1/observingconditions/0/interfaceversion", HTTP_GET, handleInterfaceVersionGet);
  server->on("/api/v1/observingconditions/0/name", HTTP_GET, handleNameGet);
  server->on("/api/v1/observingconditions/0/supportedactions", HTTP_GET, handleSupportedActionsGet);

  server->on("/api/v1/observingconditions/0/action", HTTP_GET, handleAction);
  server->on("/api/v1/observingconditions/0/commandblind", HTTP_GET, handleCommandBlind);
  server->on("/api/v1/observingconditions/0/commandbool", HTTP_GET, handleCommandBool);
  server->on("/api/v1/observingconditions/0/commandstring", HTTP_GET, handleCommandString);

  
  server->on("/api/v1/observingconditions/0/averageperiod", HTTP_ANY, handleDriver0AvaragePeriod);
  server->on("/api/v1/observingconditions/0/cloudcover", HTTP_GET, handleDriver0CloudCover);
  server->on("/api/v1/observingconditions/0/dewpoint", HTTP_GET, handleDriver0DewPoint);
  server->on("/api/v1/observingconditions/0/humidity", HTTP_GET, handleDriver0CHumidity);
  server->on("/api/v1/observingconditions/0/pressure", HTTP_GET, handleDriver0Pressure);
  server->on("/api/v1/observingconditions/0/rainrate", HTTP_GET, handleDriver0RainRate);
  server->on("/api/v1/observingconditions/0/skybrightness", HTTP_GET, handleDriver0SkyBrightness);
  server->on("/api/v1/observingconditions/0/skyquality", HTTP_GET, handleDriver0SkyQuality);
  server->on("/api/v1/observingconditions/0/skytemperature", HTTP_GET, handleDriver0SkyTemperature);
  server->on("/api/v1/observingconditions/0/starfwhm", HTTP_GET, handleDriver0StarFWHM);
  server->on("/api/v1/observingconditions/0/temperature", HTTP_GET, handleDriver0Temperature);
  server->on("/api/v1/observingconditions/0/winddirection", HTTP_GET, handleDriver0WindDirection);
  server->on("/api/v1/observingconditions/0/windgust", HTTP_GET, handleDriver0WindGust);
  server->on("/api/v1/observingconditions/0/windspeed", HTTP_GET, handleDriver0WindSpeed);
  server->on("/api/v1/observingconditions/0/refresh", HTTP_PUT, handleDriver0Refresh);
  server->on("/api/v1/observingconditions/0/sensordescription", HTTP_GET, handleDriver0SensorDescription);
  server->on("/api/v1/observingconditions/0/timesincelastupdate", HTTP_GET, handleDriver0TimeSinceLastUpdate);

  /*
  //Custom
  server.on("/status",                              HTTP_GET, handlerStatus);
  server.on("/restart",                             HTTP_ANY, handlerRestart);
  */

  updater.setup(server);
  server->begin();
  Log.infoln("Alpaca server handlers setup & started...");
  Log.infoln("Listening for Alpaca discovery requests...");

  Udp.begin(localPort);

  // connectToRedis();
  // MQTT

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  server->handleClient();
  CheckForDiscovery();
  timer.tick();

  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
      

      String serverPath = "http://apollo:6020/api/v1/weather/current";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        //Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();

        device->ocDevice->update(payload);
        //Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    
    lastTime = millis();
  }
}