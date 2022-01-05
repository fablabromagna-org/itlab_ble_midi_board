#ifndef FOOTSWITCH_CONTROLLER_H
#define FOOTSWITCH_CONTROLLER_H

#include <MidiHelper.h>
// #include <ArduinoJson.h>
#include <FootSwitch.h>

#define USE_SPIFFS //comment to use FFat

#ifdef USE_SPIFFS
#include <SPIFFS.h>
#define FLASH SPIFFS
#define FASTMODE false //SPIFFS write is slow
#else
#define FLASH FFat
#define FASTMODE true //FFat is faster
#endif

#define VARS_NR 4
#define GROUPS_NR 4
#define FS_NR_MAX 8
#define INT_VAR_MAX 4

class FootSwitchController
{

    typedef enum ConfigBLEMode
    {
        BLE_NONE,
        BLE_SERVER,
        BLE_CLIENT
    } ConfigBLEMode;

    typedef enum ConfigEvent
    {
        EVENT_NONE,
        EVENT_SINGLE,
        EVENT_REPEAT,
        EVENT_INCREMENT,
        EVENT_ONOFF
    } ConfigEvent;

    typedef enum ConfigMidiType
    {
        MIDI_TYPE_NONE,
        MIDI_TYPE_CC,
        MIDI_TYPE_PC
    } ConfigMidiType;

public:
    typedef struct FootSwitchConfigurationDetail
    {
        uint8_t start;
        uint8_t event;
        uint8_t midi_ch;
        uint8_t midi_type;
        uint8_t midi_nr;
        uint8_t midi_value_on;
        uint8_t midi_value_off;
        uint8_t repeat_interval;
        uint8_t group_idx;
        uint8_t intval_idx;
        uint8_t positive_step;
        uint8_t intval_step;
        uint8_t end = 0x88;
    } FootSwitchConfigurationDetail;

    typedef struct FootSwitchConfiguration
    {
        // uint8_t footswitch_idx;
        // uint8_t dummy;
        FootSwitchConfigurationDetail config_tap;
        FootSwitchConfigurationDetail config_hold;
    } FootSwitchConfiguration;

    typedef struct InternalVariableConfiguration
    {
        uint8_t val_min;
        uint8_t val_max;
        uint8_t cycle;
    } InternalVariableConfiguration;

    typedef struct ControllerConfiguration
    {
        uint8_t ctrl_char_start;
        char ble_name[32] = "FLR BLE MIDI Controller";
        uint8_t version_major;
        uint8_t version_minor;
        uint8_t ble_mode;
        uint8_t footswitch_nr = 0;
        InternalVariableConfiguration internalVariableConfiguration[INT_VAR_MAX];
        FootSwitchConfiguration footSwitchConfiguration[FS_NR_MAX];

        uint8_t ctrl_char_end;
        //TODO: valutare se gestire array dinamico sulla base di footswitch_nr
    } ControllerConfiguration;

    FootSwitchController(void);
    bool processBinaryConfiguration(uint8_t *bin_config, size_t bin_config_size, bool save_flash);

    uint8_t *getBinConfiguration(size_t *size);
    MidiHelper::MidiMessage processEvent(uint8_t fs_id, FootSwitch::FootSwitchEvent event);
    bool isValid();
    bool loadBinaryConfiguration();

    char *debugThis()
    {
        FootSwitchConfigurationDetail config_detail;

        config_detail = this->controllerConfiguration.footSwitchConfiguration[0].config_tap;

        return (char *)&config_detail;
    }

private:
    // DynamicJsonDocument *jsonConfiguration;
    ControllerConfiguration controllerConfiguration;
    size_t configSize;

    uint8_t midiVarsValues[VARS_NR];
    uint8_t midiGroups[GROUPS_NR];

    bool valid_config;
    static const char *FS_CONFIG_FILENAME;
};

#endif