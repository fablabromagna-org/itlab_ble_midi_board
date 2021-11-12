#include <FootSwitchController.h>

FootSwitchController::FootSwitchController() {
    jsonConfiguration = new DynamicJsonDocument(2048);
}

bool FootSwitchController::processJsonConfiguration(char* json_config_str) {

    DeserializationError err = deserializeJson(*jsonConfiguration, json_config_str);
    if (err = DeserializationError::Ok) {
        this->valid_config = true;
    }
    else {
        this->valid_config = false;
    }
    return this->valid_config;
}


MidiHelper::MidiMessage FootSwitchController::processEvent(uint8_t fs_id, FootSwitch::FootSwitchEvent event){
    
    static MidiHelper::MidiMessage ret_message;

    //TODO: check anche dell'indice massimo!
    if (fs_id > 0) {
        JsonObject myfootswitch = (*jsonConfiguration)["fs_config"][fs_id -1];

        JsonObject tap_config = myfootswitch["tap"];
        
        MidiHelper::MidiMessageType midi_type = (strcmp(tap_config["midi_type"],"cc")==0)?MidiHelper::MidiMessageType::MIDI_CC:MidiHelper::MidiMessageType::MIDI_PC;
        uint8_t midi_ch = tap_config["midi_channel"];
        uint8_t midi_nr = tap_config["midi_nr"];
        uint8_t midi_val;
        if ((strcmp(tap_config["event"],"single")==0)) {
            midi_val = tap_config["midi_value"];
        }
        else {
            //TODO: testare le altre casistiche di evento
            midi_val = 99;
        }

        
        if (!MidiHelper::buildMidiCommand(midi_type, midi_ch, midi_nr, midi_val , &ret_message) ) {
            ret_message.length = 0;
        }

        return ret_message;
    }
}

bool FootSwitchController::isValid() {return valid_config;}
