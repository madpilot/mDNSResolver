#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <mDNSResolver.h>

#define WIFI_AP         "my-wifi-ap"
#define WIFI_PASS       "my-wifi-pass"
#define NAME_TO_RESOLVE "test.local"

using namespace mDNSResolver;

WiFiClient wifiClient;
WiFiUDP udp;
Resolver resolver(udp);

void setup() {
  Serial.begin(115200);
  int WiFiCounter = 0;
  // We start by connecting to a WiFi network
  Serial.println("Connecting to ");
  Serial.println(WIFI_AP);

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_AP, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED && WiFiCounter < 30) {
    delay(1000);
    WiFiCounter++;
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Resolving ");
  Serial.println(NAME_TO_RESOLVE);

  resolver.setLocalIP(WiFi.localIP());
  
  IPAddress ip = resolver.search(NAME_TO_RESOLVE);
  if(ip != INADDR_NONE) {
    Serial.print("Resolved: ");
    Serial.println(ip);
  } else {
    Serial.println("Not resolved");
  }
}

void loop() {
  // Required to clear the UDP buffer, as we can't unjoin a multicast group yet
  resolver.loop();
}
