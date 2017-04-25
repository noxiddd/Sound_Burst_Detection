#include <SoftwareSerial.h>


#define LIN_OUT8 1//use the lin out function
#define FHT_N 32 // set to 32 point fht///define before fht.h
#include <FHT.h>  

# include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <FHT.h>
     /* strtof */


RF24 radio(7, 8);

const byte rxAddr[6] = "00002";
double text=0;
int maxVolt=0;

char mod01[4]={0};
char mod02[4]={0};
char mod03[4]={0};



#define soundPin A0
#define manual_send 5

int data_count=0;//data from each module present
int reference_fht[16]={2,45,55,19,8,19,11,3,9,9,4,3,6,5,1,1};//specrtum against which detection test is done
int sound_buf[256]={0};//stores the sound values
int sound_buffer_index=0;//current positions of the sound buffer index
double min_dist=80;
long min_dist_time=0;
char distance2[32] = {0};

long maxVolt_time=0;

float distance=0;
float distance01=0;
float distance01_min=0;
float distance02=0;
float distance02_min=0;
float distance03=0;
float distance03_min=0;




double signalEnergy=0;
//here distance min is actually energy max being sent for callibration


unsigned long mod01_recieve_time=0;
unsigned long mod02_recieve_time=0;
unsigned long mod03_recieve_time=0;

char distance_recieved[32] = {0};
void setup()  
{
  pinMode(9,OUTPUT);
  pinMode(2,OUTPUT);
 // while (!Serial);
  Serial.begin(57600);
  Serial.println("Begin");
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  //radio.openReadingPipe(1, rxAddr[1]);
  
  
  radio.startListening();
  
  ADCSRA = 0xe5  ; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop()
{
  check_for_gunshot();  
  if (radio.available())
  {
    check_for0103data();//check for modules 1 and 3
    check_for_gunshot();   
  }
  
}

void senToServer()
{//send to pi/gsm which sends to server
   Serial.println(distance01) ;
   Serial.println(distance02);
   Serial.println(distance03);
  
}

void check_for_gunshot()
{digitalWrite(2,LOW); 
  if(gunshot_detected())
   {
      mod02_recieve_time=millis();
      distance02=getDist(signalEnergy);
      Serial.println(distance02);
      if (abs(mod02_recieve_time-mod01_recieve_time)<500 && abs(mod02_recieve_time-mod03_recieve_time)<500)
       {//gunshot detected
          senToServer();
       }
     digitalWrite(2,HIGH);         
   }
}

void print_datas()
{
  if(millis()-mod03_recieve_time>100)
      {
           Serial.print("Module 3: ");          
           Serial.print(distance03_min);       
           Serial.print("  ");       
      }
   
           
     if((millis()-mod02_recieve_time)>100)
      {
           Serial.print("Module 2: "); 
           Serial.print(distance02_min); 
           Serial.print("  ");       
      }

      
     if((millis()-mod03_recieve_time)>100)
      {
           Serial.print("Module 1: "); 
           Serial.println(distance01_min);
           Serial.println("  ");        
      }
        
}

void check_for0103data()
{
   
    radio.read(&distance_recieved, sizeof(distance_recieved));
    int i=0;
    while(distance_recieved[i]!='w' && distance_recieved[i]!='h')
    {
         i++;
    }
    
    if(distance_recieved[i]=='h')
    {
      mod03_recieve_time=millis();
      distance03=atof(distance_recieved);  
      if(distance03_min<distance03)    
       {
           distance03_min=distance03;
       }   
      //Serial.println(mod03_recieve_time); 
    }
    else if(distance_recieved[i]=='n')
    {
      mod01_recieve_time=millis();
      distance01=atof(distance_recieved);  
      if(distance01_min<distance01)    
       {
           distance01_min=distance01;
       }   
      //Serial.println(mod03_recieve_time); 
    }  
}




double getDist(int volts)//input adc values
{
  double dist;
  dist=(log(868.0533767/volts))/(0.1166536448);
  
  return dist;
  
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

      signalEnergy=getSignalEnergy();
        
     
       
      if(correlation()>0.9)//threshold
      {
        Serial.println("Detected");
        return 1;//gunshot detected
        
      }
      else 
      {
        //Serial.println(signalEnergy);
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
    Serial.println(corr);
    return corr;//corryy;
  }
