// For measuring temperature and humidity in a dry box.
// Gently tweaked by Lawton Willingham
//
// Based on
// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
// Adafruit Copyright INFO at BOTTOM

/************************** Configuration ***********************************/


#include "config.h"
#include "Adafruit_Si7021.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_SPIDevice.h"

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("drybox.temperature");
AdafruitIO_Feed *humidity = io.feed("drybox.humidity");
AdafruitIO_Feed *digital = io.feed("drybox.heater-toggle");
AdafruitIO_Feed *battery = io.feed("drybox.battery");

Adafruit_Si7021 sensor = Adafruit_Si7021();

void handleMessage(AdafruitIO_Data *data) {

  if (data->toPinLevel() == HIGH) {
    sensor.heater(true);
    Serial.println(sensor.isHeaterEnabled() == true  ? "Heater is on." : "Heater is off.");
    delay(HEATER_ON_TIME);
    sensor.heater(false);
    digital->save(false);


  } 
  
  Serial.println(sensor.isHeaterEnabled()  ? "Heater is on." : "Heater is off.");
  
}


int battery_level() {

  // read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // our 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).

  int level = 0;

  for(int i = 0; i < BATTERY_ADC_SAMPLES + 1; i++){
    int temp = analogRead(BATTERY_ADC_PIN);

    level = ((level * i) + temp) / (i + 1);

    delay(BATTERY_ADC_SAMPLE_DELAY);
  }

  // convert battery level to percent
  level = map(level, MIN_ANALOG_READING, MAX_ANALOG_READING, MIN_BATTERY_CAPACITY, MAX_BATTERY_CAPACITY);

  Serial.print("Battery level: ");
  Serial.print(level);
  Serial.println("%");

  return level;
}


void setup() {

  pinMode(GPIO_POWER_SI7021, OUTPUT);
  digitalWrite(GPIO_POWER_SI7021, HIGH);

  // start the serial connection
  Serial.begin(SERIAL_RATE);

  // wait for serial monitor to open
  while (!Serial)
    ;

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  digital->onMessage(handleMessage);
  digital->get();

  sensor.begin();
  sensor.heater(false);

  // 9.18ma typical current draw
  // safe for digital pin
  sensor.setHeatLevel(SI_HEATLEVEL_LOW);
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  int level = battery_level();
  Serial.println(battery->save(level) ? "Success! Battery feed updated." : "Failed to update Battery Feed");

  io.run();
  float celsius = sensor.readTemperature();
  Serial.println(temperature->save(celsius) ? "Success! Temperature feed updated." : "Failed to update Temperature Feed");
 
  io.run();
  float sensorHumidity = sensor.readHumidity();
  Serial.println(humidity->save(sensorHumidity) ? "Success! Humidity feed updated." : "Failed to update Humidity Feed");

  io.run();
  digitalWrite(GPIO_POWER_SI7021, LOW);
  
  io.run();

  // serial console 
  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");
  Serial.print("humidity: ");
  Serial.print(sensorHumidity);
  Serial.println("%");
  


  // Deepsleep to save battery life
  Serial.println("zzzz");
  ESP.deepSleep(DEEPSLEEP_INTERVAL);
}

// Based on
// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.