int a = 5;
int b = 2;
int c = 3;
int d = 4;

unsigned long time1;
unsigned long time2;
unsigned long deltatime;

unsigned long misura_tempo( unsigned long ti) {
   time2 = micros();
  deltatime = time2-ti;
  Serial.print("Tempo di esecuzione del blocco in micro secondi: ");
  Serial.println(deltatime); //prints time since program started
}

/*
inserire i seguenti comandi per eseguire la misurazione del tempo di esecuzione
 time1 = micros();
 ..qui c'è il blocco di operazioni di cui voglio misurare il tempo di esecuzione
 
 misura_tempo(time1);
*/

void Procedura_Interrupt() {
  //detachInterrupt(digitalPinToInterrupt(2));
    digitalWrite(13, HIGH);
      for (int i=0; i <= 1000; i++) {
  delayMicroseconds(10000);
  }
   // Serial.println("Eseguo  la procedura di Interrupt");

  
   //attachInterrupt(digitalPinToInterrupt(2), Procedura_Interrupt, LOW ); 
};

/*
inserire i seguenti comandi per attivare la procedura
 pinMode(2, INPUT_PULLUP); //il piedino 2 è utilizzato per l'interrupt
 attachInterrupt(digitalPinToInterrupt(2), Procedura_Interrupt, LOW ); 
*/


void Operazione_1(){
   time1 = micros();
  Serial.println("eseguo l'operazione 1");
// altre Operazioni...
   misura_tempo(time1);
  };

void Operazione_2(){
  Serial.println("eseguo l'operazione 2");
    delay(1000); 
  };

void Operazione_3(){
  Serial.println("eseguo l'operazione 3");
    delay(1000); 
  };
void Operazione_4(){
  Serial.println("eseguo l'operazione 4");
  };
void Operazione_5(){
  Serial.println("eseguo l'operazione 5");
    delay(1000); 
  };
void Operazione_6(){
  Serial.println("eseguo l'operazione 6");
  };
void Operazione_7(){
  Serial.println("eseguo l'operazione 7");
  };


void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP); //il piedino 2 è utilizzato per l'interrupt
  attachInterrupt(digitalPinToInterrupt(2), Procedura_Interrupt, FALLING ); 
  
}



void loop()
{
 digitalWrite(13, LOW);
 Operazione_1();
 Operazione_2();
 // verifica 1
  if (a == 1 ) {
       Operazione_3();
       Operazione_4();
        // verifica 2
        if (b == 2 ) {
          Operazione_6();
        }
      else {
            Operazione_7();
        }
      }
  else {
     Operazione_5();
    }
  
 Serial.println("-------------------------------------");
  delay(1000); 
 
}
