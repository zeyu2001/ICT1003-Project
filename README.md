# HealthBuddy

![Web GUI](static/screenshot.png)

## About
HealthBuddy is a wearable fitness tracker application built for [TinyCircuits](https://tinycircuits.com/products/tinyscreen-smart-watch-kit?variant=14982167303). 

### Key Features
- Uses the BMA250 accelerometer to calculate your steps taken, and detect the surrounding temperature.
- Install our [Android app](/android_app/ICT1003.apk) to pair with your mobile phone through Bluetooth LE and see statistics in real time.
- Sync with a web server, and display a QR code for others to view your statistics.

### Required TinyCircuits Libraries
- [TinyScreen](https://github.com/TinyCircuits/TinyCircuits-TinyScreen_Lib)
- [STBLE](https://github.com/TinyCircuits/TinyCircuits-TinyShield-BLE-ASD2116/blob/master/examples/STBLE.zip)
- [Wireling](https://github.com/TinyCircuits/TinyCircuits-Wireling-Lib)
- [BMA250](https://github.com/TinyCircuits/TinyCircuits-BMA250-library)

### Required Arduino Libraries
- Wire
- SPI
- RTCZero

## Project Structure
The web GUI is a Python web application built with Flask, and is deployed on Heroku, at https://ict1003.herokuapp.com/. 

The `/android_app` directory contains the application `.apk`, as well as the MIT App Inventor source code (`.aia`) and extensions used.

The `/project` directory contains the Arduino source code to be uploaded to the TinyCircuits hardware.

## License
Use of this project is governed by the [MIT License](LICENSE).

## Contributors

- Zhang Zeyu
- Lim Yong Jun
- Tan Wei Lun Nickie
- Jared Marc Song Kye-Jet
- Lee Kai Xuan

This project is an assignment submission in partial fulfillment of the Singapore Institute of Technology (SIT) module ICT1003 Computer Organization and Architecture. 
