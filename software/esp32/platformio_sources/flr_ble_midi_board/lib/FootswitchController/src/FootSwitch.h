#ifndef FOOTSWITCH_H
#define FOOTSWITCH_H

#include <MidiHelper.h>

#define DEBOUNCE_TIME 2  //ms

#define HOLD_TIME  500  //ms
class FootSwitch {

    typedef struct TapConfig {
        uint8_t midiChannel;
        MidiHelper::MidiMessageType midiMessageType;
        uint8_t midiMessageNumber;
        uint8_t midiMessageValue;

    } TapConfig;

public:
    typedef  enum FootSwitchEvent {
        FS_TAP,
        FS_HOLD
    } FootSwitchEvent;


    FootSwitch() {
        bool pressed = false;
        bool hold = false;
        bool group_active = false;
        unsigned long t_pressed = 0;
        unsigned long t_released = 0;
        unsigned long t_last_repeat = 0;
    };

    bool isPressed() {return pressed; };

    bool press(unsigned long t_press){
        bool ret = false;
        if ((t_press - t_released) > DEBOUNCE_TIME) {
            t_pressed = t_press;
            pressed = true;
            ret = true;
        }
        return ret;
    };

    bool release(unsigned long t_release, bool *tap_event) {
        bool ret = false;
        if ((t_release - t_pressed) > DEBOUNCE_TIME) {
            t_released = t_release;
            pressed = false;
            fshold = false;
            ret = true;
            if ( (t_released - t_pressed) < HOLD_TIME) {
                *tap_event = true;
            }
            else {
                *tap_event = false;
            }
        }
        else {
            *tap_event = false;
        }
        return ret;
    };

    bool checkHold(unsigned long c_time) {
        bool ret = false;

        if (pressed && (!fshold) && ((c_time - t_pressed) >= HOLD_TIME )) {
         fshold = true;
         ret = true;
        }

        return ret;
    }

private:
    bool pressed;
    bool fshold;
    bool group_active;
    unsigned long t_pressed;
    unsigned long t_released ;
    unsigned long t_last_repeat;

    TapConfig tapConfiguration;




};

#endif