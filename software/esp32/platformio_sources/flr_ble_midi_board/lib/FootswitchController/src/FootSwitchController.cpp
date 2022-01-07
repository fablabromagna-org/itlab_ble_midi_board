#include <FootSwitchController.h>
#include <Arduino.h>
#include <default_config.h>

const char *FootSwitchController::FS_CONFIG_FILENAME = "/fs_config.bin";


FootSwitchController::FootSwitchController() {
    // jsonConfiguration = new DynamicJsonDocument(2048);
    
    t_last_repeat_cmd = 0;
    
    loadBinaryConfiguration();
}


/** Load bin fs configuration from storage, if exists **/
bool FootSwitchController::loadBinaryConfiguration() {
    bool ret = false;
    uint8_t bin_config[sizeof(controllerConfiguration)];
    size_t bin_config_len;

    Serial.println("Load binary config ####");

    if (FLASH.exists(FS_CONFIG_FILENAME)) {
        try {
            
            File fsCfgFile = FLASH.open(FS_CONFIG_FILENAME, FILE_READ);

            bin_config_len = fsCfgFile.read(bin_config, sizeof(controllerConfiguration));    

            if (bin_config_len <= sizeof(controllerConfiguration)) ret = true;
            else {
                Serial.print("bad size from FILE: ");
                Serial.println(bin_config_len); 
                Serial.print("struct size : ");
                Serial.println(sizeof(controllerConfiguration)); 
            }

        }
        catch (...) {}
    }

    if (ret) {
        Serial.println("PROCESS CONFIG from FILE");
        ret = processBinaryConfiguration(bin_config, bin_config_len, false);
    }
    else {
        Serial.println("PROCESS DEFAULT CONFIG ! ");
        ret = processBinaryConfiguration(default_bin_config, sizeof(default_bin_config), false);
    }
    
    return ret;
}

bool FootSwitchController::processBinaryConfiguration(uint8_t* bin_config, size_t bin_config_len, bool save_flash) {

    //TODO: aggiungere check su dimensione e byte di controllo
    bool ret = false;

    if (bin_config_len <= sizeof(controllerConfiguration)) {

        
        memcpy(&controllerConfiguration, bin_config, bin_config_len);

        if (save_flash) {
            /** save to storage **/
            // if (FLASH.exists(FS_CONFIG_FILENAME))
            // {
            //     FLASH.remove(FS_CONFIG_FILENAME);
            // }

            File fsCfgFile = FLASH.open(FS_CONFIG_FILENAME, FILE_WRITE);
            if (!fsCfgFile) {
                 Serial.println("Failed to open config file");
                 return false;
            }
            else {
                fsCfgFile.write(bin_config, bin_config_len);
                fsCfgFile.close();
                ret = true;
            }
        }
        //controllerConfiguration = (ControllerConfiguration)bin_config;

        // memcpy(&controllerConfiguration, bin_config, 36);
        // memcpy(&controllerConfiguration.footSwitchConfiguration[0], bin_config+36, 1);
        // memcpy(&controllerConfiguration.footSwitchConfiguration[0].config_tap, bin_config+37, 20);
        
        // Serial.println("BIN CONFIG");


        // Serial.println(sizeof(controllerConfiguration));
        // Serial.println(sizeof(controllerConfiguration.footSwitchConfiguration));

        // Serial.println(sizeof(controllerConfiguration.footSwitchConfiguration[0].config_tap));
        // Serial.println(sizeof(FootSwitchConfigurationDetail));
        // Serial.println("--------");


        // Serial.println(bin_config_len);
        // Serial.println(controllerConfiguration.ble_mode);
        // // Serial.println(controllerConfiguration.footSwitchConfiguration[0].footswitch_idx);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[0].config_tap.start);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[0].config_tap.event);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[0].config_tap.midi_ch);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[0].config_tap.midi_type);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[0].config_tap.midi_nr);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[0].config_tap.midi_value_on);

        // Serial.println("------- 2 --------");

        // Serial.println(controllerConfiguration.footSwitchConfiguration[1].config_tap.start);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[1].config_tap.event);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[1].config_tap.midi_ch);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[1].config_tap.midi_type);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[1].config_tap.midi_nr);
        // Serial.println(controllerConfiguration.footSwitchConfiguration[1].config_tap.midi_value_on);
        
    }
    return ret;
}


uint8_t* FootSwitchController::getBinConfiguration() {
    return (uint8_t*) (&controllerConfiguration);
}

MidiHelper::MidiMessage FootSwitchController::processEvent(uint8_t fs_id, FootSwitch::FootSwitchEvent event){
    static MidiHelper::MidiMessage ret_message;
    FootSwitchConfigurationDetail config_detail;
    config_detail.midi_type = MIDI_TYPE_NONE;
    ret_message.length = 0;

    //TODO: check anche dell'indice massimo!
    if (fs_id > 0) {
        Serial.println("Process event fs");
        Serial.println(fs_id);

        if (event == FootSwitch::FootSwitchEvent::FS_TAP) {
            Serial.println("is a TAP event");
            
            config_detail = this->controllerConfiguration.footSwitchConfiguration[fs_id-1].config_tap;
            // Serial.println(config_detail.midi_type);
            // Serial.println(config_detail.midi_nr);
        }
        else if (event == FootSwitch::FootSwitchEvent::FS_HOLD) {
            if (controllerConfiguration.footSwitchConfiguration[fs_id-1].config_hold.event == EVENT_REPEAT) {

            }
            else {
                config_detail = this->controllerConfiguration.footSwitchConfiguration[fs_id-1].config_hold;
            }




            if (millis() - t_last_repeat_cmd >= controllerConfiguration.footSwitchConfiguration[fs_id-1].config_hold.repeat_interval*10 ) {
                t_last_repeat_cmd = millis();
                Serial.println("is an HOLD event");
                config_detail = this->controllerConfiguration.footSwitchConfiguration[fs_id-1].config_hold;
            }
        }


        uint8_t midi_val = 0x00;
        MidiHelper::MidiMessageType midi_type;
        if (config_detail.midi_type == ConfigMidiType::MIDI_TYPE_CC) {
            midi_type = MidiHelper::MidiMessageType::MIDI_CC;
        }
        else if (config_detail.midi_type == ConfigMidiType::MIDI_TYPE_PC) {
            midi_type = MidiHelper::MidiMessageType::MIDI_PC;
        }
        else if (config_detail.midi_type == ConfigMidiType::MIDI_TYPE_NONE) {
            midi_type = MidiHelper::MidiMessageType::MIDI_NONE;
        }

        switch (config_detail.event) {
        case FootSwitchController::ConfigEvent::EVENT_SINGLE:
        case FootSwitchController::ConfigEvent::EVENT_REPEAT:
            midi_val = config_detail.midi_value_on;
            break;
        
        default:
            midi_val = 99;
            break;
        }

        if (!MidiHelper::buildMidiCommand(midi_type, config_detail.midi_ch, config_detail.midi_nr, midi_val , &ret_message) ) {
            ret_message.length = 0;
        }

        // Serial.print("MSG MIDI :");
        // Serial.print(midi_type);
        // Serial.print(config_detail.midi_nr);
        // Serial.println(midi_val);
        // Serial.println(ret_message.length);
    }
    return ret_message;

}




bool FootSwitchController::isValid() {return valid_config;}
