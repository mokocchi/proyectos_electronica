#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ArduinoTrace.h>
#include "config.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include "image.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

ESP8266WiFiMulti WiFiMulti;
StaticJsonDocument<2000> doc;
HTTPClient https;
bool connected = false;
String url;
uint8_t fingerprint[20]{0x0};

void tweet_screensaver()
{
  tft.drawBitmap(10, 10, twitter, 100, 100, ST77XX_CYAN);
  delay(500);
  tft.drawBitmap(125, 125, twitter, 100, 100, ST77XX_CYAN);
  delay(500);
  tft.drawBitmap(10, 125, twitter, 100, 100, ST77XX_CYAN);
  delay(500);
  tft.drawBitmap(125, 10, twitter, 100, 100, ST77XX_CYAN);
  delay(500);
  tft.fillScreen(ST77XX_WHITE);
  delay(500);
}

void show_asterisk(u_int16_t color, bool wait)
{
  int buttonState = 0;
  if (wait)
  {
    buttonState = digitalRead(BUTTON);
  }
  int i = 0;
  while ((i < 4) || wait)
  {
    tft.drawBitmap(100, 100, asterisk, 30, 30, color);
    delay(150);
    tft.drawBitmap(100, 100, asterisk, 30, 30, ST77XX_WHITE);
    delay(150);
    i++;
    buttonState = digitalRead(BUTTON);
    if (buttonState != 0)
    {
      break;
    }
  }
}

void setup()
{
  pinMode(BACKLIGHT, OUTPUT);
  analogWrite(BACKLIGHT, 50);
  tft.init(240, 240, SPI_MODE3); // Init ST7789 display 240x240 pixel
  tft.setRotation(2);
  tft.fillScreen(ST77XX_WHITE);
  delay(200);

  show_asterisk(ST77XX_RED, false);
  Serial.begin(115200);

  Serial.println();
  Serial.print("Configuring access point...");

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID_NAME, SSID_PASS);

  while ((WiFiMulti.run() != WL_CONNECTED))
  {
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  show_asterisk(ST77XX_BLUE, false);

  char buffer[73];
  sprintf(buffer, "https://api.twitter.com/2/users/%s/tweets?max_results=5", TWITTER_USER_ID);
  String buf(buffer);
  url = buf;

  std::string string_buf(TWITTER_SSL_FINGERPRINT_HEX);
  for (int i = 0; i < 40; i += 2)
  {
    fingerprint[i / 2] = std::stoi(string_buf.substr(i, 2).c_str(), 0, 16);
  }
}

char *newest_id = "";
String token(TWITTER_API_BEARER_TOKEN);
void loop()
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);
  if (https.begin(*client, url))
  {
    https.addHeader("Authorization", token);
    connected = true;
  }
  else
  {
    show_asterisk(ST77XX_RED, true);
  }
  if (connected)
  {
    show_asterisk(ST77XX_YELLOW, false);
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = https.getString();
        https.end();
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        const char *last = doc["meta"]["newest_id"];
        char msg[70];
        sprintf(msg, "Last vs current: %s/%s", last, newest_id);
        Serial.println(msg);
        if (strcmp(last, newest_id) != 0)
        {
          const char *text = doc["data"][0]["text"];
          char msg2[300];
          sprintf(msg2, "Text found: \"%s\"", text);
          Serial.println(msg2);
          strcpy(newest_id, last);
          show_asterisk(ST77XX_MAGENTA, true);
        }
      }
      else
      {
        // unauthorized
        TRACE();
        String payload = https.getString();
        Serial.println(payload);
        https.end();
        connected = false;
        show_asterisk(ST77XX_RED, false);
      }
    }
    else
    {
      show_asterisk(ST77XX_RED, true);
    }
    int max = 6;
    for (int i = 0; i < max; i++)
    {
      Serial.printf("%d seconds left\n", (max - i) * 10);
      for (int j = 0; j < 5; j++)
      {
        tweet_screensaver();
      }
    }
  }
  else
  {
    delay(500);
    show_asterisk(ST77XX_RED, true);
  }
}
