/*     Star Tracker
 *      
 *  by Roy Darnell, www.strangerequests.net
 *  
 */

 /* 200 steps per round * 51 reduction gear = 10200 steps per round
 * 360 degrees per day -> 24 hours * 60 minutes * 60 seconds = 86400
 * 86400 seconds per rotation / 10200 steps per rotation = 8.47 seconds per step
 */
const float turning_constant = 8.470588;
#include <TM1637Display.h>
#define CLK 9
#define DIO 8
TM1637Display display = TM1637Display(CLK, DIO);

const uint8_t data[] = {0xff, 0xff, 0xff, 0xff}; // Create array that turns all segments on
const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};
const uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const int debounce_limit = 200; //debounce limit in ms
const int interruptUpPin = 2, interruptDownPin = 3;
volatile bool up_button_pressed = false; 
unsigned long lastPress; //for buttons
const int stepPin = 5; 
const int dirPin = 4; 
int rotation_dir = 1;
float CurrentRPM = turning_constant;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(interruptUpPin, INPUT_PULLUP);
  pinMode(interruptDownPin, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt( interruptUpPin ), increaseRPM, RISING );
  attachInterrupt( digitalPinToInterrupt( interruptDownPin ), decreaseRPM, RISING );
  display.clear();
  display.setBrightness(7);
  delay(500);
}
void loop() {
  digitalWrite(dirPin,HIGH ); // Enables the motor to move in a particular direction
  for(int x = 0; x < 1; x++) { //1 step a time
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  display.showNumberDecEx(CurrentRPM*100,0b01000000,true);
  delay(CurrentRPM*1000); //*1000 = 1 milisec
}

void change_dir(){
  if (rotation_dir == 1){
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  }
  else {
    digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  }
  rotation_dir = !rotation_dir;
}

void increaseRPM(){
  if ((millis()-lastPress>=debounce_limit))
  {
    lastPress = millis();
    CurrentRPM += 0.1;
  }
}

void decreaseRPM(){
  if ((millis()-lastPress>=debounce_limit) && (CurrentRPM > 0.1))
  {
    lastPress = millis();
    CurrentRPM -= 0.1;
  }
}
