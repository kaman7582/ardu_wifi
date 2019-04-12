
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

const char * ssid="myesp32";
const char * pwd="12345678";
const byte DNS_PORT = 53;
DNSServer dnsServer;
IPAddress apIP(192, 168, 1, 1);
WebServer  webServer(80);
String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
                      "be redirected here.</p></body></html>";

String reply_html= "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>";

String sub_html="<h1>Switch to Client mode</h1>";

String html_1  = R"=====(
<form action="/login" method="POST">
  <p>SSID: <input type="text" name="fname" /></p>
  <p>Password: <input type="text" name="lname" /></p>
  <input type="submit" value="Login" />
</form>
)=====";

void handleRoot() {  
  // When URI / is requested, send a web page with a button to toggle the LED
  Serial.print("handle root\n");
  webServer.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void handleSubmit() {  
   Serial.print("handle post\n");
   webServer.send(200, "text/html",sub_html);
}

void setup()
{
    Serial.begin(115200);
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Chen Xue at Home");
    IPAddress ip= WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(ip);
    dnsServer.start(DNS_PORT, "*", apIP);
    webServer.onNotFound([]() {
    Serial.print("not found dns\n");
    webServer.send(200, "text/html", html_1);
  });
    //webServer.on("/", HTTP_GET, handleRoot);
    webServer.on("/login", HTTP_POST, handleSubmit);
  webServer.begin();

}

#define logout  Serial.println
//connec a client popup a dialog box
void loop(){
    //Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
    //delay(300);
      dnsServer.processNextRequest();
  webServer.handleClient();

}
