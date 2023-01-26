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
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include "Adafruit_Si7021.h"


/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include "Adafruit_SPIDevice.h"


// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("dryboxThingPlus.temperature");
AdafruitIO_Feed *humidity = io.feed("dryboxThingPlus.humidity");
AdafruitIO_Feed *digital = io.feed("dryboxThingPlus.heater-toggle");

Adafruit_Si7021 sensor = Adafruit_Si7021();

void handleMessage(AdafruitIO_Data *data) {

  Serial.println("received <- ");
  Serial.println(data->toPinLevel());

  if (data->toPinLevel() == HIGH) {
    Serial.println("Heater ON");
    sensor.heater(1);
    delay(HEATER_ON_TIME);
  } else {
  }

  sensor.heater(0);
  Serial.println("Heater OFF");


  Serial.print(sensor.isHeaterEnabled());
}


void battery_level() {

  // read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // our 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).
  int accumulator = 0;

  for(int i = 0; i < BATTERY_ADC_SAMPLES; i++){
    accumulator += analogRead(A0);
    delay(BATTERY_ADC_SAMPLE_DELAY);
  }

  int level = accumulator / BATTERY_ADC_SAMPLES;

  // convert battery level to percent
  level = map(level, 580, 774, 0, 100);
  Serial.print("Battery level: ");
  Serial.print(level);
  Serial.println("%");

  // grab the battery feed
  AdafruitIO_Feed *battery = io.feed("dryboxThingPlus.battery");

  // send battery level to AIO
  battery->save(level);
  io.run();
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
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  float celsius = sensor.readTemperature();
  float fahrenheit = (celsius * 1.8) + 32;
  float readhumidity = sensor.readHumidity();

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  // save fahrenheit (or celsius) to Adafruit IO
  temperature->save(fahrenheit);

  Serial.print("humidity: ");
  Serial.print(readhumidity);
  Serial.println("%");

  // save humidity to Adafruit IO
  humidity->save(readhumidity);

  battery_level();
  digitalWrite(GPIO_POWER_SI7021, LOW);
  digital->save(0);

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  Serial.println("zzzz");
  ESP.deepSleep(DEEPSLEEP_INTERVAL);
}
