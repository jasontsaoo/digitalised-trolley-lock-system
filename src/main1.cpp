#include "connectionFunctions1.h"
#include "peripheralFunctions.h"

WiFiServer server(80);
bool shouldUnlock;
bool flapConnection;

void setup()
{
  pinMode(LOCK, OUTPUT);
  pinMode(externalHALL, INPUT);
  pinMode(LED, OUTPUT);
  shouldUnlock = false;

  Serial.begin(9600);
  Serial.println("Hello from setup");
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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // getServerSentEvents();
  connectTrolley();
}

void loop()
{
  // check if you should unlock trolley
  shouldUnlock = fetchTrolley();
  flapConnection = flapConnected();

  if (shouldUnlock && flapConnection)
  {
    Serial.println("Unlocking loop activated");
    toggleSolenoid(true);
    while (lockInsertion_hall() == true)
    {
      Serial.println("Still waiting for lock to be removed");
      delay(200);
    };
    Serial.println("lock is removed, going into afterUnlock function");
    afterUnlock();
    WiFi.disconnect();
    toggleSolenoid(false);
    delay(5000);

    bool x = true;
    while (x == true)
    {
      if (lockInsertion_battery() == 1)
      {
        Serial.println("detected lock insertion, starting WiFi");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);
          Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        // getServerSentEvents();
        connectTrolley();
        delay(500);
        returnTrolley();
        x = false;
      }
      else
      {
        Serial.println("Still waiting for lock insertion");
      }
      delay(1000);
    }
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
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);
          Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        // getServerSentEvents();
        connectTrolley();
        y = false;
      }
    }
  }

  Serial.println("Reached the end of the loop, going back up the loop");
}
