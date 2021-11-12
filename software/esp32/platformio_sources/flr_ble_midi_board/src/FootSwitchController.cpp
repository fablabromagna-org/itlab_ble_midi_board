#include <FootSwitchController.h>

FootSwitchController::FootSwitchController() {
    jsonConfiguration = new DynamicJsonDocument(2048);


}

bool FootSwitchController::processBinaryConfiguration(char* bin_config, size_t bin_config_len) {

    //TODO: aggiungere check su dimensione e byte di controllo

    if (bin_config_len <= sizeof(controllerConfiguration)) {
        memcpy(&controllerConfiguration, bin_config, bin_config_len);
    }

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
    FootSwitchConfigurationDetail config_detail;

    //TODO: check anche dell'indice massimo!
    if (fs_id > 0) {
        if (event == FootSwitch::FootSwitchEvent::FS_TAP) {
            config_detail = this->controllerConfiguration.footSwitchConfiguration[fs_id-1].config_tap;
        }
        else if (event == FootSwitch::FootSwitchEvent::FS_HOLD) {
            config_detail = this->controllerConfiguration.footSwitchConfiguration[fs_id-1].config_hold;
        }

        uint8_t midi_val = 0x00;
        MidiHelper::MidiMessageType midi_type;
        if (config_detail.midi_type == ConfigMidiType::MIDI_TYPE_CC) {
            midi_type = MidiHelper::MidiMessageType::MIDI_CC;
        }
        else if (config_detail.midi_type == ConfigMidiType::MIDI_TYPE_PC) {
            midi_type = MidiHelper::MidiMessageType::MIDI_PC;
        }
        //TODO: gestire anche altre tipologie?

        switch (config_detail.event) {
        case FootSwitchController::ConfigEvent::EVENT_SINGLE:
            midi_val = config_detail.midi_value_on;
            break;
        
        default:
            midi_val = 99;
            break;
        }

        if (!MidiHelper::buildMidiCommand(midi_type, config_detail.midi_ch, config_detail.midi_nr, midi_val , &ret_message) ) {
            ret_message.length = 0;
        }


    }
    return ret_message;

}



MidiHelper::MidiMessage FootSwitchController::processEvent1(uint8_t fs_id, FootSwitch::FootSwitchEvent event){
    
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
       
    }
    return ret_message;
}

bool FootSwitchController::isValid() {return valid_config;}
