#include "rest_client.h"
#include "wol_client.h"

// Define the pins we're going to call pinMode on
int LED = D7; // This one is the built-in tiny one to the right of the USB jack

#define MAC_BYTES 6
#define REPEAT_MAC 16
#define MAGIC_HEADER_LENGTH 6

uint16_t port = 7;
IPAddress broadcastIP(255, 255, 255, 255);

WolClient wolClient(broadcastIP, port);

IPAddress pingIP;
char szWolState[32];
char szSparkHostAddress[16];

int wakeHost(String param) {
    if (param.length() == 0)
    {
        strcpy(szWolState, "param not provided to wake");
        return FALSE;
    }

    int index = param.indexOf(';');
    if (index == -1 || param.indexOf(';', index + 1) >= 0 || !parseIPAddress(param.substring(0, index), &pingIP))
    {
        strcpy(szWolState, "Invalid arguments");
        return FALSE;
    }

    char szMacAddress[80];
    param.substring(index + 1).toCharArray(szMacAddress, 80);

    return wolClient.wake(szMacAddress);
}

int shutdown(String param) {

}

int heartbeat(String param) {

}

int sendCommand(String param) {

}

void setup() {
    pinMode(LED, OUTPUT);

    strcpy(szWolState, "");
    Spark.variable("state", &szWolState, STRING);

    formatIPAddress(WiFi.localIP(), szSparkHostAddress);
    Spark.variable("address", &szSparkHostAddress, STRING);
    
    Spark.function("wakeHost", wakeHost);
    Spark.function("shutdown", shutdown);
    Spark.function("heartbeat", heartbeat);
    Spark.function("sendCommand", sendCommand);

    RGB.control(true);
    RGB.brightness(20);
}

void loop() {

}