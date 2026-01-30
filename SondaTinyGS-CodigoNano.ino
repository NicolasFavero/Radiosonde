#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Adafruit_BMP280.h>

#define GPS_RX 4
#define GPS_TX 5
#define LORA_RX 2
#define LORA_TX 3

#define BATTERY_VOLTAGE 7.4  // valor fixo
#define OUT(x) do { Serial.print(x); ss.print(x); } while(0)
#define OUT2(x, y) do { Serial.print(x, y); ss.print(x, y); } while(0)
TinyGPSPlus gps;
Adafruit_BMP280 bmp;
SoftwareSerial ss(GPS_RX, GPS_TX);

unsigned long lastSend = 0;
String ID = "\"SondeTFX02\"";

// ===== STATUS =====
bool gpsOK = false;
bool bmpOK = false;

// ===== GPS =====
double lat = 0, lon = 0, altGPS = 0;
uint8_t sats = 0;
uint8_t hh = 0, mm = 0, ss_ = 0;
uint8_t day_ = 0, mon_ = 0;
uint16_t year_ = 0;

// ===== BMP =====
float t = 0, p = 0, altBMP = 0;

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  ss.begin(9600); // GPS ativo

  if (bmp.begin(0x76)) {
    bmp.setSampling(
      Adafruit_BMP280::MODE_FORCED,
      Adafruit_BMP280::SAMPLING_X2,
      Adafruit_BMP280::SAMPLING_X16,
      Adafruit_BMP280::FILTER_X16,
      Adafruit_BMP280::STANDBY_MS_500
    );
    bmpOK = true;
  }

  Serial.println(F("Sistema iniciado"));
}

// ================= LOOP =================
void loop() {
  readGPS();

  if (millis() - lastSend > 5000) {
    if (bmpOK) readBMP();
    printStatus();
    sendPacket();
    lastSend = millis();
  }
}

// ================= GPS =================
void readGPS() {
  ss.listen();

  while (ss.available()) {
    gps.encode(ss.read());
  }

  gpsOK = gps.location.isValid();

  if (gpsOK) {
    lat = gps.location.lat();
    lon = gps.location.lng();
    sats = gps.satellites.value();

    if (gps.altitude.isValid())
      altGPS = gps.altitude.meters();

    if (gps.time.isValid()) {
      hh = gps.time.hour();
      mm = gps.time.minute();
      ss_ = gps.time.second();
    }

    if (gps.date.isValid()) {
      day_ = gps.date.day();
      mon_ = gps.date.month();
      year_ = gps.date.year();
    }
  }
}

// ================= BMP =================
void readBMP() {
  if (bmp.takeForcedMeasurement()) {
    t = bmp.readTemperature();
    p = bmp.readPressure() / 100.0;
    altBMP = bmp.readAltitude(1013.25);
  }
}

// ================= STATUS SERIAL =================
void printStatus() {
  
  Serial.println(F("------------------------------"));

  Serial.print(F("BMP: "));
  Serial.print(bmpOK ? F("OK") : F("FAIL"));
  Serial.print(F(" | GPS: "));
  Serial.print(gpsOK ? F("OK") : F("FAIL"));
  Serial.print(F(" | Sats: "));
  Serial.print(sats);

}

// ================= SEND =================
void sendPacket() {
  ss.end();
  ss = SoftwareSerial(LORA_RX, LORA_TX);
  ss.begin(9600);

  

  Serial.print(F("  ------  JSON enviado:  "));

  OUT("{\"id\":"); OUT(ID);   // <<< ID PRIMEIRO ITEM

  OUT(",\"src\":\"");
  if (gpsOK && bmpOK) OUT("GPS+BMP");
  else if (gpsOK)     OUT("GPS");
  else if (bmpOK)     OUT("BMP");
  else                OUT("NONE");
  OUT("\"");

  OUT(",\"bat\":"); OUT2(BATTERY_VOLTAGE, 2);

  if (gpsOK) {
    OUT(",\"lat\":"); OUT2(lat, 6);
    OUT(",\"lon\":"); OUT2(lon, 6);

    OUT(",\"alt\":");
    if (gps.altitude.isValid() && gpsOK) OUT2(altGPS, 1);
    else OUT2(altBMP, 1);

    OUT(",\"sat\":"); OUT(sats);

    OUT(",\"date\":\"");
    OUT(day_); OUT('/'); OUT(mon_); OUT('/'); OUT(year_);
    OUT("\"");

    OUT(",\"time\":\"");
    if (hh < 10) OUT('0'); OUT(hh); OUT(':');
    if (mm < 10) OUT('0'); OUT(mm); OUT(':');
    if (ss_ < 10) OUT('0'); OUT(ss_);
    OUT("\"");
  }

  if (bmpOK) {
    OUT(",\"temp\":");  OUT2(t, 1);
    OUT(",\"press\":"); OUT2(p, 1);
  }

  OUT("}\n");

  ss.end();
  ss = SoftwareSerial(GPS_RX, GPS_TX);
  ss.begin(9600);
}


