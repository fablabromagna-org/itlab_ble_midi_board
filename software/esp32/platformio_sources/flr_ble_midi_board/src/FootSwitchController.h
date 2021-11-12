#ifndef FOOTSWITCH_CONTROLLER_H
#define FOOTSWITCH_CONTROLLER_H

#include <MidiHelper.h>
#include <ArduinoJson.h>
#include <FootSwitch.h>

# define VARS_NR 4
# define GROUPS_NR 4

class FootSwitchController {

public:
    FootSwitchController(void);
    bool processJsonConfiguration(char* json_config_str);
    MidiHelper::MidiMessage processEvent(uint8_t fs_id, FootSwitch::FootSwitchEvent event);

    bool isValid();

private:
    DynamicJsonDocument *jsonConfiguration;
    uint8_t midiVarsValues[VARS_NR];
    uint8_t midiGroups[GROUPS_NR];

    bool valid_config;
    
};

#endif