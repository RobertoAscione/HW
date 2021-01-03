/*
Programma didattico simulazione sistema di frenata automatica
https://www.tinkercad.com/things/5kywmiARJTq
Roberto Ascione - dicembre 2020
*/

#include <Servo.h>
Servo myservo;  // create servo object to control a servo

int distance = 0;
unsigned long time1;
unsigned long time2;
unsigned long deltatime;

unsigned long misura_tempo( unsigned long ti) {
   time2 = micros();
  deltatime = time2-ti;
  Serial.print("Tempo di esecuzione del blocco in micro secondi: ");
  Serial.println(deltatime); //prints time since program started
}


long readUltrasonicDistance(int triggerPin, int echoPin)
{

  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
return pulseIn(echoPin, HIGH);
}

void blink(int pin)
{
 digitalWrite(pin, HIGH); // sets the digital pin 13 on
  delay(100);            // waits for 100 ms
  digitalWrite(pin, LOW);  // sets the digital pin 13 off
  delay(100);            // waits for 100 ms


}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  myservo.attach(9);  
  myservo.write(0); 
}

void loop()
{
  // measure the ping time in cm
  time1 = micros(); // debug voglio misurare il tempo di esecuzione di questa procedura
  distance = 0.01723 * readUltrasonicDistance(7, 7);
  misura_tempo(time1); // chiamo la procedura per misurare e stampare il tempo di esecuzione
  
 
  if (distance < 300) //blink the led
  {
   blink(13);
  Serial.println("ATTENZIONE FRENA");
  tone(10, 400);
  }else { noTone(10);}
  
  
  if (distance < 30) //inchiodo
  {       
     digitalWrite(11, HIGH);
     digitalWrite(12, LOW);
    myservo.write(90); 
       Serial.println("INCHIODO");
     
 
     delay(1);
    } 
    else if (distance < 100) {
  // INIZIO A FRENARE
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    myservo.write(30);   
    Serial.println("INIZIO A FRENARE");
       delay(1);
  }
    
  else {
  // rilascio il freno
    myservo.write(0); 
       digitalWrite(11, LOW);
       digitalWrite(12, LOW);
       
    delay(1);
  }
  
  
   Serial.print(distance);
  Serial.println("cm");
  delay(1000); // Wait for 1000 millisecond(s)
}
