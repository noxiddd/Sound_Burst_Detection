
////////////////////////////////////CODE IS LIFE////////////////////////////////////////////
#define F_CPU 16000000UL//redefine if necessary//will probably be 16000000UL for FHT for audio
#include <avr/io.h>
#include <avr/interrupt.h>
#include "mirf.h"
#include <stdio.h>
#include <math.h>

#define sound_duration_threshold 7812//sound duration threshold in number of cycle to 500 miliseconds
#define message_size 18  //size of the messages sent
#define ADC_ref_V    1.1 //adc reference voltage in volts
#define adc_buffer_size 36 //size of the buffer for ADC values
#define nrf_buffersize 18


/////////////////////////////////////////////////////////////////////////////////////////////
float* fast_hartley_transform(float* signal);
float convert_ADC_to_float(unsigned int ADC_output);
void intialise_ADC();
void read_ADC();
void add_ADC_to_buffer(unsigned int adc_val);
void nrf_init();
/////////////////////////////////////////////////////////////////////////////////////////////

/********************************GLOBAL VARIABLES***********************/
char data_NRF[18];
unsigned int* buffer;//store adc values
unsigned int adc_val_index=-1;//this stores the current position of the values added to the adc_buffer
float distance=0;
/***********************************************************************/



/***************************************QUESTIONS*******************************************************/

//code for making device hear, therefore ADC
//needs to have serial communication capability to talk to GSM and NRF2401 devices

//Create a web server for the PC side

//DO I NEED TO TRANSFER ONE CHARACTER AT A TIME TO TRANSMIT A STRING??????
/*************************************************************************************************/



/**********************CODING SCHEME FOR MESSAGE******************************/

     //example message  //START_032.50_Y_END //saying gunshot confirmed at 32.5 meters from said module


//START-signals beginning of message
//distance in meters of module from gunshot event, to two decimcal places
//is sound confirmed to be gunshot Y for yes and N for no at least one module must confirm
//END end of message

//when event is heard //maybe just register time at the server 

/***********************CODING SCHEME FOR MESSAGE***************************/





/***********************************FUNCTION DEFINITIONS************************************/
void intialise_timer1()
{
    TCCR1A=0;
    TCCR1B=0x0F;// clk/1024 15625==>0.000064 seconds per cyle//tc0 256 cyles is 0.01634 seconds * 30 0.49 seconds
    TCCR1C=0;
    TIMSK1=1;//timer counter 0 interrupt enable
    TCNT1=0x0000;//start timer at 0
}


void intialise_ADC()
{
   //ADMUX 0b00000000
   ADMUX=0;//ADC0, no left adjust, internal reference turned off
   //should i use auto trigger
   //ADCSRA 0b11000100 ADC enable,start conversion,no auto trigger,interrupt flag,interrupt disable,ADC clk/16
   ADCSRA=0xC4;
   //ADCSRB 0 no multplexor thing ,not auto trigger
   ADCSRB=0;
}

void read_ADC()//reads and store the adc into the global adc buffer
{
   ADCSRA|=(1<<ADSC);//start the ADC conversion
   while(ADCSRA&(1<<ADSC));//wait for conversion to finish
   add_ADC_to_buffer(ADC);
}

void add_ADC_to_buffer(unsigned int adc_val)
{
   if(adc_val_index<(adc_buffer_size))
    { 
          buffer[adc_val_index++]=adc_val;//increment buffer index
    }
    else
    {
        adc_val_index=0;//makes the addition of data cyclic
    }
}

float convert_ADC_to_float(unsigned int ADC_output)//convert from integers to actual voltage values
{
   float V_res,Voltage;;
   V_res=ADC_ref_V/1024;//calculate voltage resolution
   Voltage=V_res*ADC_output;
   return Voltage;
}



char* compile_string(char data)
{//data size about 20
  char* compil;
  int i=0;
    for(i=0;i<message_size;i++)
    {
        compil[i]=read_data_NRF();
    }
    return compil;
}
//no boolean data type?
/*boolean*/unsigned int is_gun_detected(float adc_val)
{


}
/*boolean*/
unsigned int is_event_detected()//tell if gunshot like sound is heard (impulse) n//////REDO FUNCTION
{
    unsigned int past_adc_val=0;
    unsigned int current_adc_val=0;
    unsigned int sound_duration=0;
    unsigned int i=1;//buffer incrementor
    //code for detecting clap like sound here
    int* buffer;
    buffer[0]=0;//past adc value
    buffer[1]=0;//current adc value
    while((buffer[i]-buffer[i-1])<100)//no large changes in sound level (RISE) adjust value 100 if necessary
     {  
         i++;
         // buffer[i]=read_ADC();
     }   
     //start timer here
    TCNT1=0;//restart timer from 0
    while((buffer[i-1]-buffer[i]<100 && TCNT1<sound_duration_threshold));//no large changes in sound level (FALL)
    {
        i++;
        //buffer[i]=read_ADC();
    }
    sound_duration=TCNT1;
    //1 second is 15626 cycles 
    if(sound_duration<sound_duration_threshold)//
      {
           return 1;//event detected
      }
      else
      {
          return 0;//event not detected
      }
    //stop timer here
    
return 0;
//not all devices may postively confirm gunshot, but at least one needs to,
//others may heard sound similar to gunshot, which may be actual gunshot
//so they need to report it
}

/*****************************FUNCTION DEFINITIONS**********************/





/********************INTERRUPT SERVICE ROUINES**********************/

/*ISR(NRF_data_reception_interrupt)//interrupt service routine for NRF data reception
{
    data_NRF=read_data_NRF();

}

ISR(GSM_data_reception_interrput)//interrupt service routine for GSM data reception
{

    data_GSM=read_data_GSM();
}

*/

/************************INTERRUPT SERVICE ROUTINES*****************/


void nrf_init()
{
 // Initialize AVR for use with mirf
	 mirf_init();
	 // Wait for mirf to come up
	 _delay_ms(50);
	 // Activate interrupts

	 // Configure mirf
	 mirf_config();
     //set reciever address
     mirf_set_TADDR((uint8_t *)"Main");
}


int  main(void)
{
     sei();
    nrf_init();

   
   float adc_val=0;//stores values of adc
   intialise_timer1();
   intialise_ADC();//
   
  



   while(1)//infinite loop
   {
     read_ADC();//read and store ADC value in the adc store buffer   
     
     switch(is_event_detected())
     {
     case 0://detected but not sure if gun shot   
         //send_string_NRF(nrf_data);//send data to main module
         sprintf(data_NRF,"START_%f_N",distance);
         mirf_send(data_NRF,nrf_buffersize);
      break;

      case 1://detected and positive ID of gunshot
          //send_string_NRF(nrf_data);//send data to main module
         sprintf(data_NRF,"START_%f_Y",distance);
         mirf_send(data_NRF,nrf_buffersize);
      break;
     }
     
   }



  return 0;
}




float* fast_hartley_transform(float* signal)
{
    for (int i=0;i<=M-1;i++) 
    {
         if(i%2==0)//even index
         {
            H0= X0*cos((2*pi*f*t)/M)+X0*sin((2*pi*f*t)/M);
         }
         else//odd index 
         {
             H1=X1*cos((2*pi*f*t)/M)+X1*sin((2*pi*f*t)/M);
         }
    }
    H0=H0*(1/M);
    H1=H1*(1/M);
}