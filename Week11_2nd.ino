#include "Clicky.h"

int NO_STATE = 0;
int FIRST_STATE = 1;
int SECOND_STATE = 2;
int SUCCESS_STATE = 3;

Clicky mButton0;
Clicky mButton1;

int cState = NO_STATE;
int potPIN = A0;
int lastPotValue = 0;
int PotBool = 0;

void setup() {
  // inputs
  mButton0.setup(2);
  mButton1.setup(3);

  // outputs
  pinMode(4, OUTPUT); // Green LED
  pinMode(5, OUTPUT); // Red LED
  Serial.begin(9600);
}

void loop() {
  bool b0c = mButton0.wasClicked();
  bool b1c = mButton1.wasClicked();

  // Read Potential
  int potValue = analogRead(potPIN);

  // Potentiometer debounce
  if (abs(potValue - lastPotValue) >= 10) {
    PotBool = 1;
    lastPotValue = potValue; // Update lastPotValue
  }

  // states
  if (cState == NO_STATE) {
    if (b0c) {
      cState = FIRST_STATE; 
    } else if (b1c) {
      cState = NO_STATE; 
    } else if (PotBool == 1) { 
      cState = NO_STATE;
    }
  } else if (cState == FIRST_STATE) {
    if (b1c) {
      cState = SECOND_STATE;
    } else if (b0c) {
      cState = NO_STATE;
    // } else if (PotBool == 1) { 
    //   cState = NO_STATE;
    }
  

    // else {
    //     delay(5000);
    //     cState = NO_STATE;  // Transition back to NO_STATE after the delay
    // }
  } else if (cState == SECOND_STATE) {
    if (b0c) {
      cState = NO_STATE;
    } else if (b1c) {
      cState = NO_STATE;
    } else if (potValue >= 900) { 
      cState = SUCCESS_STATE;
    }
  } 

  // output logic
  int blinkVal = (millis() / 200) % 2;
  if (cState == NO_STATE) {
    if (b0c) {
      Serial.println("Button 1 pressed");
    }
    if (b1c) {
      Serial.println("Button 2 pressed");
    }
    digitalWrite(5, blinkVal);
    digitalWrite(4, LOW);
  } else if (cState == FIRST_STATE) {
    Serial.println("First state");
    delay(2);
    digitalWrite(5, LOW);
    digitalWrite(4, blinkVal);
  } else if (cState == SECOND_STATE) {
    Serial.println("Second state");
    digitalWrite(5, blinkVal);
    digitalWrite(4, blinkVal);
  } else if (cState == SUCCESS_STATE) {
    Serial.println("Success state");
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
    delay(10000);
    digitalWrite(4, LOW);
    
    PotBool = 0;
    cState = NO_STATE; 
  }
}
