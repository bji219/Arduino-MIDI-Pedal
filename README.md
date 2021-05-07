# Arduino-MIDI-Pedal
## Use an Arduino to send MIDI signals to a keyboard, synthesizer, etc.

- Inspired by the Instructables project by Amanda Ghassaei found here:
https://www.instructables.com/Send-and-Receive-MIDI-with-Arduino/

1. This project began as a way to address a gap in functionality in my Korg SV-2 keyboard. There is a footswitch pedal which can change the Leslie speaker effect when you are playing organ. However, there is no way to assign other FX to the pedal. For istance, I play EP more than organ so I like using the Tremolo effect. A-la Chick Corea it would be cool to turn the Tremolo on and off at different points as I am playing. I am sure other keyboards out there could have assignable effects pedals, but being an engineer I decided to solve this the hard way. 
2. First, I began by looking into the SV-2 user manual to see which MIDI parameters could be edited.
3. 

## The Finished Product
# ![alt text](https://github.com/bji219/Arduino-MIDI-Pedal/blob/main/IMG_9829.JPG)

### Let me know if you have any improvement ideas!
- This Arduino code is pretty hacky and basic, but it gets the job done. If there are other functions that you think would be cool to add or would work with a single On/Off footswitch, give it a try and let me know how it turns out!

### What comes next?
- When changing FX, it takes 2 presses to turn on the new effect instead of 1
- What to do with hold function?
- Adjust double press time to be a bit more forgiving
- Abmient on/off FX is not working, could be the MIDI code/something internal to the SV-2
- Pitch bend? 
- DCgaptime affects the latency of the FX turning on or off. Obviously low latency is better for musicality
