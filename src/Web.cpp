#include "web.h"
#include "predef.h"

#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "Lilly-AP";
const char *password = "123456789";  // password length is important

extern void sendCommand(byte command, byte dat1, byte dat2);

WiFiServer server(80);

void handleWebRequest(WiFiClient &client) {
  unsigned long ultimeout = millis() + 250;
  while (!client.available() && (millis() < ultimeout)) {
    delay(1);
  }
  if (millis() > ultimeout) {
#ifdef DEBUG
    Console.println("client connection time-out!");
#endif
    return;
  }

  // Read the first line of the request
  String sRequest = client.readStringUntil('\r');
#ifdef DEBUG
  Console.println("Request is: ");Console.println(sRequest);
#endif
  client.flush();

  // stop client, if request is empty
  if (sRequest == "") {
#ifdef DEBUG
    Console.println("empty request! - stopping client");
#endif
    client.stop();
    return;
  }

  // get path; end of path is either space or ?
  // Syntax is e.g. GET / HTTP/1.1
  String sPath = "", sParam = "", sCmd = "";
  String sGetstart = "GET ";
  int iStart, iEndSpace, iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart >= 0) {
    iStart += +sGetstart.length();
    iEndSpace = sRequest.indexOf(" ", iStart);
    iEndQuest = sRequest.indexOf("?", iStart);

    if (iEndSpace > 0) {
      if (iEndQuest > 0) {
        // there are parameters
        sPath = sRequest.substring(iStart, iEndQuest);
        sParam = sRequest.substring(iEndQuest, iEndSpace);
      } else {
        // NO parameters
        sPath = sRequest.substring(iStart, iEndSpace);
      }
    }
  }

  if (sParam.length() > 0) {
    int iEqu = sParam.indexOf("=");
    if (iEqu >= 0) {
      sCmd = sParam.substring(iEqu + 1, sParam.length());
#ifdef DEBUG
      Console.println(sCmd);
#endif
    }
  } else {
#ifdef DEBUG
    Console.println("No paramaters recognized");
#endif
  }

  String sResponse, sHeader;

  if (sPath != "/") {
#ifdef DEBUG
    Console.println("Resp 404: ");Console.println(sPath);
#endif 
    sResponse = "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
    sHeader = "HTTP/1.1 404 Not found\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  } else {
#ifdef DEBUG
    Console.println("Prepare the response on root");
#endif
    sResponse = "<!DOCTYPE html><html><head><title>QR code info</title>";
    sResponse += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">";
    sResponse += "</head><body bgcolor=\"#d0d0f0\">";
    sResponse += "<h1>Lilly Go</h1>";
    sResponse += "</body></html>";
    sResponse += "\r\n";

    sResponse = ""; // TEST

    sHeader = "HTTP/1.1 200 OK\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
#ifdef DEBUG
  Console.println("Response header: ");Console.println(sHeader);
#endif

  client.print(sHeader);
  //client.println(sResponse);

#ifdef DEBUG
  Console.println("Response written, close connection");
#endif

  client.stop();
}

MyWebServer::MyWebServer() {
}

void MyWebServer::Setup() {
  // AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
#ifdef DEBUG
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
#endif

  server.begin();
#ifdef DEBUG
  Console.print("Start AP: ");
  Console.println(ssid);
#endif
}

void MyWebServer::Update() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
#ifdef DEBUG
  Console.println("new client");
#endif
  handleWebRequest(client);
}