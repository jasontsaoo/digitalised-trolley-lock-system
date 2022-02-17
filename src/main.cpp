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

  if (shouldUnlock && flapConnected())
  {

    toggleSolenoid(true);
    Serial.println("solenoid energised, waiting for user to pull out lock");

    while (lockInsertion() == true)
    {
    };

    Serial.println("user pulled out the lock, solenoid denergised");
    afterUnlock();
    WiFi.disconnect();
    toggleSolenoid(false);

    int x = true;
    while (x == true)
    {
      Serial.println("im spamming");
      if (lockInsertion())
      {
        Serial.println("lock inserted");
        WiFi.begin(ssid, password);
        delay(1000);
        returnTrolley();
        x = false;
      }
    }
    Serial.println("finally returned the trolley, leaving function");
  }

  if (flapConnected() == false)
  {
    WiFi.disconnect();

    int y = true;
    while (y == true)
    {
      if (flapConnected() == true)
      {
        WiFi.begin(ssid, password);
        y = false;
      }
    }
  }

  WiFiClient client = server.available(); // listen for incoming clients
}
