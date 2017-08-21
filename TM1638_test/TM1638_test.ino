/*
 * Programma testato e funzionante per Arduino PRO -17/08/2017

#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0
 */
//TM1638 module pin define

/*
valori precedenti
int DIO = 2;    //Pin 2
int CLK = 1;    //Pin 1
int STB = 0;    //Pin 0
*/

int DIO = 8;    //Pin 8 
int CLK = 9;    //Pin 9
int STB = 7;    //Pin 7

/*
 * unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
 */
unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
unsigned char num[8];		                     
 void setup ()
{
  pinMode(STB,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(DIO,OUTPUT); 
// inserisco anche l'inizializzazione della seriale
   Serial.begin(9600);
}
 void loop()
    {
	unsigned char i;
  unsigned char ser;
	init_TM1638();	                           //Init TM1638
	for(i=0;i<8;i++)
	Write_DATA(i<<1,tab[0]);	          //Initialization register	
	while(1)
	{


  
		i=Read_key();      
    if (i==0)
    {
     for(i=0;i<8;i++)
      Write_DATA(i<<1,tab[0]);            //azzero i display
    }                    

       if (i==1)
    {
  Write_COM(0x8f);          //aumento la luminosità
    }   
           if (i==2)
    {
  Write_COM(0x89);          // abbasso la luminosità
    }   
    
		if(i<8)
		{
		  num[i]++;
		  while(i==Read_key());		      
		  if(num[i]>15)
		      num[i]=0;
		  Write_DATA(i*2,tab[num[i]]);// i*2 è l'indirizzo, mentre tab[num[i]] è il valore del dato
		  Write_allLED(1<<i);
		}
// da qui leggo la seriale 
//Serial.println("da qui dovrei leggere la seriale");
   
if (Serial.available() )
  { 
    Serial.println("OK, ai mandato qualcosa ");
    ser=Serial.read();
    Serial.println("hai inviato");
    Serial.println(ser);
    if(ser=='1')
    {
     // Led_1();
       Write_COM(0x8f);          //aumento la luminosità
    }
  
    else if (ser== '2')
    {
     // Led_2();
        Write_COM(0x89);          // abbasso la luminosità
    } 
  }
   
	}
}
/*
 * questa parte la uso per leggere la seriale
 
 */
//*****************************************************

  //*******************************************************


/*
 * qui sotto il codice per il TM1638
*/



void TM1638_Write(unsigned char	DATA)			
{
	unsigned char i;
        pinMode(DIO,OUTPUT);
	for(i=0;i<8;i++)
	{
            digitalWrite(CLK,LOW);
	    if(DATA&0X01)
               digitalWrite(DIO,HIGH);
	    else
               digitalWrite(DIO,LOW);
	    DATA>>=1;
            digitalWrite(CLK,HIGH);
	}
}
unsigned char TM1638_Read(void)					
{
	unsigned char i;
	unsigned char temp=0;;	
        pinMode(DIO,INPUT);
	for(i=0;i<8;i++)
	{
  	    temp>>=1;
            digitalWrite(CLK,LOW);
  	    if(digitalRead(DIO)==HIGH)
  	      temp|=0x80;
            digitalWrite(CLK,HIGH);
               
	}
	return temp;
}
void Write_COM(unsigned char cmd)		
{
        digitalWrite(STB,LOW);
	TM1638_Write(cmd);
        digitalWrite(STB,HIGH);
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
        digitalWrite(STB,LOW);
	TM1638_Write(0x42);		           
	for(i=0;i<4;i++)		
  	{
            c[i]=TM1638_Read();
        }
        digitalWrite(STB,HIGH);					           //
	for(i=0;i<4;i++)
	{
            key_value|=c[i]<<i;
        }	
        for(i=0;i<8;i++)
        {
            if((0x01<<i)==key_value)
            break;
        }
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//
{
	Write_COM(0x44);
        digitalWrite(STB,LOW);
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
        digitalWrite(STB,HIGH);
}
void Write_allLED(unsigned char LED_flag)					//
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
	    if(LED_flag&(1<<i))
	      Write_DATA(2*i+1,1);
	    else
	      Write_DATA(2*i+1,0);
        }
}

//TM1638
void init_TM1638(void)
{
	unsigned char i;
	Write_COM(0x8b);       //
	Write_COM(0x40);       //
        digitalWrite(STB,LOW);		           //
	TM1638_Write(0xc0);    //
	for(i=0;i<16;i++)	   //
	  TM1638_Write(0x00);
        digitalWrite(STB,HIGH);
}

