# include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <FHT.h>
#include <stdlib.h>     /* strtof */


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


long maxVolt_time=0;

float distance=0;
float distance02=0;
float distance03=0;

unsigned long mod01_detect_time=0;
unsigned long mod02_recieve_time=0;
unsigned long mod03_recieve_time=0;

char distance_recieved[32] = {0};
void setup()  
{
  pinMode(9,OUTPUT);
  pinMode(2,OUTPUT);
 // while (!Serial);
  Serial.begin(57600);
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  //radio.openReadingPipe(1, rxAddr[1]);
  
  
  radio.startListening();
  
}

void loop()
{
  if (radio.available())
  {
    digitalWrite(2,HIGH);   
    radio.read(&distance_recieved, sizeof(distance_recieved));
    if(distance_recieved[5]=='w')
    {
      mod02_recieve_time=millis();
      distance02=atof(distance_recieved);
      Serial.print("Module 2: ");
      Serial.print(distance02);    
    }
    else if(distance_recieved[5]=='h')
    {
      mod03_recieve_time=millis();
      distance03=atof(distance_recieved);   
      Serial.print("Module 3: "); 
      Serial.print(distance03);
    }
   
    /*if(text<min_dist)
    {
      min_dist=text;
      min_dist_time=millis();
    }*/
   // Serial.println(distance_recieved);
  }
  else
  {
    //detect gunshot
    digitalWrite(2,LOW);
   /* if(sample_sound())
    {
      digitalWrite(9,LOW);
      if(millis()-grabTime)
       {
          Mod02=maxVolt;
       }
 
    } */  
    if(fht_input[sound_buffer_index]>560)
    {  
      if(fht_input[sound_buffer_index]>maxVolt)
       {
         maxVolt=fht_input[sound_buffer_index];
         maxVolt_time=millis();
       }
       distance=getDist(maxVolt);
       if(distance<0)
         {
          distance=0;
         }
   
       //radio.write(&distance, sizeof(distance));    
       Serial.println(distance);
       if(millis()-maxVolt_time>50)
         maxVolt=1;
     }
  
  }
  
/*
  if(fht_input[sound_buffer_index]>560)
 {
    if(fht_input[sound_buffer_index]>maxVolt)
      {
        maxVolt=fht_input[sound_buffer_index];
        maxVolt_time=millis();
      }
     distance=getDist(maxVolt);
     if(distance<0)
       {
        distance=0;
       }
   
     //radio.write(&distance, sizeof(distance));    
     Serial.println(distance);
     if(millis()-maxVolt_time>50)
        maxVolt=1;
 }*/
  // if(millis()-min_dist_time>50)
 //     min_dist=80;    
  
  
  //text=text/100000000;
  // Serial.println(min_dist);
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


double getDist(int volts)//input adc values
{
  double dist;
  dist=(log(868.0533767/volts))/(0.1166536448);
  
  return dist;
  
}
