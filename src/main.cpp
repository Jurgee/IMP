/*
 * Project:  Display MQTT ovládaný gesty
 * Author:   Jiří Štípek
 * Login:    xstipe02
 * Created:  2021-12-03
 */

#include "main.h"

// global variables for menues
int screen;
int cursor;
int value;

EspMQTTClient client(
    "Jurge",
    "savojebest",
    "aceautomation.ddns.net", // MQTT Broker server ip
    "aceautomation",
    "ilovetheseproducts",
    "Jurge",              // Client name that uniquely identify your device
    1883                  // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
  // start stuff
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages();                                     // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater();                                        // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA();                                                   // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("Jurge/lastwill", "I am going offline"); // You can activate the retain flag by setting the third parameter to true

  main_screen();

  // gesture sensor
  if (!apds.begin())
  {
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else
  {
    Serial.println("Device initialized!");
  }

  pinMode(INT_PIN, INPUT_PULLUP);

  apds.enableProximity(true);
  apds.enableGesture(true);
  apds.setProximityInterruptThreshold(200, 0);
  apds.enableProximityInterrupt();
}

// the loop function runs over and over again forever
void loop()
{
  client.loop();
  uint8_t gesture = apds.readGesture();
  
  switch (screen)
  {
  case MAIN_SCREEN: // welcome screen
    if (gesture == APDS9960_LEFT)
    {
      Serial.println(">>");
      cursor = 5;
      menu();
    }
    break;

  case MENU: // menu screen
    if (gesture == APDS9960_RIGHT)
    {
      Serial.println("<<");
      main_screen();
    }
    else if (gesture == APDS9960_LEFT)
    {
      Serial.println(">>");
      if (cursor == 5)
      {
        how_to();
      }
      else if (cursor == 20)
      {
        value = 0;
        set_value();
      }
    }
    else if (gesture == APDS9960_DOWN)
    {
      Serial.println("^");
      cursor = 5;
      menu();
    }
    else if (gesture == APDS9960_UP)
    {
      Serial.println("v");
      cursor = 20;
      menu();
    }
    break;

  case HOW_TO: // help how to use
    if (gesture == APDS9960_RIGHT)
    {
      Serial.println("<<");
      menu();
    }
    break;

  case SET_VALUE: // set value screen
    if (gesture == APDS9960_RIGHT)
    {
      Serial.println("<<");
      value = value - 1;
      set_value();
    }
    else if (gesture == APDS9960_LEFT)
    {
      Serial.println(">>");
      value = value + 1;
      set_value();
    }
    else if (gesture == APDS9960_UP)
    {
      Serial.println("v");
      confirm_value();
    }
    else if (gesture == APDS9960_DOWN)
    {
      Serial.println("^");
      menu();
    }
    break;

  case CONFIRM_VALUE: // confirm value screen
    if (gesture == APDS9960_RIGHT)
    {
      Serial.println("<<");
      set_value();
    }
    else if (gesture == APDS9960_UP)
    {
      Serial.println("v");
      client.publish("Jurge/esp32/value", String(value));

      menu();
    }
    break;
  }
}
// move to main screen
void main_screen()
{
  // welcome text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 20);
  display.println("Welcome");

  // underlined text
  display.setTextSize(1);
  display.setCursor(2, 40);
  display.println("---------------------");

  // move left >>
  display.setTextSize(1);
  display.setCursor(60, 50);
  display.println(">");
  display.display();

  screen = MAIN_SCREEN;
}

void menu() // move to menu
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 5);
  display.println("How to use"); // cursor y = 5
  display.setCursor(30, 20);
  display.println("Set value"); // cursor y = 20

  display.setCursor(4, 55);
  display.println("< Home page");

  display.setCursor(15, cursor);
  display.println(">");

  display.display();

  screen = MENU;
}

void how_to() // move to how to use screen
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(15, 5);
  display.println(">");
  display.setCursor(60, 5);
  display.println("Swipe right");

  display.setCursor(15, 20);
  display.println("<");
  display.setCursor(60, 20);
  display.println("Swipe left");

  display.setCursor(15, 35);
  display.println("^");
  display.setCursor(60, 35);
  display.println("Swipe up");

  display.setCursor(15, 50);
  display.println("v");
  display.setCursor(60, 50);
  display.println("Swipe down");

  display.display();

  screen = HOW_TO;
}

void set_value() // set value screen
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 10);
  display.println("-1 <");
  display.setCursor(100, 10);
  display.println("> +1");

  display.setTextSize(2);
  display.setCursor(60, 20);
  display.println(value);

  display.setCursor(5, 40);
  display.println("----------");

  display.setTextSize(1);
  display.setCursor(5, 55);
  display.println("^ Back");

  display.setCursor(95, 55);
  display.println("v Set");
  display.display();

  screen = SET_VALUE;
}

void confirm_value() // confirm value screen or go back to set value screen
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 10);
  display.println("Confirm value ??");

  display.setTextSize(2);
  display.setCursor(60, 20);
  display.println(value);

  display.setCursor(5, 40);
  display.println("----------");

  display.setTextSize(1);
  display.setCursor(5, 55);
  display.println("< NO");

  display.setCursor(95, 55);
  display.println("^ YES");
  display.display();

  screen = CONFIRM_VALUE;
}
void onConnectionEstablished()
{
  client.publish("Jurge/esp32/connected", "true");
  client.subscribe("Jurge/esp32/value", [](const String &payload)
                   { Serial.println(payload); });
}