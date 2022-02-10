#include "connectionFunctions.h"
#include "peripheralFunctions.h"

WiFiServer server(80);
// WiFiClientSecure client;
bool shouldUnlock;

void setup()
{
  shouldUnlock = false;
  Serial.begin(9600);
  Serial.println("Hello from setup");

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
  // getServerSentEvents();
  connectTrolley();
}

void loop()
{

  // check if you should unlock trolley
  shouldUnlock = fetchTrolley();
  if (shouldUnlock)
  {
    // dp stuff here
  }

  // call this after lock locks
  returnTrolley();

  // call this function after the lock unlocks
  afterUnlock();

  toggleSolenoid(true);
  delay(1000);
  toggleSolenoid(false);
  delay(1000);

  // checkServerResponse();
  readTrolleyData();

  WiFiClient client = server.available(); // listen for incoming clients
}