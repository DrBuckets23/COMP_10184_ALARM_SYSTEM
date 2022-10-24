/**
 * I, Andrew Maye, 000344062 certify that this material is my original work.
 * No other person's work has been used without due acknowledgement.
 * 
 * Completed Stage 5 of project
*/

#include <Arduino.h>

// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6 

// Define all alarm states
#define ALARM_DISABLED 0
#define ALARM_ENABLED 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3

int iAlarmState;

// Button definition
int BUTTON;

// Delay
#define BUTTON_DELAY 200
#define BLINK 125

/**
 * This method collects all pin information
*/
void collectInputs(){
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 

  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * This method Enables the alarm and turns the led off
*/
void enableAlarm(){
  iAlarmState = ALARM_ENABLED;
  digitalWrite(LED_BUILTIN, HIGH);
}

/**
* This method returns iAlarmState
*/
int checkAlarmState() {
  return iAlarmState;
}

/**
 * This method blinks the led 4 times per second(2 times per .5 second)
*/
void blinkLED() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(BLINK);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(BLINK);
}

/**
 * This method actives the alarm
*/
void activeAlarm() {
  iAlarmState = ALARM_ACTIVE;
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * This method disables the alarm
*/
void disableAlarm() {
  iAlarmState = ALARM_DISABLED;
  digitalWrite(LED_BUILTIN, HIGH);
}

/** 
 * this methods used a countdown and awaits for a button press, if no button press
 * the alarm gets activated, if button is pressed alarm gets disabled
*/
void alarmCountDown() {
  long duration = 10000; //ALarm countdown duration
  long startMillis;
  long currentMillis;
  boolean disable = false;

  iAlarmState = ALARM_COUNTDOWN;

  //time to measure countdown
  startMillis = millis();
  currentMillis = millis();
  
  while(currentMillis - startMillis < duration){
    currentMillis = millis();
    blinkLED();
    BUTTON = digitalRead(PIN_BUTTON);
    if(BUTTON == LOW){
      delay(BUTTON_DELAY);
      disable = true;
      break;
    }
  }
  if(disable){
    disableAlarm();
  } else {
    activeAlarm();
  } 
}


void setup() {
  // configure the USB serial monitor 
  Serial.begin(115200); 

  collectInputs();
  Serial.println("\nAndrew Maye, 000344062 Alarm system. Completion level: Stage 5");
  Serial.println("Alarm starts off disabled, press the button to enable alarm but be careful, you will probaly set of the motion sensor.");
  disableAlarm();
}


void loop() {
  bool bPIR;
  bool detectMotion;
  
  checkAlarmState();
  
  switch(iAlarmState) {
    case ALARM_DISABLED: 
      BUTTON = digitalRead(PIN_BUTTON);
      if(BUTTON == LOW){
        delay(BUTTON_DELAY);
        enableAlarm();
      }
      detectMotion = false;
      break;
    case ALARM_ENABLED:
      detectMotion = true;
      break;
    case ALARM_COUNTDOWN:
      detectMotion = false;
      break;
    case ALARM_ACTIVE:
      detectMotion = false;
      break;
  }
  
  if(detectMotion){
    // read PIR sensor (true = Motion detected!).  As long as there 
    // is motion, this signal will be true.  About 2.5 seconds after  
    // motion stops, the PIR signal will become false. 
    bPIR = digitalRead(PIN_PIR);

    if(bPIR){
      alarmCountDown();
    }
  }
}