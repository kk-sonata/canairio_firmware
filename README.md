[![Build Status](https://travis-ci.com/kike-canaries/esp32-hpma115s0.svg?branch=master)](https://travis-ci.com/kike-canaries/esp32-hpma115s0) 

# ESP32-HPMA115S0 (CanAirIO sensor)

<a href="https://github.com/kike-canaries/esp32-hpma115s0/blob/master/images/collage.jpg" target="_blank"><img src="https://github.com/kike-canaries/esp32-hpma115s0/blob/master/images/collage.jpg" align="right" width="384" ></a>

<a href="https://play.google.com/store/apps/details?id=hpsaturn.pollutionreporter" target="_blank"><img src="https://github.com/kike-canaries/android-hpma115s0/blob/master/assets/googleplay/gplayicon.png" align="left" width="128" ></a>

Citizen science project with mobile and fixed sensors for measuring air quality (PM 2.5) using low-cost sensors and smartphones. Built with a `ESP32` module board and `HPMA115s0 Honeywell` dust sensor, interfaced with an [CanAirIO Android client app](https://github.com/kike-canaries/android-hpma115s0).

**Full howto guide**:
[CanAirIO guide [English]](https://github.com/kike-canaries/esp32-hpma115s0/wiki/Official-Guide-(EN)) **|** [CanAirIO guide [Spanish]](https://github.com/kike-canaries/esp32-hpma115s0/wiki/Official-Guide-(ES))

## Installation

### Linux and MacOSx

You can download the last firmware version in [releases](https://github.com/kike-canaries/esp32-hpma115s0/releases) section. Download the last release from `assets` section in releases and please uncompress zip file, connect your device and execute the next command for your model board (D1Mini, WemosOLED, Heltec) like this:

``` bash
unzip canairio_rev414_20190829.zip
cd canairio_installer
./install.sh canairio_d1mini_rev414_20190829.bin
```

**Note**: you need python2 or python3 with pyserial in your system.  
**Tip**: if you want clear all preferences and flash variables, please execute before:

``` bash
esptool.py --port /dev/ttyUSB0 erase_flash
```

After that you will able to send OTA updates to any board supported, like this:

``` bash
./install.sh ota canairio_d1mini_rev414_20190829.bin
```

### Windows

Please read procedure on `firmware` section on [HacksterIO Guide](https://www.hackster.io/114723/canairio-red-ciudadana-para-monitoreo-de-calidad-del-aire-96f79a#toc-firmware-y-software-3) for details for load firmware via oficial **Espressif Download Tool** in Windows

## [Optional] Compiling and installing

Please install first [PlatformIO](http://platformio.org/) open source ecosystem for IoT development compatible with **Arduino** IDE and its command line tools (Windows, MacOs and Linux). Also, you may need to install [git](http://git-scm.com/) in your system.

For **default** board `D1Mini Kit like`, clone and upload firmware via USB cable:

``` bash
git clone https://github.com/kike-canaries/esp32-hpma115s0.git
cd esp32-hpma115s0
pio run -e d1mini --target upload
```

After that, it able for sending updates via OTA protocol using Wifi in your LAN, is more fastest than USB and you can disconnect your board, but `you need first send Wifi credentials` via Android CanAirIO app (see below)

For **OTA updates** you only run

``` bash
pio run --target upload
```

**Optional** for other board, please select the right environment for example for `wemos` board:

``` bash
pio run -e wemos --target upload
```

### Building Installer

You can build `CanAirIO Installer` zip package with all binaries of all board flavors running the next command:

``` bash
./build all && ./build installer
```

The directory output is in: `releases/installer`  
Also the binaries flavors directory: `releases/binaries/`

### Troubleshooting

If you have some issues with Bluetooth library internals, or libraries issues, please upgrade all frameworks and tools on PlatformIO:

``` bash
pio update
sudo pio upgrade
pio run -t clean
rm -rf .pio
pio lib update
pio run --target upload
```

## CanAirIO Firmware Usage

From [CanAirIO Android app](https://github.com/kike-canaries/android-hpma115s0) you can connect to your device via Bluetooth and record mobile captures and save tracks on your sdcard. Also you can share these tracks to CanAirIO network. If you want set your device for static station, please configure Wifi and CanAirIO API or InfluxDb server. (see below)

## [Optional] Setup WiFi, CanAirIO API or InfluxDb

The [current firmware](https://github.com/kike-canaries/esp32-hpma115s0/releases) supports setup WiFi crendentials, CanAirIO API or InfluxDb configs via Bluetooth for static statations. You can use the oficial [CanAirIO Android app](https://github.com/kike-canaries/android-hpma115s0) for send these settings to your device or you also can use [nRF Connect app](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp) for the same tasks.

### Option 1: CanAirIO Android App:

Please connect your device via Bluetooth and in the settings section configure parameters like `Sample Time Interval` and `Station Name`. If you want configure our API cloud or a custom influxDb instance too. You can get a username and password of our API on the next [link](http://canairiofront.herokuapp.com/register) and view captures [here](http://gblabs.co:8888/sources/1/dashboards/1)

### Option 2: nRF Connect App:

#### WiFi Credentials

1. Start your sensor with last firmware (rev212)
2. Scan and connect to it with nRF connect App
3. Expand the GATT service item (Unknown Service, ends in aaf3)
4. Click on `upload button` on the `READ,WRITE` characteristic item (ends in ae02)
5. Change value type to `TEXT`
6. Put your credentials on `New Value` field, i.e. like this:
    ```json
    {"ssid":"YourWifiName","pass":"YourPassword"}
    ```
7. Click on `send` button.
8. On your serial messages your sensor will be log succesuful connection or on your display the wifi icon will be enable.

#### Device name (station name)

Repeat previous steps `1 to 6` but the payload for `dname` connection is for example:

```json
"{"dname":"PM25_Berlin_Pankow_E04"}"
```

#### CanAirIO API credentials

Repeat previous steps `1 to 6` and send the next payload with your credentials:

```json
"{"apiusr":"username","apipss":"password"}"
```

#### InfluxDb config

Repeat previous steps `1 to 6` but the payload for `InfluxDb` connection is:

```json
"{"ifxdb":"","ifxip":"","ifxtg":""}"
```

the fields mean:
- **ifxdb**: InfluxDb database name
- **ifxip**: InflusDb hostname or ip
- **ifxtg**: Custom tags **(optional)**

##### Example:

```json
{"ifxdb":"database_name","ifxip":"hostname_or_ip","ifxtg":"zone=north,zone=south"}
```
#### Location config

Repeat previous steps `1 to 6` but the payload for `sensor location` for example is:

```json
"{"lat":52.53819,"lon":13.44024,"alt":220,"spd":34.5}"
```

#### InfluxDb payload

The current version send the next variables to InfluxDb:

```
pm25","pm10,"hum","tmp","lat","lng","alt","spd","stime"
```
- **pm25 and pm10**, from Honeywell sensor (is a average of `stime` samples)
- **hum and tmp**, humidity and temperature if you connect AM2320 to your ESP32
- **lat, lng, alt, spd**, variables that you already configured

## Device status vector

The current flags status is represented on one byte and it is returned on config:

``` java
bit_sensor  = 0;    // sensor fail/ok
bit_paired  = 1;    // bluetooth paired
bit_wan     = 2;    // internet access
bit_cloud   = 3;    // publish cloud
bit_code0   = 4;    // code bit 0
bit_code1   = 5;    // code bit 1
bit_code2   = 6;    // code bit 2
bit_code3   = 7;    // code bit 3

```

The error codes are represented on up four bits. Error code table:

``` java
ecode_sensor_ok          =   0;
ecode_sensor_read_fail   =   1;
ecode_sensor_timeout     =   2;
ecode_wifi_fail          =   3;
ecode_ifdb_write_fail    =   4;
ecode_ifdb_dns_fail      =   5;
ecode_json_parser_error  =   6;
ecode_invalid_config     =   7;
```

sample:

``` java
    00000011 -> sensor ok, device paired
    00001101 -> sensor ok, wan ok, ifxdb cloud ok
    01000101 -> sensor ok, wan ok, ifxdb write fail
```
---

<a href="https://github.com/kike-canaries/esp32-hpma115s0/blob/master/images/rev212.jpg" target="_blank"><img src="https://github.com/kike-canaries/esp32-hpma115s0/blob/master/images/rev212.jpg" align="right" width="384" ></a>

## TODO
- [X] HPMA115S0 fixes and libraries tests
- [X] SSD1306 OLED display output (PM2.5 and PM10)
- [X] Basic output via Bluetooth LE GATT server
- [X] Gson output parser (for [Android client](https://github.com/kike-canaries/android-hpma115s0))
- [X] WeMOS OLED board supported
- [X] Heltec board supported
- [X] D1 MINI Kit OLED board supported
- [X] LaserCut box for D1Mini board
- [X] Config WiFi via Bluetooth
- [X] Config InfluxDb via Bluetooth (without auth for now)
- [X] Config sample time via Bluetooth
- [X] GUI: bluetooth, wifi and cloud status icons 
- [X] Config CanAirIO API via Bluetooth
- [X] Config location via Bluetooth
- [X] enable/disable API, Wifi, influxDB
- [X] OTA updates ready (LAN)
- [ ] OTA updates ready (WAN)
- [ ] ROM storage for offline issues

---

## CanAirIO device HOWTO guide

With the next guides, you will be able to build a device to measure air quality by using a Honeywell HPMA115SO sensor, which measures PM 2.5 and PM 10 particles and then if you want, publish it to CanAirio cloud or a personal server using [CanAirIO App](https://play.google.com/store/apps/details?id=hpsaturn.pollutionreporter)

[CanAirIO guide [English]](https://github.com/kike-canaries/esp32-hpma115s0/wiki/Official-Guide-(EN))  
[CanAirIO guide [Spanish]](https://github.com/kike-canaries/esp32-hpma115s0/wiki/Official-Guide-(ES))

<a href="https://raw.githubusercontent.com/kike-canaries/esp32-hpma115s0/master/images/collage_v2.jpg" target="_blank"><img src="https://github.com/kike-canaries/esp32-hpma115s0/blob/master/images/collage_v2.jpg" height="324" ></a>

