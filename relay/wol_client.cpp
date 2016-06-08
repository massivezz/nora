#include "wol_client.h"

WolClient::WolClient(IPAddress _broadcastIP, uint16_t _port){
    broadcastIP = _broadcastIP;
    port = _port;
}

int WolClient::wake(const char* mac) {
    uint8_t contents[MAGIC_HEADER_LENGTH + REPEAT_MAC * MAC_BYTES];
    uint8_t rawMac[MAC_BYTES];

    parseMacAddress(mac, rawMac);

    UDP udp;
    udp.begin(port);
    udp.beginPacket(broadcastIP, port);

    for (int i = 0; i < MAGIC_HEADER_LENGTH; i++) {
        contents[i] = 0xFF;
    }
    for (uint8_t i = MAGIC_HEADER_LENGTH; i < sizeof contents; i++) {
        contents[i] = rawMac[(i - MAGIC_HEADER_LENGTH) % MAC_BYTES];
    }

    udp.write(contents, sizeof contents);

    udp.endPacket();
    udp.stop();

    return TRUE;
}

void WolClient::formatIPAddress(const IPAddress& ipAddress, char* target) {
    String ip(ipAddress[0]);
    for (int i = 1; i < 4; ++i)
    {
        ip.concat(".");
        ip.concat(ipAddress[i]);
    }
    ip.toCharArray(target, ip.length() + 1);
}

bool WolClient::parseIPAddress(String string, IPAddress* target) {
    uint8_t values[4] = { 0, 0, 0, 0 };
    int prevIndex = -1;
    for (int i = 0; i < 4; ++i)
    {
        int dotIndex = string.indexOf('.', prevIndex + 1);
        if (dotIndex < 0)
        {
            if (i != 3)
            {
                return false;
            }
            dotIndex = string.length();
        }
        values[i] = string.substring(prevIndex + 1, dotIndex).toInt();
        prevIndex = dotIndex;
    }
    *target = IPAddress(values);
    return true;
}

void WolClient::parseMacAddress(const char* string, uint8_t* target) {
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t max = 17; // MAC String is 17 characters.
    while (i < max) {
        target[j++] = hex_to_byte(string[i], string[i + 1]);
        i += 3;
    }
}

uint8_t WolClient::hex_to_byte(uint8_t h, uint8_t l) {
    uint8_t retval = 0x00;

    // higher nibble
    if (h >= 0x30 && h <= 0x39) { // 0-9
        retval |= (h - 0x30) << 4;
    }

    if (h >= 0x41 && h <= 0x46) { // A-F
        retval |= (h - 0x41 + 0x0A) << 4;
    }

    if (h >= 0x61 && h <= 0x66) { // a-f
        retval |= (h - 0x61 + 0x0A) << 4;
    }

    // lower nibble
    if (l >= 0x30 && l <= 0x39) { // 0-9
        retval |= l - 0x30;
    }

    if (l >= 0x41 && l <= 0x46) { // A-F
        retval |= l - 0x41 + 0x0A;
    }

    if (l >= 0x61 && l <= 0x66) { // a-f
        retval |= l - 0x61 + 0x0A;
    }

    return retval;
}
