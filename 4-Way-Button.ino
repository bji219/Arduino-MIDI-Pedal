// Brendan Inglis
// MIDI Pedal project for SV-2

/* 4-Way Button:  Click, Double-Click, Press+Hold, and Press+Long-Hold Test Sketch

Adapted from the code written by Jeff Saltzman Oct. 13, 2009

To keep a physical interface as simple as possible, this sketch demonstrates generating four output events from a single push-button.
1) Click:  rapid press and release
2) Double-Click:  two clicks in quick succession
3) Press and Hold:  holding the button down
4) Long Press and Hold:  holding the button for a long time
*/

#define buttonPin 4        // analog input pin to use as a digital input
#define ledPin1 13          // digital output pin for LED 1
//#define ledPin2 4          // digital output pin for LED 2
//#define ledPin3 0          // digital output pin for LED 3
//#define ledPin4 13          // digital output pin for LED 4

// LED variables
boolean ledVal1 = false;    // state of LED 1
int programchange = 0; // counter for switching MIDI signal sent

//=================================================

void setup() {
   // Set button input pin
   pinMode(buttonPin, INPUT);
   digitalWrite(buttonPin, HIGH );
   // Set LED output pins
   pinMode(ledPin1, OUTPUT);
   digitalWrite(ledPin1, ledVal1);
   // Set MIDI baud rate:
   Serial.begin(31250);//MIDI Baud rate 31250, use 9600 for testing
}
 
void loop() {
   // Get button event and act accordingly
   int b = checkButton();
   if (b == 1) clickEvent();
   if (b == 2) doubleClickEvent();
   if (b == 3) holdEvent();
}

//=================================================
// Events to trigger

void clickEvent() { //Needs to turn MIDI on AND off
   
   if(ledVal1 == LOW){on();}else{off();}
   
   ledVal1 = !ledVal1;
   digitalWrite(ledPin1, ledVal1);
}
void doubleClickEvent() {
   ledVal1 = !ledVal1;
   digitalWrite(ledPin1, HIGH);
   delay(200);
   digitalWrite(ledPin1,LOW);
   delay(200);
   digitalWrite(ledPin1,HIGH);
   delay(200);
   digitalWrite(ledPin1,LOW);
   //programchange = programchange+1; //Cause too much lag
   
}
void holdEvent() {
   ledVal1 = !ledVal1;
   digitalWrite(ledPin1, HIGH);
   delay(500);
   digitalWrite(ledPin1,LOW);
   delay(500);
   digitalWrite(ledPin1,HIGH);
   delay(500);
   digitalWrite(ledPin1,LOW);
  programchange = programchange+1;
  
}

// plays a MIDI note
void noteOn(int cmd, int pitch, int velocity) {
  //Serial.write(func);
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
 
}

void off() {

  if (programchange == 0) {
    noteOn(0xBE, 0x67, 0x00); //Toggle Pre-FX On/Off; Channel 15 (0x9E), some note value (103, which is 0x67), low velocity (0x00):
  } else if (programchange == 1) {
    noteOn(0xBE, 0x68, 0x00); //Toggle Amp On/Off
  } else if (programchange == 2) {
    noteOn(0xBE, 0x69, 0x00); //Toggle Modulation On/Off
  } else if (programchange == 3) {
    noteOn(0xBE, 0x70, 0x00); //Toggle Ambient On/Off
  } else if (programchange == 4) {
    programchange = 0; //cycle back to the beginning
    noteOn(0xBE, 0x67, 0x00); //Toggle Pre-FX On/Off
  }
}

void on() {
  
  if (programchange == 0) {
    noteOn(0xBE, 0x67, 0x7F); //Toggle Pre-FX On/Off; Channel 15 (0x9E), some note value (103, which is 0x67), low velocity (0x00):
  } else if (programchange == 1) {
    noteOn(0xBE, 0x68, 0x7F); //Toggle Amp On/Off
  } else if (programchange == 2) {
    noteOn(0xBE, 0x69, 0x7F); //Toggle Modulation On/Off
  } else if (programchange == 3) {
    noteOn(0xBE, 0x70, 0x7F); //Toggle Ambient On/Off
  } else if (programchange == 4) {
    programchange = 0; //cycle back to the beginning
    noteOn(0xBE, 0x67, 0x7F); //Toggle Pre-FX On/Off
  }
}

//=================================================
//  MULTI-CLICK:  One Button, Multiple Events

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 50;            // max ms between clicks for a double click event
int holdTime = 1000;        // ms hold period: how long to wait for press+hold event
//int longHoldTime = 3000;    // ms long hold period: how long to wait for press+hold event

// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
boolean DCwaiting = false;  // whether we're waiting for a double click (down)
boolean DConUp = false;     // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;    // whether it's OK to do a single click
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
boolean ignoreUp = false;   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;        // when held, whether to wait for the up event
boolean holdEventPast = false;    // whether or not the hold event happened already
//boolean longHoldEventPast = false;// whether or not the long hold event happened already

int checkButton() {   
   int event = 0;
   buttonVal = digitalRead(buttonPin);
   // Button pressed down
   if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
   {
       downTime = millis();
       ignoreUp = false;
       waitForUp = false;
       singleOK = true;
       holdEventPast = false;
       if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true)  DConUp = true;
       else  DConUp = false;
       DCwaiting = false;
   }
   // Button released
   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
   {       
       if (not ignoreUp)
       {
           upTime = millis();
           if (DConUp == false) DCwaiting = true;
           else
           {
               event = 2;
               DConUp = false;
               DCwaiting = false;
               singleOK = false;
           }
       }
   }
   // Test for normal click event: DCgap expired
   if ( buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
   {
       event = 1;
       DCwaiting = false;
   }
   // Test for hold
   if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
       // Trigger "normal" hold
       if (not holdEventPast)
       {
           event = 3;
           waitForUp = true;
           ignoreUp = true;
           DConUp = false;
           DCwaiting = false;
           //downTime = millis();
           holdEventPast = true;
       }
   }
   buttonLast = buttonVal;
   return event;
}

//Successful
//However, when program changes, takes additional press to start new effect
//What to do with hold function?
//Adjust double press time to be a bit more forgiving
//If fx are off and double press, takes two presses to turn on new fx
//Abmient on/off not working
//Pitch bend? 
//Way slower than vanilla no-switching code. DCgaptime effects this!
//change chnnel 4 to pitch bend or something
