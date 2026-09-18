// Importations

#include <Arduino.h>
#include <AccelStepper.h> 

// Pins

int xDir = 5; 
int xStep = 2; 
int enable = 8; 

// Variables

int CodeIn;
AccelStepper xStepper(AccelStepper::DRIVER, xStep, xDir); 
int microstepping = 16;

// Conversion
long longueur_to_steps(float longueur) 
{
  
  long steps = (long)(longueur * 200 * microstepping/8) ; // 1 rotation => 8 mm
  return steps;
}



void setup() {   
  Serial.begin(115200);
  pinMode(xDir, OUTPUT);
  pinMode(xStep, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, LOW);
  xStepper.setPinsInverted(false, false, true);

  xStepper.setMaxSpeed(80); // Speed : Steps per seconde
  xStepper.setAcceleration(300); 
}


// Syringe pump A : go
void to_go()
{   float position = 100;
    long steps = longueur_to_steps(position); // Conversion
    xStepper.move(steps);

}

// Syringe pump A : back off
void to_back_off(){
  float location = -50;
  long steps_location = longueur_to_steps(location);
  xStepper.move(steps_location); // position relative
}



void loop() {
  if (Serial.available() > 0) { 
    char CodeIn = Serial.read();
    if (CodeIn == 'A') { 
      digitalWrite(enable, HIGH); 
      to_go();}

    if (CodeIn == 'S'){
        xStepper.stop();
      }

    if (CodeIn == 'R'){
        digitalWrite(enable, HIGH);
        to_back_off();
      }
 else { 
      
      digitalWrite(enable,LOW);}} 

  xStepper.run();
}


