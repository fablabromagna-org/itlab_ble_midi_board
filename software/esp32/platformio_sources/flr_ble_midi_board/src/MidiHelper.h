#ifndef MIDI_HELPER
#define MIDI_HELPER

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

    

    bool buildMidiCommand(MidiHelper::MidiMessageType type, __UINT8_TYPE__ midi_channel, __UINT8_TYPE__ midi_cmd_nr, __UINT8_TYPE__ midi_cmd_value, __UINT8_TYPE__* midi_message) {
         //  TODO: inserire il controllo sul range dei comandi 

  
 
        // TODO: gestiore meglio allocazione array
        
    
    __UINT8_TYPE__ midiPacketTemplate[] = {
    0x80,  // header
    0x80,  // timestamp, not implemented 
    0x00,  // type + channel
    0x3c,  // data nr - key note: 0x3c == 60 == middle c
    0x00   // data value
    };

    /** Send the CC MIDI message ON **/
    midiPacketTemplate[2] = 0xB0 + midi_channel -1; // CC on channel midi_channel
    midiPacketTemplate[3] = midi_cmd_nr; // Control number
    midiPacketTemplate[4] = midi_cmd_value;    // Control value

    midi_message[0] = 0x80;
    midi_message[1] = 0x80;
    midi_message[2] = 0xB0 + midi_channel -1; // CC on channel midi_channel
    midi_message[3] = midi_cmd_nr; // Control number
    midi_message[4] = midi_cmd_value;    // Control value


    //midi_message = midiPacketTemplate;
    return true;
    }

    




};

#endif