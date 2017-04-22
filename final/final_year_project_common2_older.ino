  
/*
This code runs on the common devices(ones with only nrf24l01+)
It sends data to main module via nrf with distance info
for now it will only detect impulse sounds(clap)


*/
#include <FHT.h>

//#include <fhtConfig.h>


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>


#define soundPin A0
#define manual_send 5


int sound_buf[256]={0};//stores the sound values
int sound_buffer_index=0;//current positions of the sound buffer index

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
char t[4];
char h='z';
String g="upp";
unsigned long i=0;
unsigned long times=i;
unsigned long impulse_start;
unsigned long impulse_end;
int re=0;
int rise=0;//rise of peak detected?
int diff=0;//adc differnces in values
int pulse_time=251;
int positve_peak=0;
int negative_peak=0;
unsigned int peak=0;
int maxVolt=0;//maximum voltage heard
long maxVolt_time=0;
int q;

void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
 radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  Serial.println("begin");
  //radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);//trasnmit distance maximum, but uses more power
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
 // delay(15000);//wait for noise to go away
}

float distance=0;
char distance2[32] = {0};//distance from mod02
void loop() 
{
  
  /*if(sample_sound())
  {
    digitalWrite(9,LOW);
    radio.write(&fht_input[sound_buffer_index-1], sizeof(fht_input[sound_buffer_index-1]));
  }
  else
  {
     digitalWrite(9,HIGH);
  }*/
  digitalWrite(9,LOW);
  fht_input[sound_buffer_index]=analogRead(soundPin);
  //radio.write(&fht_input[sound_buffer_index], sizeof((double)fht_input[sound_buffer_index]));    
  //Serial.println(fht_input[sound_buffer_index]);
  ///////////////////////////////
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
       //distance=//round(distance*100)/100;
       //itoa(distance,distance2,10);
       dtostrf(distance, 4, 2, distance2);
       //sprintf(distance2,"%lf", distance);
      strcat (distance2," w");// module tWo
     //distance2=distance
     radio.write(&distance2, sizeof(distance2)); 
     digitalWrite(9,HIGH);       
     Serial.println(distance2);     
     if(millis()-maxVolt_time>50)
        maxVolt=1;
   }
  // Serial.println(distance);     
///////////////////////////////////77
  //distance=getDist(fht_input[sound_buffer_index]); 
  
  //radio.write(&distance, sizeof(double));
   /*digitalWrite(2,LOW);
   digitalWrite(9,LOW);
   Serial.print('N');
    //char* f="#";
    digitalWrite(9,LOW);
    digitalWrite(2,HIGH);
    i=fht_input[sound_buffer_index];
    
   // radio.write(&peak, sizeof(int));
   Serial.println(peak);
     digitalWrite(9,HIGH);
     digitalWrite(2,LOW);*/
 
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
   //for calibrattion
    
    
  if(sound_buffer_index!=0)
  { 
   if ((fht_input[sound_buffer_index-1]> 600) || (fht_input[sound_buffer_index-1]<400) )
    {
      if((fht_input[sound_buffer_index-1]> maxVolt))
      {
        maxVolt=fht_input[sound_buffer_index-1];
      }
      
      return 1;  
    }
    else 
    {
      return 0;
    }
  }
}

int impulse()//returns 1 if impulse found
{
  
  /*    
  if(sound_buffer_index<255)
    {
      diff=fht_input[sound_buffer_index]-fht_input[sound_buffer_index+1];
      if(diff<0)//is 
      {
        positve_peak=fht_input[sound_buffer_index];
      }   
      if(diff>0)//is 
      {
        negative_peak=fht_input[sound_buffer_index];
      }  
      diff=abs(diff);
      
    }
  else 
     {
      diff=fht_input[255]-fht_input[0];
      if (diff<0)
      {
        positve_peak=fht_input[0];
      } 
      if(diff>0)//is 
      {
        negative_peak=fht_input[0];
      }  
      diff=abs(diff);
   
     }

  if (diff>10 && rise==0)
   {
    digitalWrite(2,HIGH);
    impulse_start=millis();
    rise=1;
    
   }
   else if(diff>10 && rise==1)
    {
      
        digitalWrite(9,HIGH);
       impulse_end=millis();
       pulse_time=impulse_end-impulse_start;
       rise=0;
    }
    else
    {
      return 0;//no pulse detected 
     }
   if(pulse_time>250 || (millis()-impulse_start)>250)
   {
      rise=0; 
      return 0;//
      
   }    
   else
    {
       return 1;
    }*/
    if (fht_input[sound_buffer_index]> 600 || fht_input[sound_buffer_index]<400 )
    {
      return 1;  
    }
    else 
    {
      return 0;
    }
}

void send_nrf(char p[])
{
 // const char text[] = "Hello World";
  radio.write(&p, sizeof(p));
  
}

double getDist(int volts)//input adc values
{
  double dist;
  dist=(log(868.0533767/volts))/(0.1166536448);
  
  return dist;
  
}
