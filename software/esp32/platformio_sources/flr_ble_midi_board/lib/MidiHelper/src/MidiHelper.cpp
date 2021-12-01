#include <MidiHelper.h>


// __UINT8_TYPE__* MidiHelper::buildMidiCommand(MidiHelper::MidiMessageType type, __UINT8_TYPE__ midi_channel, __UINT8_TYPE__ midi_cmd_nr, __UINT8_TYPE__ midi_cmd_value) {


    // //  TODO: inserire il controllo sul range dei comandi 

  
 
    // __UINT8_TYPE__ midiPacketTemplate[] = {
    // 0x80,  // header
    // 0x80,  // timestamp, not implemented 
    // 0x00,  // type + channel
    // 0x3c,  // data nr - key note: 0x3c == 60 == middle c
    // 0x00   // data value
    // };

    // /** Send the CC MIDI message ON **/
    // midiPacketTemplate[2] = 0xB0 + midi_channel -1; // CC on channel midi_channel
    // midiPacketTemplate[3] = midi_cmd_nr; // Control number
    // midiPacketTemplate[4] = midi_cmd_value;    // Control value

    // return midiPacketTemplate;
 
// }