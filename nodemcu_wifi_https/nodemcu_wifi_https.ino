#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <sstream>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFiMulti.h>
#include "config.h"
using namespace std;

ESP8266WiFiMulti WiFiMulti;
HTTPClient https;

void setup() {
  Serial.begin(115200);
  Serial.print("Configuring access point...");

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID_name, SSID_pass);

  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  uint8_t fingerprint [20] {0x0};
  std::string string_buf(fingerprint_buf);
  for (int i = 0; i < 40; i += 2) {
    fingerprint[i / 2] = std::stoi(string_buf.substr(i, 2).c_str(), 0, 16);
  }

  client->setFingerprint(fingerprint);
  if (https.begin(*client, HTTPS_URL)) {  // HTTPS
    int httpCode = https.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        Serial.println("OK OR MOVED");
        String payload = https.getString();
        Serial.println(payload);
        https.end();
      } else {
        //HTTP error
        Serial.println("HTTP error");
      }
    } else {
      //connection error
      Serial.println("connection error");
    }
  }
}
void loop() {

}
