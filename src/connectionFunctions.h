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

HTTPClient trolleyHttp;
WiFiClientSecure client;

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

// should be on loop when active and not on loan
bool fetchTrolley()
{
    Serial.println("\nfetchTrolley: function activated");
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String serverPath = baseUrl + endpoint;
        http.begin(serverPath.c_str());
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0)
        {
            Serial.println("fetchTrolley: function all good. HTTP Response code: ");
            Serial.print(httpResponseCode);
            String payload = http.getString();
            Serial.println("fetchTrolley: payload: ");
            Serial.print(payload);
            parseJson(payload);
        }
        else
        {
            Serial.println("fetchTrolley: function facing issues, HTTP Response error code: ");
            Serial.print(httpResponseCode);
        }
        // Free resources
        http.end();
    }
    else
    {
        Serial.println("fetchTrolley: WiFi Disconnected");
    }
    delay(1000);
    return true;
}

// this should be called after the lock is actually unlocked
void afterUnlock()
{
    Serial.println("\nafterUnlock: function activated");
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String serverPath = baseUrl + "trolley/setIsUnlocked";

        http.begin(serverPath.c_str());

        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.PUT("{\"trolleyID\":\"1\", \"isUnlocked\": true}");

        if (httpResponseCode > 0)
        {
            Serial.print("afterUnlock: function all good. HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
        else
        {
            Serial.print("afterUnlock: function facing issues, HTTP Response error code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }
    else
    {
        Serial.println("afterUnlock: WiFi Disconnected");
    }
}

// this should be called after the trolley is returned
void returnTrolley()
{
    Serial.println("\nreturnTrolley: function activated");
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String serverPath = baseUrl + "trolley/returnTrolley";
        http.begin(serverPath.c_str());
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.PUT("{\"trolleyID\": \"1\"}");
        if (httpResponseCode > 0)
        {
            Serial.print("returnTrolley: function all good. HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
        else
        {
            Serial.print("returnTrolley: function facing issues, HTTP Response error code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
        // return httpResponseCode == 200;
    }
    else
    {
        Serial.println("returnTrolley: WiFi Disconnected");
        // return false;
    }
}

void connectTrolley()
{
    Serial.println("\n:connectTrolley function activated");
    if (WiFi.status() == WL_CONNECTED)
    {
        String serverPath = baseUrl + "trolley/connect/1";
        trolleyHttp.begin(serverPath.c_str());
        int httpResponseCode = trolleyHttp.GET();
        if (httpResponseCode > 0)
        {
            Serial.print("connectTrolley: function all good. HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = trolleyHttp.getString();
            Serial.print("connectTrolley: payload: ");
            Serial.println(payload);
            parseJson(payload);
        }
        else
        {
            Serial.print("connectTrolley: function facing issues, HTTP Response error code: ");
            Serial.println(httpResponseCode);
        }
        // Free resources
        // trolleyHttp.end();
    }
    else
    {
        Serial.println("connectTrolley: WiFi Disconnected");
    }
    delay(1000);
}

void checkServerResponse()
{
    while (client.available())
    {
        char c = client.read();
        if (c == '\r')
            Serial.print("\\r"); //キャリッジリターン
        if (c == '\n')
            Serial.print("\\n"); //ラインフィード
        Serial.print(c);
    }
}

void readTrolleyData()
{
    while (trolleyHttp.connected())
    {
        String c = trolleyHttp.getString();
        // if (c == '\r')
        //     Serial.print("\\r"); //キャリッジリターン
        // if (c == '\n')
        //     Serial.print("\\n"); //ラインフィード
        Serial.print(c);
    }
}

// void getServerSentEvents()
// {
//     Serial.println("\nStarting connection to server...");
//     if (!client.connect("https://stormy-stream-68782.herokuapp.com/", 443))
//     {
//         Serial.println("Connection failed!");
//     }
//     else
//     {
//         Serial.println("Connected to server!");
//         String req_url_str;
//         req_url_str = "GET /";
//         // req_url_str += user_path + ".json?auth=";
//         req_url_str += "trolley/connect/1 HTTP/1.1\r\n";

//         String req_header_str;
//         req_header_str = "Host: ";
//         req_header_str += String(baseUrl) + "\r\n";
//         req_header_str += "Accept: text/event-stream\r\n";
//         req_header_str += "Connection: close\r\n";
//         req_header_str += "\r\n"; //空行

//         Serial.println("Send Server-Sent Events GET request.");
//         // FirebaseサーバーへGETリクエスト送信
//         client.print(req_url_str);
//         client.print(req_header_str);

//         Serial.print(req_url_str);
//         Serial.print(req_header_str);
//     }
// }
//***********************************
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