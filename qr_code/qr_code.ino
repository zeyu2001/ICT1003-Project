#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>
#include <qrcode.h>



void generate_qr_code(char *qr_target) {

    // Initialize TinyScreen display
    Wire.begin();
    display.begin();
    display.setBrightness(10);
  
    // Create the QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, qr_target);

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
