#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>
#include <STBLE.h>

// Library must be passed the board type
// TinyScreenDefault for TinyScreen shields
// TinyScreenAlternate for alternate address TinyScreen shields
// TinyScreenPlus for TinyScreen+
TinyScreen display = TinyScreen(TinyScreenDefault);

// Adapted from: TinyCircuits ST BLE TinyShield UART Example Sketch
// https://learn.tinycircuits.com/Communication/Bluetooth-Low-Energy_TinyShield_Tutorial/#upload-the-example-code

//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

char *rx = NULL;

void setup() {
    SerialMonitorInterface.begin(9600);
    while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
    BLEsetup();

    // Initialize TinyScreen display
    Wire.begin();
    display.begin();
    display.setBrightness(10);
}

void buttonLoop() {
    display.setCursor(0, 0);
    // getButtons() function can be used to test if any button is pressed, or used like:
    // getButtons(TSButtonUpperLeft) to test a particular button, or even like:
    // getButtons(TSButtonUpperLeft|TSButtonUpperRight) to test multiple buttons
    // results are flipped as you would expect when setFlip(true)
    if (display.getButtons(TSButtonUpperLeft)) {

        // Display the current QR plaintext

        if (rx) {
            display.clearScreen();
            display.setCursor(48 - (display.getPrintWidth(rx) / 2), 32);
            display.setFont(thinPixel7_10ptFontInfo);
            display.println(rx);
        }

    }
    display.setCursor(0, 54);
    if (display.getButtons(TSButtonLowerLeft)) {
        display.println("Pressed!");
    }
    display.setCursor(95 - display.getPrintWidth("Pressed!"), 0);
    if (display.getButtons(TSButtonUpperRight)) {

        // Go back to main display
        display.clearScreen();
        generate_qr_code(rx);

    }
    display.setCursor(95 - display.getPrintWidth("Pressed!"), 54);
    if (display.getButtons(TSButtonLowerRight)) {
        display.println("Pressed!");
    }
}

void loop() {
    aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
    if (ble_rx_buffer_len) {//Check if data is available

        // RX data
        rx = (char *) ble_rx_buffer;

        // Print to Serial Monitor
        SerialMonitorInterface.print(ble_rx_buffer_len);
        SerialMonitorInterface.print(" : ");
        SerialMonitorInterface.println(rx);

        display.clearScreen();

        // Generate QR code based on RX input
        generate_qr_code((char *) ble_rx_buffer);

        ble_rx_buffer_len = 0;//clear after reading

    }
    if (SerialMonitorInterface.available()) {//Check if serial input is available to send
        delay(10);//should catch input
        uint8_t sendBuffer[21];
        uint8_t sendLength = 0;
        while (SerialMonitorInterface.available() && sendLength < 19) {
            sendBuffer[sendLength] = SerialMonitorInterface.read();
            sendLength++;
        }
        if (SerialMonitorInterface.available()) {
            SerialMonitorInterface.print(F("Input truncated, dropped: "));
            if (SerialMonitorInterface.available()) {
                SerialMonitorInterface.write(SerialMonitorInterface.read());
            }
        }
        sendBuffer[sendLength] = '\0'; //Terminate string
        sendLength++;
        if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t *) sendBuffer, sendLength)) {
            SerialMonitorInterface.println(F("TX dropped!"));
        }
    }

    buttonLoop();
}
