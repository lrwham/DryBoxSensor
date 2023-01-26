# Dry Box Sensor
I'm curious how effective the silica dessicant packs are in combination with a few different types of plastic boxes. I intend to setup a few of these sensors in a few combinations of boxes to see the results.

I have some typical boxes with snap on lids as well as a few boxes with gaskets that claim to be waterproof (not airtight).
# Thanks to Adafruit
All of this is fantastically easy thanks to the hard work of the community and the people at Adafruit!
All of the code here is based off their example sketch for temperature and humidity data logging with Adafruit IO.
# What have I done....
* added battery level reporting to a feed
* added a feed for remote toggling the heater
* added deep sleep for increased battery life
* used a digital pin to power the temperature sensor ON for readings and OFF to save power.
    * the sensor lists 150 μA active current
    * [Datasheet](https://cdn-learn.adafruit.com/assets/assets/000/035/931/original/Support_Documents_TechnicalDocs_Si7021-A20.pdf)
# Gotchas
Run ```io.run()``` **often**. You won't get an error message if a feed fails to update with ```feed->save()```

```io.run()``` seems to keep things alive so the feed is **more likely** to update.
## secretConfig.h
You must create a secretConfig.h file.

```C
#ifndef secretConfig_h
#define secretConfig_h

// or if you need your Adafruit IO key.
#define IO_USERNAME ""
#define IO_KEY ""

/**************************** WiFi Config ***********************************/
#define WIFI_SSID ""
#define WIFI_PASS ""

#endif
```