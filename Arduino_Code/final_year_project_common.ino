#include <FHT.h>

//#include <fhtConfig.h>



/*
This code runs on the common devices(ones with only nrf24l01+)
It sends data to main module via nrf with distance info
for now it will only detect impulse sounds(clap)


*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



#define soundPin A0
#define manual_send 5


int sound_buf[256]={0};//stores the sound values
int sound_buffer_index=0;//current positions of the sound buffer index

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  Serial.println("begin");
  radio.setPALevel(RF24_PA_MAX);
  pinMode(9, OUTPUT);
  
}

char t[4];
char h='z';
String g="upp";
unsigned long i=0;
unsigned long times=i;
int re=0;

void loop() {
  sample_sound();
  // put your main code here, to run repeatedly:
//  sample_sound();
  //Serial.println("Test");
 // if(impulse_sound_detected()==1)
 // {
    
    char* f="#";
//     t=g.toCharArray();
   /*  re=analogRead(A0);
    Serial.print(re);
    Serial.print(f);*/
    digitalWrite(9,LOW);
    i=1;
   /* if(re>530)
    {
      i=1;
      }
      else
    {
        i=0;
    }*/
    
    radio.write(&i, sizeof(unsigned long));
     digitalWrite(9,HIGH);
     
     
 /// }
 /* else
  {//unsigned long times=i;
    i=0;
   
     radio.write(&i, sizeof(unsigned long));
    }*/
}


void sample_sound()
{
  if(sound_buffer_index<256)
  {
    digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
    //sound_buf[sound_buffer_index]=analogRead(0);
    fht_input[sound_buffer_index]=analogRead(0);
    //Serial.println(sound_buffer_index);
    sound_buffer_index++;
    
  }
  else
  {
      digitalWrite(9, LOW);   
    sound_buffer_index=0;
     
    //Serial.println("Start FHT");
    fht_window();
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    for(int i=0;i<256;i++)
    {
      Serial.print(fht_input[i]);
      Serial.print("#");
     }
   /*   send_to_processing(); 
    
    //fht_mag_log(); // take the output of the fht*
  } 
} 

int impulse_sound_detected()
{
   if(fht_input[sound_buffer_index-1]>1000)//fht_input[sound_buffer_index-1]>200 || fht_input[sound_buffer_index-1]<140)  
   {
     return 1; 
   }
   else
   {
     return 0; 
   }
}


void send_to_processing()
{
  for(int i=0;i<256;i++) 
  {
    /*Serial.print("Index: ");
    Serial.print(i); 
    Serial.print("  Val:  "); */
    Serial.println(fht_input[i]);  
  }
   
}

void send_nrf(char p[])
{
 // const char text[] = "Hello World";
  radio.write(&p, sizeof(p));
  
}
