# include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <FHT.h>


RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
double text=0;
int maxVolt=0;

char mod01[4]={0};
char mod02[4]={0};
char mod03[4]={0};


#define soundPin A0
#define manual_send 5

int data_count=0;//data from each module present

int sound_buf[256]={0};//stores the sound values
int sound_buffer_index=0;//current positions of the sound buffer index
double min_dist=80;
long min_dist_time=0;
void setup()  
{
  pinMode(9,OUTPUT);
  pinMode(2,OUTPUT);
 // while (!Serial);
  Serial.begin(57600);
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  
  radio.startListening();
  
}

void loop()
{
  if (radio.available())
  {
    digitalWrite(2,HIGH);   
    radio.read(&text, sizeof(text));
    if(text<min_dist)
    {
      min_dist=text;
      min_dist_time=millis();
    }
  }
  else
  {
    digitalWrite(2,LOW);
   /* if(sample_sound())
    {
      digitalWrite(9,LOW);
      if(millis()-grabTime)
       {
          Mod02=maxVolt;
       }
 
    } */
  
  }
  // if(millis()-min_dist_time>50)
 //     min_dist=80;    
  
  
  //text=text/100000000;
   Serial.println(text);
  // text=0;
}

int sample_sound()//fills the bufer array with voltage reading values
{
  if(sound_buffer_index<256)
  {
    fht_input[sound_buffer_index]=analogRead(soundPin);
    Serial.println(fht_input[sound_buffer_index]);
    sound_buffer_index++; 
  }
  else
  {
    sound_buffer_index=0;  
  }
  if(sound_buffer_index!=0)
  { 
   if ((fht_input[sound_buffer_index-1]> 600) || (fht_input[sound_buffer_index-1]<400) )
    {
      if((fht_input[sound_buffer_index-1]> maxVolt))
      {
        maxVolt=fht_input[sound_buffer_index-1];
       // Mod02=maxVolt;
      }
      
      return 1;  
    }
    else 
    {
      return 0;
    }
  }
}
