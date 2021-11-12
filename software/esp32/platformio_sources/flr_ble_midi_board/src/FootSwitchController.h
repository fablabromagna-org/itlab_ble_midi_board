#ifndef FOOTSWITCH_CONTROLLER_H
#define FOOTSWITCH_CONTROLLER_H

#include <MidiHelper.h>
#include <ArduinoJson.h>
#include <FootSwitch.h>

# define VARS_NR 4
# define GROUPS_NR 4
# define FS_NR_MAX 8

class FootSwitchController {

    typedef enum ConfigBLEMode {
        BLE_NONE,
        BLE_SERVER,
        BLE_CLIENT
    } ConfigBLEMode;


    typedef enum ConfigEvent {
        EVENT_NONE,
        EVENT_SINGLE,
        EVENT_REPEAT,
        EVENT_INCREMENT,
        EVENT_ONOFF
    } ConfigEvent;

    typedef enum ConfigMidiType {
        MIDI_TYPE_NONE,
        MIDI_TYPE_CC,
        MIDI_TYPE_PC
    } ConfigMidiType;


public:
    typedef struct FootSwitchConfigurationDetail {
        uint8_t start = 0x80;
        uint8_t event = 0x00;
        uint8_t midi_ch;
        uint8_t midi_type;
        uint8_t midi_nr;
        uint8_t midi_value_on;
        uint8_t midi_value_off;
        uint16_t repeat_interval;
        uint8_t group_idx;
        uint8_t intval_idx;
        uint8_t intval_min;
        uint8_t intval_max;
        int8_t  intval_step;
        uint8_t cycle;
        uint8_t end = 0x88;
    } FootSwitchConfigurationDetail;

    typedef struct FootSwitchConfiguration {
        uint8_t footswitch_idx;
        FootSwitchConfigurationDetail  config_tap;
        FootSwitchConfigurationDetail  config_hold;
    } FootSwitchConfiguration;

    typedef struct ControllerConfiguration {
        char ble_name[32] = "FLR BLE MIDI Controller";
        uint16_t version;
        uint8_t  footswitch_nr = 0;
        uint8_t ble_mode;
        FootSwitchConfiguration footSwitchConfiguration[FS_NR_MAX];

        //TODO: valutare se gestire array dinamico sulla base di footswitch_nr
    } ControllerConfiguration;


    FootSwitchController(void);
    bool processBinaryConfiguration(char* bin_config, size_t bin_config_size);

    bool processJsonConfiguration(char* json_config_str);
    MidiHelper::MidiMessage processEvent(uint8_t fs_id, FootSwitch::FootSwitchEvent event);
    MidiHelper::MidiMessage processEvent1(uint8_t fs_id, FootSwitch::FootSwitchEvent event);
    bool isValid();

private:
    DynamicJsonDocument *jsonConfiguration;
    ControllerConfiguration controllerConfiguration;

    uint8_t midiVarsValues[VARS_NR];
    uint8_t midiGroups[GROUPS_NR];

    bool valid_config;
    
};

#endif