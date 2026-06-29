#include <Arduino.h>
#include <AccelStepper.h> //importation bibliothèque

int xDir = 5; // numéro broche step sur driver x, broche direction
int xStep = 2; // broche cadencement
int enable = 8;

AccelStepper xStepper(AccelStepper::DRIVER, xStep, xDir); // déclaration variable xstepper

int microstepping = 16; // pas intermédiaires

long longueur_to_steps(float longueur) // fonction qui devra convertir la longueur qu'on veut en nombre de pas (longueur entre deux dents 1mm implique un tour fait un avancement de1mm)
{
  // 1 motor revolution = 360/number of teeth
  long steps = (long)(longueur * 200 * microstepping/8) ; // 8 longueur pour un tour en
  return steps;
}

String inputString = "";
bool stringComplete = false;

void setup() {   // il faut écrire x espace la distance en mm, c est une position absolue attention la position x=0 correspond à la position initiale
  Serial.begin(115200);
  Serial.println("Hello");
  pinMode(xDir, OUTPUT);
  pinMode(xStep, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, LOW);
  xStepper.setPinsInverted(false, false, true);

  xStepper.setMaxSpeed(8000); // valeurs max à changer (accélération v max arret)
  xStepper.setAcceleration(8000); // accélération pour obtenir la vitesse maximale pareil pour décélérer 

  inputString.reserve(50);

}

void processCommand(String command) 
{
  Serial.print("command received : "); Serial.println(command);
  command.trim();
  if (command.startsWith("x"))
  {
    float position = command.substring(2).toFloat();
    long steps = longueur_to_steps(position);
    Serial.print(position);
    Serial.print("go to x = "); Serial.print(position); Serial.print("mm"); 
  
    xStepper.moveTo(steps);
  }
//position absolue
}

void serialEvent()
// arduino method, called during loop implicitely
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read(); // nom donne a la chaine de caractere
    if (inChar == '\n' || inChar == '\r')// si il y a un saut a la ligne(entree dans arduino), dans le langage ASCII compte bien comme un unique caractère
    {
      stringComplete = true; // alors ce sera la fin du message
    }
    else
    {
      inputString += inChar;
    }
  }
}

void loop() {
  if (xStepper.distanceToGo() == 0) {
    digitalWrite(enable, HIGH);
    }
    else{
      digitalWrite(enable, LOW);}
  
  xStepper.run();
  
  if (stringComplete)
  {
    processCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}




