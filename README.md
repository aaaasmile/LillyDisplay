# Lilly Display
This is a simple PlatformIO project for displaing text and QR code on the Lilly Go device.
The LillyGo is a ESP32 device with 2.3" epaper display.

I use it to print the Green Pass and also a visit card with a whatsup QR link.

![Device](https://github.com/aaaasmile/LillyDisplay/blob/main/doc/IMG_0634.JPG?raw=true)

## Compile
Please copy the inidata.inc_template file into the file inidata.inc and change the data.

## Changing Text and QR code
You can change text and QR code using the file inidata.inc at compile time.
You can change the text on runtime using the integrated access point "Lilly-AP".
The password is a dummy one coded into the file WebServer.cpp.

The IP Address of the AP is 192.168.4.1.
The Whatsup QR code is created using the Line 5 and it should be a phone number. 