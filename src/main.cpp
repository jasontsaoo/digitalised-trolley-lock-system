#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// gpio number of the solenoid lock
int LOCK = 12;
// gpio number of the external hall sensor
//  int externalHALL = 34;

// hotspot wifi username + password
const char *ssid = "Geturownwifilah";
const char *password = "lvys3318";

// Network
const String baseUrl = "https://stormy-stream-68782.herokuapp.com/";
const String endpoint = "trolley/1";

void parseJson(String json)
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  const bool shouldUnlock = doc["shouldUnlock"];
  Serial.println(shouldUnlock);
  if (shouldUnlock)
  {
    Serial.println("true");
    digitalWrite(LOCK, HIGH);
  }
  else
  {
    Serial.println("false");
    digitalWrite(LOCK, LOW);
  }
}

WiFiServer server(80);
WiFiClientSecure client;

void fetchTrolley()
{
  Serial.println("\nStarting connection to server...");
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String serverPath = baseUrl + endpoint;
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0)
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      parseJson(payload);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
  delay(1000);
}

// this should be called after the lock is actually unlocked
void afterUnlock()
{
  Serial.println("\n Setting isUnlocked");
  if (WiFi.state() == WL_CONNECTED)
  {
    HTTPClient http;
    String serverPath = baseUrl + "/trolley/setIsUnlocked";

    http.begin(serverPath.c_str());
    int httpResponseCode = http.PUT({
      "trolleyID" : ______,
      "isUnlocked" : true
    });
    if (httpResponseCode > 0)
    {
      Serial.println("All good. \n HTTP Response code:");
      Serial.println(httpResponseCode);
    }
    else
    {
      Serial.println("ERROR");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else
  {
    Serial.println("WIFI DISCONNECTED")
  }
}

// this should be called after the trolley is returned
void returnTrolley()
{
  Serial.println("\n Returning trolley");
  if (WiFi.state() == WL_CONNECTED)
  {
    HTTPClient http;
    String serverpath = baseUrl + "/trolley/returnTrolley";

    http.begin(serverPath.c_str());
    int httpResponseCode = http.PUT({
      "trolleyID" : "__________"
    });
    if (httpResponseCode > 0)
    {
      Serial.println("All good. \n HTTP Response code:");
      Serial.println(httpResponseCode);
    }
    else
    {
      Serial.println("ERROR");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else
  {
    Serial.println("WIFI DISCONNECTED");
  }
}

// hello
//   if (!client.connect(baseUrl, 443))
//   {
//     Serial.println("Connection failed!");
//   }
//   else
//   {
//     Serial.println("Connected to server!");
//     String req_url_str;
//     req_url_str = "GET /";
//     req_url_str += "trolley/1";
//     // req_url_str += user_path + ".json?auth=";
//     // req_url_str += String(firebase_auth) + " HTTP/1.1\r\n";

//   String req_header_str;
//   req_header_str = "Host: ";
//   req_header_str += String(baseUrl) + "\r\n";
//   req_header_str += "Accept: text/event-stream\r\n";
//   req_header_str += "Connection: close\r\n";
//   // req_header_str += "Connection: close\r\n";
//   req_header_str += "\r\n"; //空行

//   Serial.println("Send Server-Sent Events GET request.");
//   // FirebaseサーバーへGETリクエスト送信
//   client.print(req_url_str);
//   client.print(req_header_str);

//   Serial.print(req_url_str);
//   Serial.print(req_header_str);
// }

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello from setup");
  // pinMode(externalHALL, INPUT);
  pinMode(LOCK, OUTPUT);

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
  WiFiClient client = server.available(); // listen for incoming clients
}

// function for internal hall sensor
/*bool lockInsertion()
{
  long x = 0;

  for (int i = 0; i < 1000; i++)
  {
    x += hallRead();
    delayMicroseconds(100);
  }

  x = (double)x / 1000.;

  if (x < 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}
*/

// function for external hall sensor
/*
bool flapConnected()
{
  long x = 0;
  for (int i = 0; i < 1000; i++)
  {
    x += analogRead(externalHALL);
    delayMicroseconds(100);
  }
  x = (double)x / 1000.;

  if (x > 2200)
  {
    return true;
  }
  else
  {
    return false;
  }
}
*/
