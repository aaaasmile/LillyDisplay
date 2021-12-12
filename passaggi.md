# LilyGo display 
Uso la scheda  di Lilygo 
LILYGO® TTGO T5 V2.0 WiFi Wireless Module bluetooth Base ESP-32 ESP32 2.13 e-Paper
comprata su https://www.banggood.com/

Per settare questo progetto sono partito da un nuovo progetto in PlatformIO
Poi ho aggiunto la library GxEPD2.

Il sorgente sono partito dall'esempio che si trova su:
https://github.com/ZinggJM/GxEPD2/tree/master/examples/GxEPD2_Example
Ho dovuto cambiare il file GxEPD2_display_selection.h
per selezionare il display necessario che è:
GxEPD2_213_B74

Ho avuto problemi a compilare Adafruit con questo errore:
Adafruit_GrayOLED.h:30:32: fatal error: Adafruit_I2CDevice.h: No such file or directory
Che ho risolto mettendo questa linea nel mio main.cpp (oltre ad includere arduino.h)
#include <Adafruit_I2CDevice.h>
Direi che questo progetto può essere usato per avere un display a modo, per esempio un qr code.

## Display
Il display che ho a dispozione è un epaper da 2.3" che ha queste dimensioni:
display.epd2.WIDTH: 128, height: 128, width: 250
La scritta "Hello World!" con il font mono ha questo getTextBounds
Text bounds. tbx : 1, tby: -10, tbw: 127, tbh: 11

## QR
Per vedere il contenuto di un qr code si usa:
zbarimg --raw impf-qr-code.png
Per vedere il significato di un QR code Covid:
https://gir.st/blog/greenpass.html
Documentazione del payload:
https://ec.europa.eu/health/sites/default/files/ehealth/docs/covid-certificate_json_specification_en.pdf

## Web Server
Ho creato un access point per cambiare le linee di testo. Non sono riuscito a fare andare il server
che ho usato nel player. Allora ho provato con questo e funziona alla grande:
https://github.com/me-no-dev/ESPAsyncWebServer
