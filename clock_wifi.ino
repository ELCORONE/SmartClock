#include <iarduino_OLED_txt.h>  // Библиотека для дисплея
iarduino_OLED_txt myOLED(0x78); // Подключение по адресу

#include <TimeLib.h>            // Библиотека часов

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int server_time;
int tHour, tMinute, tSecond, tTimer;

// Настройки Wi-Fi
String WifiSettings[][2] = {
  {"PIESTUDIO.RU", "9147374552"},
  {"SOFTVED_2.4Gh", "128256512"}
};


int pButton = 1;                // Пин подключения кнопки
extern uint8_t SmallFontRus[];  // Подключение шрифта

void setup() {
  myOLED.begin();               // Инициируем работу с дисплеем.
  myOLED.setFont(SmallFontRus); // Установка маленького шрифта
  pinMode(pButton, INPUT);      // Объявляем кнопку цифровым входом
  myOLED.clrScr();              // Очистка дислея перед началом
  myOLED.print("Привет сладкий", 7,7);
  WiFi.mode(WIFI_OFF);                // Отключение от прошлых соединений
  delay(200);                         // Ждем секунду
  WiFi.mode(WIFI_STA);                // ESP8266 в режиме станции
  getStatus();
}

// Цикл Адруин очки
void loop() {
  if (digitalRead(pButton) == HIGH) {
    getStatus();
  }
}


// При нажатии кнопки проверяет рядом существующие Wi-Fi сети
// После обнаружения дружелюбных сетей подключается и обновляет данные в память устройства
void getStatus() {
  byte wificount = WiFi.scanNetworks(); // Сканирует сети и записывает в переменную
  if (wificount != 0) {
    for (int wifi_found_i = 0; wifi_found_i < wificount; wifi_found_i++) {  // Цикл проверки Wi-Fi сетей
      for (int wifi_found_y = 0; wifi_found_y < 1; wifi_found_y++) {        // Цикл дружелюбных Wi-Fi сетей
        if (WiFi.SSID(wifi_found_i) == WifiSettings[wifi_found_y][0]) {     // Проверка на дружелюбность
          WiFi.begin(WifiSettings[wifi_found_y][0], WifiSettings[wifi_found_y][1]);         // Подключение к Wi-Fi
        }
      }
    }
    getTimeFromServer();
  }
}

// Получение времени с сервера Piestudio.ru
void getTimeFromServer() {
  HTTPClient http_start;
  http_start.begin("http://piestudio.ru/weather/query.php");
  http_start.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode_start = http_start.POST("secretkey=QueryTime");
  String payload_start = http_start.getString();
  server_time = payload_start.toInt();
  setTime(server_time);
  // Присвоение переменных
  tHour = hour();     // Час в переменную
  tMinute = minute(); // Минута в переменную
  tSecond = second(); // Секунда в переменную
}
