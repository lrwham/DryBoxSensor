# Thanks to Adafruit
All of this is fantastically easy thanks to the hard work of the community and the people at Adafruit!
All of the code here is based off their example sketch for temperature and humidity data logging with Adafruit IO.
# What have I done....
* added battery level reporting to a feed
* added a feed for remote toggling the heater
* added deep sleep for increased battery life
# Gotchas
Run ```io.run()``` **often**
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