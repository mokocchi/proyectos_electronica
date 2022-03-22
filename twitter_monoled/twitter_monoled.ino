#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ArduinoTrace.h>
#include "config.h"

ESP8266WiFiMulti WiFiMulti;
StaticJsonDocument<2000> doc;
HTTPClient https;
bool connected = false;
String url;
uint8_t fingerprint [20] {0x0};

void tweet_blink() {
  analogWrite(RED_RGB, 255 - 29);
  analogWrite(GREEN_RGB, 255 - 161);
  analogWrite(BLUE_RGB, 255 - 242);
  delay(1500);
  analogWrite(RED_RGB, 255);
  analogWrite(GREEN_RGB, 255);
  analogWrite(BLUE_RGB, 255);
}
void setup() {
  pinMode(RED_RGB, OUTPUT);
  pinMode(GREEN_RGB, OUTPUT);
  pinMode(BLUE_RGB, OUTPUT);
  analogWrite(RED_RGB, 255);
  analogWrite(GREEN_RGB, 255);
  analogWrite(BLUE_RGB, 0);
  delay(1500);
  analogWrite(BLUE_RGB, 255);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID_NAME, SSID_PASS);

  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  char buffer[73];
  sprintf(buffer, "https://api.twitter.com/2/users/%s/tweets?max_results=5",  TWITTER_USER_ID);
  String buf(buffer);
  url = buf;

  std::string string_buf(TWITTER_SSL_FINGERPRINT_HEX);
  for (int i = 0; i < 40; i += 2) {
    fingerprint[i / 2] = std::stoi(string_buf.substr(i, 2).c_str(), 0, 16);
  }
}

char* newest_id = "";
String token(TWITTER_API_BEARER_TOKEN);
void loop() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);
  if (https.begin(*client, url)) {
    https.addHeader("Authorization", token);
    connected = true;
  } else {
    analogWrite(RED_RGB, 0);
  }
  if (connected) {
    analogWrite(GREEN_RGB, 0);
    analogWrite(RED_RGB, 0);
    int httpCode = https.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        https.end();
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        const char* last = doc["meta"]["newest_id"];
        char msg[70];
        sprintf(msg, "Last vs current: %s/%s", last, newest_id);
        Serial.println(msg);
        if (strcmp(last, newest_id) != 0) {
          const char* text = doc["data"][0]["text"];
          char msg2[300];
          sprintf(msg2, "Text found: \"%s\"", text);
          Serial.println(msg2);
          strcpy(newest_id, last);
          tweet_blink();
        }
      } else {
        //unauthorized
        TRACE();
        String payload = https.getString();
        Serial.println(payload);
        https.end();
        connected = false;
        analogWrite(RED_RGB, 0);
        delay(600);
        analogWrite(RED_RGB, 255);
      }
    } else {
      //connection error
      analogWrite(RED_RGB, 0);
    }
    analogWrite(RED_RGB, 255);
    analogWrite(GREEN_RGB, 0);
    int max = 6;
    for (int i = 0; i < max; i++) {
      Serial.printf("%d seconds left\n", (max - i) * 10);
      delay(1000 * 10);
      analogWrite(GREEN_RGB, 255);
      delay(200);
      analogWrite(GREEN_RGB, 0);
    }
    analogWrite(GREEN_RGB, 255);
  } else {
    delay(500);
    analogWrite(RED_RGB, 255);
  }
}
