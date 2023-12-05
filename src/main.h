// includes
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_APDS9960.h>
#include "EspMQTTClient.h"


// display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// SPI display
#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 27
#define OLED_CS 5
#define OLED_RESET 17
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// gesture sensor
#define INT_PIN 26 // Needs to be an interrupt pin
Adafruit_APDS9960 apds;

//screen variable
#define MAIN_SCREEN 1
#define MENU 2
#define HOW_TO 3
#define SET_VALUE 4
#define CONFIRM_VALUE 5

void main_screen();
void menu();
void how_to();
void set_value();
void confirm_value();
void onConnectionEstablished();

