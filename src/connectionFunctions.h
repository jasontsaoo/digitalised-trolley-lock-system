#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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
    }
    else
    {
        Serial.println("false");
    }
}

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
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String serverPath = baseUrl + "/trolley/setIsUnlocked";

        http.begin(serverPath.c_str());

        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.PUT("{\"trolleyID\":1, \"isUnlocked\":true}");

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

// this should be called after the trolley is returned
void returnTrolley()
{
    Serial.println("\n Returning trolley");
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String serverPath = baseUrl + "/trolley/returnTrolley";

        // construct the json
        StaticJsonDocument<256> doc;
        doc['trolleyID'] = 1;
        char out[128];
        int b = serializeJson(doc, out);
        Serial.print("bytes = ");
        Serial.println(b, DEC);

        http.begin(serverPath.c_str());
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.PUT(out);
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