#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

#define DATABASE_URL "fechaduraeleservo-default-rtdb.firebaseio.com"
#define DATABASE_SECRET "gXS2l6gWnDXj9N8AuEMfIgqS7yia0zPndOsoBk1d"

FirebaseData fbdo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {21, 19, 18, 5};
byte colPins[COLS] = {17, 16, 4, 0};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo servoMotor;
const int servoPin = 15;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Conectando WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  lcd.setCursor(0,1);
  lcd.print("WiFi conectado");

  Firebase.begin(DATABASE_URL, DATABASE_SECRET);
  Firebase.reconnectWiFi(true);

  servoMotor.attach(servoPin);
  servoMotor.write(0);
}

void loop() {
  if (Firebase.getString(fbdo, "/teste")) {
    String valor = fbdo.stringData();
    Serial.println("Valor Firebase: " + valor);
    lcd.setCursor(0,0);
    lcd.print("Firebase:       ");
    lcd.setCursor(0,1);
    lcd.print(valor + "            ");
    if (valor == "1") servoMotor.write(90);
    else servoMotor.write(0);
  } else {
    Serial.println("Erro Firebase: " + fbdo.errorReason());
  }

  char key = keypad.getKey();
  if (key) {
    Serial.println("Tecla pressionada: " + String(key));
    lcd.setCursor(0,1);
    lcd.print("Tecla: " + String(key) + "       ");
  }

  delay(500);
}
