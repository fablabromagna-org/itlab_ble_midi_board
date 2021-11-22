#ifndef __OTA__
#define __OTA__

#include <Arduino.h>
#define FORMAT_SPIFFS_IF_FAILED true
#define FORMAT_FFAT_IF_FAILED true

#define USE_SPIFFS //comment to use FFat

#ifdef USE_SPIFFS
#include <SPIFFS.h>
#define FLASH SPIFFS
#define FASTMODE false //SPIFFS write is slow
#else
#define FLASH FFat
#define FASTMODE true //FFat is faster
#endif
using namespace std;
class OtaManager
{
private:
    /* data */
    int totalParts;
    int processedParts = 0;
    static const char *OTA_FILENAME;
    void writeOtaPacket(const char *bytes, int size);
    void updateSystem();
    function<void(int)> onProgessUpdate;
    function<void(bool, int)> onOtaFinished;
    static const int INFO_PACKET_COMMAND = 0xFF;
    static const int NEXT_PACKET = 0xFC;
    static const int LAST_PACKET = 0xFE;

public:
    OtaManager(function<void(bool, int)> onOtaFinished, function<void(int)> onProgessUpdate = NULL);
    void onOTADataReceived(string data);

    enum OTA_ERROR
    {
        NOT_ENOUG_SPACE,

    };
};

#endif
