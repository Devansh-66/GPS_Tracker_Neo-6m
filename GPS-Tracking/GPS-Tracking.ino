#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <TinyGPS++.h>

static const int RXpin = D7, TXpin = D6;  //May differ with different gps modules
const int ledPin = D4;
const int MAX_RECONNECT_ATTEMPTS = 5;  // Maximum reconnection attempts before starting portal
WiFiManager wifiManager;
int reconnectCount = 0;

TinyGPSPlus gps;
SoftwareSerial SerialGPS(TXpin, RXpin);

float Latitude, Longitude, Altitude, Speed, HDOP, Course;
//PDOP, VDOP, SatellitesUsed, FixString;
int year, month, date, hour, minute, second;
String DateString, TimeString, LatitudeString, LongitudeString, AltitudeString, SpeedString, HDOPString, CourseString;
//PDOPString, VDOPString, SatellitesUsedString;

WiFiManager wifi;
WiFiServer server(80);

void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("AP SSID: ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  Serial.begin(9600);
  SerialGPS.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED off initially (LED is active LOW)

  Serial.println("\nStarting setup...");

  // Configure WiFiManager
  wifiManager.setConfigPortalTimeout(30);         // Portal timeout after 30 sec
  wifiManager.setAPCallback(configModeCallback);  // Called when entering config mode

  if (!wifiManager.autoConnect("ESP_WiFi")) {   // Try to connect using saved credentials
    Serial.println("Failed to connect and timeout occurred");
    ESP.restart();  // Restart ESP if connection fails
  }
  server.begin();
  Serial.println("Server started");
  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    reconnectCount = 0;  // Reset reconnect counter when connected

    // Connected state - LED blink pattern (4 seconds on, 1 second off)
    digitalWrite(ledPin, LOW);  // LED ON
    delay(4000);
    digitalWrite(ledPin, HIGH);  // LED OFF
    delay(1000);

  } else { // Not connected - try to reconnect
    digitalWrite(ledPin, HIGH);  // LED off while disconnected
    Serial.println("WiFi disconnected");

    reconnectCount++;
    Serial.print("Reconnection attempt: ");
    Serial.println(reconnectCount);

    if (reconnectCount >= MAX_RECONNECT_ATTEMPTS) {
      Serial.println("Starting configuration portal");
      reconnectCount = 0;

      if (!wifiManager.startConfigPortal("ESP_WiFi")) { // Start config portal
        Serial.println("Failed to connect through portal");
        // delay(100);
        ESP.restart();
      }

      Serial.println("Connected through portal!");
      Serial.println(WiFi.localIP());

    } else {
      WiFi.reconnect();  // Try to reconnect to saved network
      delay(5000);  // Wait 5 seconds between reconnection attempts
    }
  }


  //For Gps location
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {

      // Extract GPS data
      if (gps.location.isValid()) {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude, 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude, 6);
        Serial.print("Latitude: ");
        Serial.println(Latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(Longitude, 6);
      }

      if (gps.altitude.isValid()) {
        Altitude = gps.altitude.meters();      // or gps.altitude.feet()
        AltitudeString = String(Altitude, 1);
        Serial.print("Altitude: ");
        Serial.println(Altitude, 6);
      }

      if (gps.speed.isValid()) {
        Speed = gps.speed.kmph();        // or gps.speed.knots()
        SpeedString = String(Speed, 6);
        Serial.print("Speed: ");
        Serial.println(Speed, 6);
      }

      if (gps.hdop.isValid()) {
        HDOP = gps.hdop.value();
        HDOPString = String(HDOP, 2);
        Serial.print("hdop: ");
        Serial.println(HDOP, 2);
      }

      if (gps.course.isValid()) {
        Course = gps.course.deg();
        CourseString = String(Course, 2);
        Serial.print("Course: ");
        Serial.println(Course, 2);
      }

      if (gps.date.isValid()) {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10) {
          DateString += '0';
        }
        DateString += String(date);
        DateString += " / ";

        if (month < 10) {
          DateString += '0';
        }
        DateString += String(month);
        DateString += " / ";

        DateString += String(year);

        Serial.print("Date: ");
        Serial.println(date);
        Serial.print("Month: ");
        Serial.println(month);
        Serial.print("Year: ");
        Serial.println(year);
      }

      if (gps.time.isValid()) {
        TimeString = "";
        hour = gps.time.hour() + 5;  //adjust UTC
        minute = gps.time.minute() + 30;
        second = gps.time.second();

        if (minute >= 60) {
          hour += 1;
          minute -= 60;
        }

        if (hour >= 24) {
          hour -= 24;
        }

        if (hour < 10)
          TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
          TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
          TimeString += '0';
        TimeString += String(second);

        Serial.print("Hour: ");
        Serial.println(hour);
        Serial.print("Minute: ");
        Serial.println(minute);
        Serial.print("Second: ");
        Serial.println(second);
      }

      // Additional

      // if (gps.pdop.isValid()) {
      //   PDOP = gps.pdop.value();
      //   PDOPString = String(PDOP, 2);
      // }

      // if (gps.vdop.isValid()) {
      //   VDOP = gps.vdop.value();
      //   VDOPString = String(VDOP, 2);
      // }

      // if (gps.satellites.used() > 0) {
      //   SatellitesUsed = gps.satellites.used();
      //   SatellitesUsedString = String(SatellitesUsed);
      // }

      // if (gps.fix() == TinyGPSPlus::GPS_FIX_OK) {
      //   FixString = "GPS Fix";
      // } else {
      //   FixString = "No Fix";
      // }
    }
  }

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String s = "<!DOCTYPE html>";
  s += "<html lang='en'>";
  s += "<head>";
  s += "<meta charset='UTF-8'>";
  s += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  s += "<title>Live GPS Tracking</title>";
  s += "<style>";
  s += "body {";
  s += "  font-family: Arial, sans-serif;";
  s += "  background-color: #f4f4f4;";
  s += "  margin: 0;";
  s += "  padding: 20px;";
  s += "}";
  s += ".container {";
  s += "  max-width: 800px;";
  s += "  margin: 0 auto;";
  s += "  background-color: white;";
  s += "  border-radius: 15px;";
  s += "  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);";
  s += "  padding: 20px;";
  s += "  display: flex;";
  s += "  flex-direction: column;";
  s += "}";
  s += ".header {";
  s += "  text-align: center;";
  s += "  margin-bottom: 20px;";
  s += "}";
  s += ".header h1 {";
  s += "  color: #333;";
  s += "  font-size: 24px;";
  s += "  margin-bottom: 10px;";
  s += "}";
  s += ".header p {";
  s += "  color: #666;";
  s += "}";
  s += ".card {";
  s += "  flex: 1;";  /* Equal width for cards */
  s += "  background-color: #f0f0f0;";
  s += "  border-radius: 10px;";
  s += "  padding: 15px;";
  s += "  margin: 10px;"; /* Spacing between cards */
  s += "}";
  s += ".card-title {";
  s += "  color: #007bff;";
  s += "  font-weight: bold;";
  s += "  margin-bottom: 10px;";
  s += "}";
  s += ".row {";
  s += "  display: flex;";
  s += "  align-items: center;";
  s += "  margin-bottom: 10px;";
  s += "}";
  s += ".icon {";
  s += "  margin-right: 10px;";
  s += "  font-size: 18px;";
  s += "}";
  s += ".label {";
  s += "  font-weight: bold;";
  s += "  margin-right: 10px;";
  s += "  color: #333;";
  s += "}";
  s += ".footer {";
  s += "  text-align: center;";
  s += "  background-color: #f1f1f1;";
  s += "  padding: 10px;";
  s += "  margin-top: 20px;";
  s += "}";
  s += "@media (max-width: 768px) {"; /* Styles for other screens */
  s += "  .container {";
  s += "    flex-direction: column;";  /* Content vertically for small screens */
  s += "  }";
  s += "}";
  s += "</style>";
  s += "</head>";
  s += "<body>";
  s += "<div class='container'>";

  // Header
  s += "<div class='header'>";
  s += "<h1>Live GPS Tracking</h1>";
  s += "<p>Real-time Location Monitoring</p>";
  s += "</div>";

  // Location Details
  s += "<div class='card'>";
  s += "<div class='card-title'>Location Details</div>";
  s += "<div class='row'>";
  s += "<div class='icon'>🌍</div>";
  s += "<div class='label'>Latitude:</div>";
  s += LatitudeString + "°N";
  s += "</div>";
  s += "<div class='row'>";
  s += "<div class='icon'>🌎</div>";
  s += "<div class='label'>Longitude:</div>";
  s += LongitudeString + "°W";
  s += "</div>";
  s += "<div class='row'>";
  s += "<div class='icon'>🌏</div>";
  s += "<div class='label'>Altitude:</div>";
  s += AltitudeString + "m";
  s += "</div>";

  // Map Link
  String mapUrl = "https://maps.google.com/?q=" + LatitudeString + "," + LongitudeString;
  s += "<div class='row'>";
  s += "<a href='" + mapUrl + "' target='_blank'>Click to view on Google Maps</a>";
  s += "</div>";
  s += "</div>";

  // Movement Details
  s += "<div class='card'>";
  s += "<div class='card-title'>Movement & Environment</div>";
  s += "<div class='row'>";
  s += "<div class='icon'>🧭</div>";
  s += "<div class='label'>Course:</div>";
  s += CourseString + "°";
  s += "</div>";
  s += "<div class='row'>";
  s += "<div class='icon'>🚗</div>";
  s += "<div class='label'>Speed:</div>";
  s += SpeedString + " km/h";
  s += "</div>";
  s += "<div class='row'>";
  s += "<div class='icon'>📡</div>";
  s += "<div class='label'>HDOP:</div>";
  s += HDOPString;
  s += "</div>";
  //Additional add
  s += "</div>";

  // Footer
  s += "<div class='footer'>";
  s += "📅 " + DateString + " | 🕒 " + TimeString;
  s += "</div>";

  client.print(s);
  delay(10);
}