#include <WiFi.h>

const char * ap_ssid="xxx";
const char * ap_pwd="xxxx";

const char * my_ssid="little pony";
const char * my_pwd="12345678";
const byte DNS_PORT = 53;

WiFiUDP udp;
byte packetBuffer[512]; //buffer to hold incoming and outgoing packets
WiFiUDP udp8;



void setup() {
  // put your setup code here, to run once:
  //startup wifi sta mode, connected to the home server ap
  int i =0;
  Serial.begin(115200);
  WiFi.begin(ap_ssid,ap_pwd);
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("dnsIP address: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("gatewayIP address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("subnetMask address: ");
  Serial.println(WiFi.subnetMask());
  WiFi.softAP(my_ssid, my_pwd);
  IPAddress myIP = WiFi.softAPIP();
  udp.begin(DNS_PORT);
  //udp8.begin(DNS_PORT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int packetSize=udp.parsePacket();
  if(packetSize)
  {
        Serial.print(" received from ");
        Serial.print(udp.remoteIP());
        Serial.print(":");
        Serial.println(udp.remotePort());


        int len = udp.read(packetBuffer, 512);

        Serial.printf("read len:%d packetSize:%d",len,packetSize);
        //Serial.println(packetBuffer);
        //for (int i = 1; i <= packetSize; i++) {
        //  Serial.print(packetBuffer[i - 1], HEX);
        //  if (i % 32 == 0) {
        //    Serial.println();
        //  }
        //  else Serial.print(' ');
        //} // end for
        //Serial.println();

        memset(packetBuffer,0,512);
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(packetBuffer,packetSize);
        udp.endPacket();
  }
}
