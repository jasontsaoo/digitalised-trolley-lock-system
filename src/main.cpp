#include "connectionFunctions.h"
#include "peripheralFunctions.h"

WiFiServer server(80);
WiFiClientSecure client;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello from setup");
  // pinMode(externalHALL, INPUT);
  pinMode(LOCK, OUTPUT);
  pinMode(externalHALL, INPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  fetchTrolley();
  returnTrolley();
  WiFiClient client = server.available(); // listen for incoming clients
}