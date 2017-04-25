#define LIN_OUT8 1//use the lin out function
#define FHT_N 32 // set to 32 point fht///define before fht.h
#include <FHT.h>  
/*
This code runs on the common devices(ones with only nrf24l01+)
It sends data to main module via nrf with distance info
for now it will only detect impulse sounds(clap)


*/


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

const byte rxAddr[6] = "00002";
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
int ambient_voltage_accumulate=0;
int ambient_voltage=0;
unsigned long ambient_voltage_time=0;
int ambient_voltage_count=0;
double signalEnergy=0;
double max_signalEnergy=0;
unsigned long max_signalEnergy_time=0;
double energy_hold=0;
int take_signal_time=1;//true or false 0 true 1 false


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
 

  //TIMSK0 = 0; // turn off timer0 for lower jitter
 ADCSRA = 0xe5  ; // set the adc to free running mode
 ADMUX = 0x40; // use adc0
 DIDR0 = 0x01; // turn off the digital input for adc0

 // delay(15000);//wait for noise to go away
}


int reference_fht[16]={2,45,55,19,8,19,11,3,9,9,4,3,6,5,1,1};//specrtum against which detection test is done
float fht_reorder_hold[FHT_N]={0};
float fht_run_hold[FHT_N]={0};
float fht_window_hold[FHT_N]={0};
float fht_lin8_hold[FHT_N]={0};//hold the value so operations can go on
float fht_hold[FHT_N]={0};//hold the value so operations can go on


float distance=0;
char distance2[32] = {0};//distance from mod03

int toggle=0;//to measure sampling rate 
int incre=1;//controls fht processes
int allow_fht=0;
int do_nrf_send=0;

void loop() 
{
  
  
  digitalWrite(9,LOW);
  digitalWrite(2,LOW);

 
  if(gunshot_detected())
   {//Serial.print("Here"); 
    digitalWrite(2,HIGH);
     distance=getDist(maxVolt);
       if(distance<0)
       {
        distance=0.1;
       }
       //Serial.println(distance2);
       //dtostrf(getDist(signalEnergy), 4, 2, distance2);
       dtostrf(getDist02(max_signalEnergy), 4, 2, distance2);
       //Serial.print(" ");
       
      strcat (distance2," n");// module one
      //Serial.println(distance2);    
      //radio.write(&distance2, sizeof(distance2)); 
      digitalWrite(9,HIGH);   

   }
   if (millis()-max_signalEnergy_time>1000)
        {
          if(do_nrf_send==1)
            {
              Serial.println("send to nrf"); 
              do_nrf_send=0;
               /* radio.write(&distance2, sizeof(distance2));
          Serial.println("sent to nrf"); */
          //Serial.println("sent to nrf");
            }
        
          max_signalEnergy=0;
          take_signal_time=1;
        }
 
  
  
 
}


double getDist02(int volts)//input adc values
{
  double dist;
  dist=(log(9976.370441/volts))/(0.6126089054);
  
  return dist;
  
}



  void visualizer()
  {
    for(int i=0;i<FHT_N/2;i++)
          {
           Serial.print(i);
           Serial.print(" "); 
           Serial.print("#");
           for(int y=0;y<fht_lin_out8[i];y++)
            {
              if(y<10)
              {
                Serial.print(" ");
              }
              Serial.print("#");
            }
            Serial.println(" ");
          }
  }

double getDist(int energy)//input adc values
{
  double dist;
  dist=(log(19396.67306/energy))/(0.9125233908);
  
  return dist;
  
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
    return corr;//corryy;
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
       fht_mag_lin8();
       
      
       sei();

         //Serial.print("Energy: ");
          
         // Serial.println(signalEnergy);
         energy_hold=getSignalEnergy();
         if(max_signalEnergy<energy_hold)
           {
            max_signalEnergy=energy_hold;
              if (take_signal_time==1) 
               {
                 max_signalEnergy_time=millis();
                 take_signal_time=0;
                 
               }
           }
            signalEnergy=getSignalEnergy();
       
      if(abs(correlation())>0.9)//threshold
      {
        do_nrf_send=1;//send
        return 1;//gunshot detected
      }
      else 
      {
        return 0;//no gunshot
      }
  
}

void ambiency(int i)//supposed to find nominal values for volatge and detect peak over that
{
  if (abs(ambient_voltage-fht_input[i])>60)//threshold
            {
              allow_fht=1;
            }
          else
          {
            allow_fht=0;
          }  
          if ((millis()-ambient_voltage_time)>100)
          {
            ambient_voltage_time=millis();
              ambient_voltage_accumulate=fht_input[i]+ambient_voltage_accumulate;
            ambient_voltage_count++;
          }
          else
          {
            ambient_voltage=ambient_voltage_accumulate/ambient_voltage_count;
            Serial.print("ambient volatge: ");
            Serial.println(ambient_voltage);
            ambient_voltage_count=0;
          }

}

void faster_fht()
{
   if(incre==1)
       {  
        fht_window(); // window the data for better frequency response
        memcpy(fht_window_hold,fht_input,FHT_N);
        incre++;
       }
       else if(incre==2)
       {
         memcpy(fht_hold,fht_input,FHT_N);//hold on this for me
         memcpy(fht_input,fht_window_hold,FHT_N);
         fht_reorder(); // reorder the data before doing the fht
         memcpy(fht_reorder_hold,fht_input,FHT_N);
         
         memcpy(fht_input,fht_hold,FHT_N);
         fht_window(); // window the data for better frequency response  
         memcpy(fht_window_hold,fht_input,FHT_N); 
         incre++;
       }
       else if (incre==3)
       {
         memcpy(fht_hold,fht_input,FHT_N);//hold on this for me
       
         memcpy(fht_input,fht_reorder_hold,FHT_N);
         fht_run(); // process the data in the fht
         memcpy(fht_run_hold,fht_input,FHT_N);

         memcpy(fht_input,fht_window_hold,FHT_N);
         fht_reorder(); // reorder the data before doing the fht
         memcpy(fht_reorder_hold,fht_input,FHT_N);

         memcpy(fht_input,fht_hold,FHT_N);
         fht_window(); // window the data for better frequency response  
         memcpy(fht_window_hold,fht_input,FHT_N); 
         
         incre++;
       }
       else if(incre==4)
         {
          memcpy(fht_hold,fht_input,FHT_N);//hold on this for me

          memcpy(fht_input,fht_run_hold,FHT_N);
          fht_mag_lin8();//then goes to fht lin_out

          memcpy(fht_input,fht_reorder_hold,FHT_N);
          fht_run(); // process the data in the fht
          memcpy(fht_run_hold,fht_input,FHT_N);

          memcpy(fht_input,fht_window_hold,FHT_N);
          fht_reorder(); // reorder the data before doing the fht
          memcpy(fht_reorder_hold,fht_input,FHT_N);

          memcpy(fht_input,fht_hold,FHT_N);
          fht_window(); // window the data for better frequency response  
          memcpy(fht_window_hold,fht_input,FHT_N); 
          
          incre=1;
         }
}

void checkSampleFreq()
{
   
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
     
         if(toggle==0)
         {
          toggle=1;
          digitalWrite(9,HIGH);
         }
         else 
         {
           toggle=0;
           digitalWrite(9,LOW);   
          }
}
  }



double getSignalEnergy()
{double temp=0;
  for(int i=0;i<FHT_N/2;i++)
      {      
          temp=(fht_lin_out8[i]*fht_lin_out8[i])+temp;
       }

       if(temp<0)
       {
        temp=0;
       }
     return 2*temp;  
}
