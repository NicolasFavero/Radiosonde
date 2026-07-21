#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <avr/wdt.h>
#include <Arduino.h>
#define GPS_RX 4
#define GPS_TX 5
#define LORA_RX 2
#define LORA_TX 3
#define BMP_ADDR 0x76
#define BAT_PIN A3

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
SoftwareSerial loraSerial(LORA_RX, LORA_TX);

float readBattery() {
  int raw = analogRead(BAT_PIN);
  float voltage = raw * (5.0 / 1023.0); 
  return voltage;
 }
class Sensors {
  public:
    TinyGPSPlus gps;
    Adafruit_BMP280 bmp;

    bool gpsOK = false;
    bool bmpOK = false;

    double lat = 0, lon = 0, altGPS = 0;
    float altBMP = 0, temp = 0, press = 0;

    uint8_t sats = 0;
    uint8_t hh = 0, mm = 0, ss = 0;
    uint8_t day = 0, mon = 0;
    uint16_t year = 0;

    void begin() {
      gpsSerial.begin(9600);

      if (bmp.begin(BMP_ADDR)) {
        bmp.setSampling(
          Adafruit_BMP280::MODE_FORCED,
          Adafruit_BMP280::SAMPLING_X2,
          Adafruit_BMP280::SAMPLING_X16,
          Adafruit_BMP280::FILTER_X16,
          Adafruit_BMP280::STANDBY_MS_500
        );
        bmpOK = true;
      }
    }

    void update() {
      readGPS();
      if (bmpOK) readBMP();
    }

  private:
    void readGPS() {
      while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
      }

      gpsOK = gps.location.isValid();

      if (!gpsOK) return;

      lat = gps.location.lat();
      lon = gps.location.lng();
      sats = gps.satellites.value();

      if (gps.altitude.isValid())
        altGPS = gps.altitude.meters();

      if (gps.time.isValid()) {
        hh = gps.time.hour();
        mm = gps.time.minute();
        ss = gps.time.second();
      }

      if (gps.date.isValid()) {
        day = gps.date.day();
        mon = gps.date.month();
        year = gps.date.year();
      }
    }

    void readBMP() {
      if (bmp.takeForcedMeasurement()) {
        temp = bmp.readTemperature();
        press = bmp.readPressure() / 100.0;
        altBMP = bmp.readAltitude(1013.25);
      }
    }
 };

class JsonPacket {
  public:
    char* create(Sensors &s) {

      char bat[8];
      dtostrf(readBattery(), 4, 2, bat);

      snprintf(buffer, sizeof(buffer),
        "{\"id\":\"SondeTFX02\","
        "\"src\":\"%s\","
        "\"bat\":%s"
        "%s"
        "%s"
        "}\n",

        source(s),
        bat,
        gpsBlock(s),
        bmpBlock(s)
      );
      return buffer;
    }

  private:
    char buffer[256];

    const char* source(Sensors &s) {
      if (s.gpsOK && s.bmpOK) return "GPS+BMP";
      if (s.gpsOK) return "GPS";
      if (s.bmpOK) return "BMP";
      return "NONE";
    }

    const char* gpsBlock(Sensors &s) {
      static char gpsPart[160];
      if (!s.gpsOK) return "";

      char lat[12], lon[12], alt[8];
      dtostrf(s.lat, 8, 6, lat);
      dtostrf(s.lon, 8, 6, lon);
      dtostrf(s.altGPS, 6, 1, alt);

      snprintf(gpsPart, sizeof(gpsPart),
        ",\"lat\":%s,"
        "\"lon\":%s,"
        "\"alt\":%s,"
        "\"sat\":%d,"
        "\"date\":\"%02d/%02d/%04d\","
        "\"time\":\"%02d:%02d:%02d\"",

        lat,
        lon,
        alt,
        s.sats,
        s.day, s.mon, s.year,
        s.hh, s.mm, s.ss
      );
      return gpsPart;
    }

    const char* bmpBlock(Sensors &s) {
      static char bmpPart[64];
      if (!s.bmpOK) return "";

      char temp[8], press[8];
      dtostrf(s.temp, 4, 1, temp);
      dtostrf(s.press, 5, 1, press);

      snprintf(bmpPart, sizeof(bmpPart),
        ",\"temp\":%s,"
        "\"press\":%s",
        temp,
        press
      );
      return bmpPart;
    }
 };

unsigned long lastSend = 0;
Sensors sensors;
JsonPacket json;
void setup() {
  wdt_disable();
  
  delay(50);
  wdt_enable(WDTO_8S);

  Serial.begin(9600);
  loraSerial.begin(9600);

  pinMode(BAT_PIN, INPUT);  // adiciona pino da bateria

  sensors.begin();
  Serial.println("Sistema iniciado");
 }
void loop() {
  wdt_reset();

  sensors.update();

  if (millis() - lastSend >= 10000) {
    char* payload = json.create(sensors);
    char data[220];
    Serial.print("JSON enviado: ");

    Serial.println(payload);
    loraSerial.print(payload);
 
    lastSend = millis();
    
    }
 }
