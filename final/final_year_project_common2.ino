#define LIN_OUT8 1//use the lin out function
#define FHT_N 32 // set to 32 point fht///define before fht.h
#include <FHT.h>

  
/*
This code runs on the common devices(ones with only nrf24l01+)
It sends data to main module via nrf with distance info
for now it will only detect impulse sounds(clap)


*/

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


///detection algorithms
//int reference_fht[16]={12,59,41,21,17,13,11,9,8,8,7,7,6,6,6,6};//specrtum against which detection test is done
int reference_fht[16]={2,45,55,19,8,19,11,3,9,9,4,3,6,5,1,1};//specrtum against which detection test is done
///

void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
 Serial.println("begin");
 radio.begin();
 radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  
  //radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);//trasnmit distance maximum, but uses more power
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
 // delay(15000);//wait for noise to go away

 //TIMSK0 = 0; // turn off timer0 for lower jitter
 ADCSRA = 0xe5; // set the adc to free running mode
 ADMUX = 0x40; // use adc0
 DIDR0 = 0x01; // turn off the digital input for adc0
}

float distance=0;
char distance2[32] = {0};//distance from mod02
void loop() 
{
  digitalWrite(9,HIGH);
  digitalWrite(2,HIGH);
  
  if(gunshot_detected())
   {//Serial.print("Here"); 
    digitalWrite(2,LOW);
     distance=getDist(maxVolt);
      if(distance<0)
       {
        distance=0;
       }
       dtostrf(distance, 4, 2, distance2);
      strcat (distance2," w");// module tWo
     //radio.write(&distance2, sizeof(distance2)); 
     digitalWrite(9,LOW);       
     Serial.println(distance2);     
     if(millis()-maxVolt_time>50)
        maxVolt=1;//reset maxvolt
   }
 
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
    
    
  /*if(sound_buffer_index!=0)
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
  }*/
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

int gunshot_detected()
{
  
       cli();  // UDRE interrupt slows this way down on arduino1.0
       for (int i = 0 ; i < FHT_N ; i++)
       { // save 32 samples
         while(!(ADCSRA & 0x10)); // wait for adc to be ready
         ADCSRA = 0xf5; // restart adc
         byte m = ADCL; // fetch adc data
         byte j = ADCH;
         int k = (j << 8) | m; // form into an int
         k -= 0x0200; // form into a signed int
         k <<= 6; // form into a 16b signed int
         fht_input[i] = k; // put real data into bins
         if(fht_input[i]>maxVolt)
         {
          maxVolt=fht_input[i];
          maxVolt_time=millis();
         }
       }
      
       fht_window(); // window the data for better frequency response
       fht_reorder(); // reorder the data before doing the fht
       fht_run(); // process the data in the fht
       //fht_mag_log(); // take the output of the fht
       fht_mag_lin8();
       sei();

       
       Serial.println("Start");
         for(int i=0;i<FHT_N/2;i++)
          {
           Serial.print(i);
           Serial.print(" "); 
           Serial.println(fht_lin_out8[i]);
          }
       Serial.println("End");
       
      if(abs(correlation())>0.8)//threshold
      {
        return 1;//gunshot detected
      }
      else 
      {
        return 0;//no gunshot
      }
  
}


  double correlation()
  {
    double corrxy=0;//correlation value
    double corrxx=0;
    double corryy=0;
    double corr=0;//final correlation value
    double mult=0;
    for(int i=0;i<FHT_N/2;i++)
    {
       corrxy=(reference_fht[i]*fht_lin_out8[i])+corrxy;
    } 
    for(int j=0;j<FHT_N/2;j++)
    {
       corrxx=(reference_fht[j]*reference_fht[j])+corrxx;
    }
    for(int k=0;k<FHT_N/2;k++)
    {
       corryy=(fht_lin_out8[k]*fht_lin_out8[k])+corryy;
    }
    mult=corrxx*corryy;
    corr=corrxy/sqrt(mult); 
    //Serial.print("corr corrxy corrxx corryy mult");
    //Serial.print(corr);Serial.print(" ");Serial.print(corrxy);Serial.print(" ");Serial.print(corrxx);
    //Serial.print(" ");Serial.print(corryy);Serial.print(" ");Serial.println(mult);
    return corr;//corryy;
  }
