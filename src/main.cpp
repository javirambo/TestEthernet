
/****************************************************************************************************************************
  HelloServer.ino

  For Ethernet shields using ESP32_SC_ENC (ESP32_S2/3, ESP32_C3 + LwIP ENC28J60)

  WebServer_ESP32_SC_ENC28J60 is a library for the ESP32_S2/3, ESP32_C3 with LwIP Ethernet ENC28J60

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_ESP32_SC_ENC
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

#if !(defined(ESP32))
    #error This code is designed for (ESP32 + ENC28J60) to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#define DEBUG_ETHERNET_WEBSERVER_PORT Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_ 3

//////////////////////////////////////////////////////////
// esto es de EffiCast
#define SPI_MOSI_GPIO      GPIO_NUM_7
#define SPI_SCLK_GPIO      GPIO_NUM_15
#define LORA_SPI_MISO_GPIO GPIO_NUM_6

// Optional values to override default settings
#define ETH_SPI_HOST  SPI2_HOST
#define SPI_CLOCK_MHZ 20

// Must connect INT to GPIOxx or not working
#define INT_GPIO  2

#define MISO_GPIO LORA_SPI_MISO_GPIO
#define MOSI_GPIO SPI_MOSI_GPIO
#define SCK_GPIO  SPI_SCLK_GPIO
#define CS_GPIO   1

//////////////////////////////////////////////////////////

#include <WebServer_ESP32_SC_ENC.h>

WebServer server(80);

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC 20

byte mac[][NUMBER_OF_MAC] = {
    {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05},
    {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A},
    {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F},
    {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12}, {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13}, {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14},
};

// Select the IP address according to your local network
// IPAddress myIP(192, 168, 100, 100); // poner IP para IP fija
IPAddress myGW(192, 168, 100, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
// IPAddress myDNS(8, 8, 8, 8); // esto tambien es para IP fija

void handleRoot()
{
    String html = F("Hello from HelloServer running on ");

    html += String(BOARD_NAME);

    server.send(200, F("text/plain"), html);
}

void handleNotFound()
{
    String message = F("File Not Found\n\n");

    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");

    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, F("text/plain"), message);
}

void setup()
{
    Serial.begin(115200);

    while (!Serial && (millis() < 5000));

    delay(500);

    Serial.print(F("\nStart HelloServer on "));
    Serial.print(ARDUINO_BOARD);
    Serial.print(F(" with "));
    Serial.println(SHIELD_TYPE);
    Serial.println(WEBSERVER_ESP32_SC_ENC_VERSION);

    ET_LOGWARN(F("Default SPI pinout:"));
    ET_LOGWARN1(F("SPI_HOST:"), ETH_SPI_HOST);
    ET_LOGWARN1(F("MOSI:"), MOSI_GPIO);
    ET_LOGWARN1(F("MISO:"), MISO_GPIO);
    ET_LOGWARN1(F("SCK:"), SCK_GPIO);
    ET_LOGWARN1(F("CS:"), CS_GPIO);
    ET_LOGWARN1(F("INT:"), INT_GPIO);
    ET_LOGWARN1(F("SPI Clock (MHz):"), SPI_CLOCK_MHZ);
    ET_LOGWARN(F("========================="));

    ///////////////////////////////////

    // To be called before ETH.begin()
    ESP32_ENC_onEvent();  // setea el WiFiEvent para recibir eventos de red.

    // start the ethernet connection and the server:
    // Use DHCP dynamic IP and random mac
    // bool begin(int MISO_GPIO, int MOSI_GPIO, int SCLK_GPIO, int CS_GPIO, int INT_GPIO, int SPI_CLOCK_MHZ,
    //           int SPI_HOST, uint8_t *W6100_Mac = W6100_Default_Mac);
    //   ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST );
    ETH.begin(MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[millis() % NUMBER_OF_MAC]);

    // Static IP, leave without this line to get IP via DHCP
    // bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
    // ETH.config(myIP, myGW, mySN, myDNS);

    // ESP32_ENC_waitForConnect();  // espera tener IP...

    ///////////////////////////////////
    Serial.println("Cuando se conecte a la red levanta el server...");
}

void SetupServer()
{
    server.on(F("/"), handleRoot);
    server.on(F("/inline"), []() { server.send(200, F("text/plain"), F("This works as well")); });
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.print(F("HTTP EthernetWebServer is @ IP : "));
    Serial.println(ETH.localIP());
}

void loop()
{
    server.handleClient();

    static bool noConectado = true;
    if (noConectado && ESP32_ENC_isConnected())
    {
        Serial.println("Se conecto a la red");
        noConectado = false;
        SetupServer();
    }
}
