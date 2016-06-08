#include "application.h"

#define MAC_BYTES 6
#define REPEAT_MAC 16
#define MAGIC_HEADER_LENGTH 6

class WolClient {

  public:
    WolClient(IPAddress _broadcastIP, uint16_t _port);

    int wake(const char* mac);
  private:
    IPAddress broadcastIP;
	uint16_t port;
    void formatIPAddress(const IPAddress& ipAddress, char* target);
	bool parseIPAddress(String string, IPAddress* target);
	void parseMacAddress(const char* string, uint8_t* target);
	uint8_t hex_to_byte(uint8_t h, uint8_t l);
};