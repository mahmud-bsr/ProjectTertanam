
// import library
#include <DHT.h>
#include <Wire.h>
#include <FirebaseESP8266.h>
#include <LiquidCrystal_I2C.h>


// konfigurasi pin 
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int relayPin = D3;

// Konfigurasi WiFi
#define WIFI_SSID "YEYEYE"
#define WIFI_PASSWORD "765476548"

// Konfigurasi Firebase
#define FIREBASE_HOST "https://projecttertanam-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyDhnIFKpj64m1QWmalkxfROADORh-I2Y1E"

void setup() {
// inisiasi serial monitor
Serial.begin(115200);

// inisasi lcd dan sensor
lcd.init();
lcd.backlight();
dht.begin();

pinMode(relayPin, OUTPUT);
digitalWrite(relayPin, LOW);

lcd.setCursor(0, 0);
lcd.print("Pemantau suhu");
delay(1000);

// Inisialisasi WiFi
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.println("Connecting to WiFi...");
  }
Serial.println("Connected to WiFi");

// Inisialisasi Firebase
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
// Method Sensor
void Sensor() {
// inisialisasi data dht
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();

// mengatur text di lcd
lcd.setCursor(0, 0);
lcd.print("Suhu: " + String(temperature) + "C");
lcd.setCursor(0, 1);
lcd.print("Humiditas: " + String(humidity) + "%");

// Kirim data ke Firebase
Firebase.pushFloat("sensor/suhu", temperature);
Firebase.pushFloat("sensor/kelembaban", humidity);

float hum = dht.readHumidity();
if (hum >= 95.0) {
digitalWrite(relayPin, HIGH);
// Kirim status relay aktif ke Firebase
Firebase.setBool("sensor/relay_status", true);
delay(1000);
} else {
 digitalWrite(relayPin, LOW);
  // Kirim status relay non-aktif ke Firebase
  Firebase.setBool("sensor/relay_status", false);
  }
}


void loop() {
 Sensor();
}

