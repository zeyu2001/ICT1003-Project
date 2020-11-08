#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>
#include <qrcode.h>

// Library must be passed the board type
// TinyScreenDefault for TinyScreen shields
// TinyScreenAlternate for alternate address TinyScreen shields
// TinyScreenPlus for TinyScreen+
TinyScreen display = TinyScreen(TinyScreenDefault);

void setup() {

    // Initialize TinyScreen display
    Wire.begin();
    display.begin();
    display.setBrightness(10);
  
    // Create the QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, "HELLO WORLD I LOVE KAI HUAN <3");

    // Initialize cursor
    display.setCursor(0,0);

    // Center-align QR code
    uint8_t x0 = 48 - (qrcode.size);
    uint8_t y0 = 32 - (qrcode.size);

    // Draw QR code (2:1 ratio)
    for (uint8_t y = 0; y < qrcode.size; y++) 
    {
        for (uint8_t x = 0; x < qrcode.size; x++) 
        {
            if (qrcode_getModule(&qrcode, x, y)) 
            {
                display.drawPixel(x0 + 2 * x, y0 + 2 * y, TS_8b_White);
                display.drawPixel(x0 + 2 * x, y0 + 2 * y + 1, TS_8b_White);
                display.drawPixel(x0 + 2 * x + 1, y0 + 2 * y, TS_8b_White);
                display.drawPixel(x0 + 2 * x + 1, y0 + 2 * y + 1, TS_8b_White);
            } 
            else 
            {
                display.drawPixel(x0 + 2 * x, y0 + 2 * y, TS_8b_Black);
                display.drawPixel(x0 + 2 * x, y0 + 2 * y + 1, TS_8b_Black);
                display.drawPixel(x0 + 2 * x + 1, y0 + 2 * y, TS_8b_Black);
                display.drawPixel(x0 + 2 * x + 1, y0 + 2 * y + 1, TS_8b_Black);
            }
        }
    }
}

void loop() {
    
}
