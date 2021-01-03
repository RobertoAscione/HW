#include "PS2X_lib.h"  //for v1.6
#include <Wire.h>
#include "Adafruit_MotorShield.h"
#include "Adafruit_MS_PWMServoDriver.h"

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        12  
#define PS2_CMD        11  
#define PS2_SEL        10 
#define PS2_CLK        13 

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   true 
//#define rumble      true
#define rumble      true

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

int lx = 0;
int ly=  0;
int L_Speed = 0;
int R_Speed = 0;
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *L_M3 = AFMS.getMotor(3);
Adafruit_DCMotor *R_M4 = AFMS.getMotor(4);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);


const int SERVOS = 4;
const int ACC = 10; // the accurancy of the potentiometer value before idle starts counting
int PIN[SERVOS], value[SERVOS], idle[SERVOS], currentAngle[SERVOS], MIN[SERVOS], MAX[SERVOS], INITANGLE[SERVOS], previousAngle[SERVOS],ANA[SERVOS];

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 50;   // 50 Hz
 //--AR-- Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  //--AR-- Serial.print(pulselength); Serial.println(" us per bit");  
  pulse *= 1000;
  pulse /= pulselength;
 //--AR--  Serial.println(pulse);
  AFMS.setPWM(n, pulse);
}

// Angle to PWM -- la chiamo per far ruotae il servo si occupa anche di convertire da gradi in PWM
void writeServo(uint8_t n,uint8_t angle){
  double pulse;
  pulse=0.5+angle/90.0;
  setServoPulse(n,pulse);
}

void setup(){
  Serial.begin(57600);
  delay(2000);  //added delay to give wireless ps2 module some time to startup, before configuring it
  Serial.print("Cerco il controller..");
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  do{
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    if(error == 0){
      Serial.println("\nConfigured successful ");
      break;
    }else{
      Serial.print(".");
      delay(100);
    }
  }while(1);  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
    case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
   }
   ps2x.read_gamepad(true, 200);
   delay(500);
   ps2x.read_gamepad(false, 200);
   delay(300);
   ps2x.read_gamepad(true, 200);
   delay(500);


   
    //Middle Servo  --- questa è la parte che devo controllare
 // pinza
  PIN[0] = 0;  // AR questo sono i valori che devo individuare
  MIN[0] = 0; // AR valore precedente 10
  MAX[0] = 60; // AR valore precedente 170
  INITANGLE[0] = 90;
  ANA[0] = 3;
  //Left Side
  PIN[1] = 1;
  MIN[1] = 10; // AR valore precedente 10// This should bring the lever to just below 90deg to ground
  MAX[1] = 170; // AR valore precedente 140
  INITANGLE[1] = 90; // This should bring the lever parallel with the ground
  ANA[1] = 2;
  //Right Side
  PIN[2] = 2;   // precedente valore 14
  MIN[2] = 30; // precedente valore 140
  MAX[2] = 120; // precedente valore 170
  INITANGLE[2] = 90;
  ANA[2] = 0;
  //Claw Servo
  PIN[3] = 3;  // precedente valore 15
  MIN[3] = 30;
  MAX[3] = 100; // precedente valore 120
  INITANGLE[3] = 90; // precedente valore 60
  ANA[3] = 1;


  AFMS.begin(50);  // create with the default frequency 1.6KHz
  for (int i = 0; i < SERVOS; i++){
    value[i] = 0;
    idle[i] = 0;
    previousAngle[i]=INITANGLE[i];
    currentAngle[i]=INITANGLE[i];
    writeServo(PIN[i],INITANGLE[i]);
  }
  

   
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START)){         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
      ps2x.read_gamepad(true, 200);
    }
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");      

  
    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      L_M3->run(FORWARD);
      R_M4->run(FORWARD);
      L_Speed = ps2x.Analog(PSAB_PAD_UP);
      R_Speed = ps2x.Analog(PSAB_PAD_UP);  
    }else if(ps2x.ButtonReleased(PSB_PAD_UP))  {
      L_M3->run(RELEASE);
      R_M4->run(RELEASE);      
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      L_Speed = ps2x.Analog(PSAB_PAD_DOWN);
      R_Speed = ps2x.Analog(PSAB_PAD_DOWN);       
      L_M3->run(BACKWARD);
      R_M4->run(BACKWARD);
    }else if(ps2x.ButtonReleased(PSB_PAD_DOWN))  {
      L_M3->run(RELEASE);
      R_M4->run(RELEASE);      
    }
    if(ps2x.Button(PSB_R1)){
      R_Speed = 0;
    }
    if(ps2x.Button(PSB_L1)){
      L_Speed = 0;
    }
    L_M3->setSpeed(L_Speed);
    R_M4->setSpeed(R_Speed);
    
    if(ps2x.Button(PSB_PAD_LEFT)){
      L_M3->run(BACKWARD);
      R_M4->run(FORWARD);
      L_M3->setSpeed(250);
      R_M4->setSpeed(250);
    }else if(ps2x.ButtonReleased(PSB_PAD_LEFT))  {
      L_M3->run(RELEASE);
      R_M4->run(RELEASE);      
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.println(ps2x.Analog(PSB_PAD_RIGHT));
      L_M3->run(FORWARD);
      R_M4->run(BACKWARD);
      L_M3->setSpeed(250);
      R_M4->setSpeed(250);
    }else if(ps2x.ButtonReleased(PSB_PAD_RIGHT))  {
      Serial.println("*****PSB_PAD_RIGHT***** ");
      L_M3->run(RELEASE);
      R_M4->run(RELEASE);      
    }

    
    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if(ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");        
    }

    if(ps2x.ButtonPressed(PSB_CIRCLE)){               //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
      openGripper() ; 
    }
    if(ps2x.NewButtonState(PSB_CROSS)) {              //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
      ps2x.read_gamepad(true, vibrate);   
    }
    if(ps2x.ButtonPressed(PSB_SQUARE)) {             //will be TRUE if button was JUST released
      Serial.println("Square just released");
      closeGripper() ;
    }
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values: (0)LY =");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(", (1)LX=");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(", (2)RY=");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(", (3)RX=");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
      
    }     
  
  /*
   * togliere inizio
  
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
  /*
   * togliere fine
   */
   
  value[0] = ps2x.Analog(PSS_LY); // in questo vettore inserisco i valore letti dal joestic LX 1, RY 0, RX 3, LY 2 /prima i primi due erano invertiti
  value[1] = ps2x.Analog(PSS_LX);
  value[2] = ps2x.Analog(PSS_RY);
  value[3] = ps2x.Analog(PSS_RX); // o muovo o chiudo 

// modifico il codice perche LY (della conselle ha valori tra 0 e 70 non so perche forse è difettosa la consolle lo uso per la pinza che sposto sul PIN [0] per comodità

   if (value[0] > 40) {// rispetto al ciclo ho cambiato i valori
      if (currentAngle[0] < MAX[0]) currentAngle[0]+=1;
      writeServo(PIN[0],currentAngle[0]);     
    } else if (value[0] < 30) { // rispetto al ciclo ho cambiato i valori
      if (currentAngle[0] > MIN[0]) currentAngle[0]-=1;
      writeServo(PIN[0],currentAngle[0]); }
  
   for (int i = 1; i < SERVOS; i++){  // lo facci opartire da 1 anziche 0
    if (value[i] > 130) {
      if (currentAngle[i] < MAX[i]) currentAngle[i]+=1;
      writeServo(PIN[i],currentAngle[i]);     
    } else if (value[i] < 120) {
      if (currentAngle[i] > MIN[i]) currentAngle[i]-=1;
      writeServo(PIN[i],currentAngle[i]);    
    }  
   
        if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //debug
        Serial.print("Angolo Corrente=");
        Serial.print(currentAngle[i], DEC); 
        Serial.print(", Value=");
        Serial.print(value[i], DEC); 
        Serial.print(", i=");
        Serial.println(i, DEC); 
          }
   }  
  
  delay(10);
}


//Grab something
void openGripper() {
    writeServo(PIN[0],MIN[0]);
    //currentAngle[0]= MIN[0];// aggiunto AR
  delay(300);
}

//Let go of something
void closeGripper() {
  writeServo(PIN[0],MAX[0]);
  //currentAngle[0]= MAX[0]; // aggiunto AR
  delay(300);
}
