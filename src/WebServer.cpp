#include "WebServer.h"

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include "LineDataUpdater.h"
#include "predef.h"

extern char g_Lines[][25];
extern bool g_textHasChanged;

const char *ssid = "Lilly-AP";
const char *password = "123456789";  // password length is important
String g_Status;
LineDataUpdater g_LineUpdater;
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>QR Card</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .textbox {position: relative; display: inline-block} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>QR Visitenkarte</h2>
  %LINESPLACEHOLDER%
  <br/>
  %STATUSPLACEHOLDER%
<script>
</script>
</body>
</html>
<script>
function httpPostChange ( theReq ){
    var theUrl = "/?" + theReq + "&version=" + Math.random() ;
    var xhr = new XMLHttpRequest() ;
    xhr.onreadystatechange = function() {
      if ( xhr.readyState == XMLHttpRequest.DONE )
      {
        resultstr.value = xhr.responseText ;
      }
    }
    xhr.open ( "POST", theUrl, false ) ;
    xhr.send() ;
}
</script>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String &var) {
  //Serial.println(var);
  if (var == "LINESPLACEHOLDER") {
    String app = "";
    app += "<h4>Zeilen</h4>";
    app += "<form method='POST' action='/changeline'";
    app += "<label class='textbox'>Line 1</label><input type='text' name='line1' value='" + String(g_Lines[0]) + "' id='ln1'>";
    app += "  <br><input type=\"submit\" value=\"Submit\">";
    app += "</form>";
    return app;
  } else if (var == "STATUSPLACEHOLDER") {
    String status = "";
    status += "<div><span>" + g_Status + "</status></div>";
    return status;
  }
  return String();
}

void onRequestNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

MyWebServer::MyWebServer() {
}

void MyWebServer::Setup() {
  g_Status = "";
  WiFi.mode(WIFI_AP);  // AP mode
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  CONSOLEPF("AP IP address: %s", IP.toString().c_str());
  CONSOLEPF("Start AP: %s", ssid);

  g_LineUpdater.Setup();
  g_LineUpdater.FetchLines();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    g_Status = "";
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/changeline", HTTP_POST, [](AsyncWebServerRequest *request) {
    CONSOLEPF("changeline POST ");

    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter *p = request->getParam(i);
      CONSOLEPF("POST[%s]: %s", p->name().c_str(), p->value().c_str());
      g_LineUpdater.AddLine(p->name().c_str(), p->value().c_str());
    }
    if (g_LineUpdater.IsChanged()) {
      g_LineUpdater.Commit();
      g_textHasChanged = true;
      g_Status = "Lines changed!";
    } else {
      g_Status = "";
    }

    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/changeline", HTTP_GET, [](AsyncWebServerRequest *request) {
    // seems necessary to work with POST
    CONSOLEPF("changeline GET ");
    request->send(200);
  });

  server.onNotFound(onRequestNotFound);
  server.begin();
}
