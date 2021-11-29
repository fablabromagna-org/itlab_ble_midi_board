#ifndef MIDI_HELPER
#define MIDI_HELPER
#include <Arduino.h>

class MidiHelper {
public:


    typedef  enum MidiOutput {
        MIDI_OUT_BLE,
        MIDI_OUT_DIN,
        MIDI_OUT_USB

    } MidiOutput;

    typedef  enum MidiMessageType {
        MIDI_CC,
        MIDI_PC,
        MIDI_NOTE_ON,
        MIDI_NOTE_OFF

    } MidiMessageType;


    typedef struct MidiMessage {
        __UINT8_TYPE__ content[10];
        __UINT8_TYPE__ length;
    } MidiMessage;
    
   static bool buildMidiCommand(MidiHelper::MidiMessageType type, __UINT8_TYPE__ midi_channel, __UINT8_TYPE__ midi_cmd_nr, __UINT8_TYPE__ midi_cmd_value, MidiMessage* midi_message) {
         //  TODO: inserire il controllo sul range dei comandi 

        // Serial.println("QUI ARRIVO__2___?");
        // Serial.println(midi_channel);
        // Serial.println(midi_cmd_nr);
        // Serial.println(midi_cmd_value);



        // TODO: gestire meglio check array
        midi_message->content[0] = 0x80;
        midi_message->content[1] = 0x80;
        midi_message->content[2] = 0xB0 + midi_channel -1; // CC on channel midi_channel
        midi_message->content[3] = midi_cmd_nr; // Control number
        midi_message->content[4] = midi_cmd_value;    // Control value
        
        midi_message->length = 5;

        // Serial.println("QUI ARRIVO__3___?");
        //midi_message = midiPacketTemplate;
        return true;
    }
    
 
private:

        



};

#endif