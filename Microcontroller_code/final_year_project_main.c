#define F_CPU 16000000UL//redefine if necessary//will probably be 16000000UL for FHT for audio
#include <avr/io.h>
#include <avr/interrupt.h>


#define CSN PORTB1   //tell nrf rhat something is being sent to it
#define CE PORTB2    //send/recieve sig
#define message_size 18  //size of the messages sent
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

/*******************************************************/



/**********************FUNCTION DEFINITIONS**************************/
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

unsigned int read_ADC()//returns adc value as integer
{
   ADCSRA|=(1<<ADSC);//start the ADC conversion
   while(ADCSRA&(1<<ADSC));//wait for conversion to finish
   return ADC;
}

float convert_ADC(unsigned int data)//convert from integers to actual voltage values
{
   
}

void intialise_spi_NRF()
{
   //set sck,mosi, csn and ce as output
   DDRB|=(1<<PORTB1)|(1<<PORT2)|(1<<PORT3)|(1<<PORT5);
   //set miso to input
   DDRB|=(1<<PORTB4);
   //enble spi master set clock rate fck/16/clocl rate not that important
   SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);//enable spi, set as master mode and spi clocl fck/16
   //csn pin to low for nrf to listen to commands, set to high not ready for commands yet
   PORTB&=~(1<<CE);//low at beginning nothing to send or recieve yet
   PORTB|=(1<<CSN);//high to start with nothing to send to nrf yet
}


void send_data_NRF(char data)//spi
{//start transmission 	
  
    SPDR=data;
   //wait for transmission to complete
   while(!(SPDR & (1<<SPIF)));
   
}

void send_string_NRF(char* data)
{
    int i=0;
    for(i=0;i<message_size;i++)
    {
        send_data_NRF(data[i]);
    }
}



//does it only transmit one byte at a time do i have to loop to get multiple characters

char read_data_NRF()//spi
{
  //wait for reception complete
   while(!(SPSR & (1<<SPIF)));
  //return data register
   return SPDR;

}

void initialise_serial_GSM()
{  //UCSR0A 0b0000000
  UCSR0A=0;//all flags register
  //UCSR0B 0b00011000
  UCSR0B=(1<<RXEN0)|(1<<TXEN0);//0x18;//reciever and transmitter enable
  //UCSR0C 0b00000011
  UCSR0C=0x03;//asynchronous usart, parity disabled, 1 stop bit 8 data bits
  //UBRR=103;
  UBRR0L=0x67;//9600 buad is 103 in UBRR at 16 Mhz
  UBRR0H=0;
}

void send_data_GSM(unsigned char data)//usart 
{
   while(!(UCSR0A & (1<<UDRE0)));//wait for UDR to empty. wait for empty transmit buffer
   UDR0=data;

}
 void send_string_GSM(unsigned char* data)
 {   int i=0;
     for(i=0;i<message_size;i++)
     {
       send_data_GSM(data[i]);
     }
 }

unsigned char read_data_GSM()
{
 /* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)));
/* Get and return received data from buffer */
return UDR0;
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

/*boolean*/ unsigned int is_data_recieved()//to tell the device when data is recieved
{

}

/*boolean*/unsigned int data_verifications()// to tell if data recieved is corrupt--advanced
{

}


char* event_synthesis(float adc_val)
{

}

char device_recieved_data()//which device data was recieved
{

}

char* calculate_gun_coordinates()
{

}

/*****************************FUNCTION DEFINITIONS**********************/


/********************INTERRUPT SERVICE ROUINES_begin**********************/


/*ISR(NRF_data_reception_interrupt)//interrupt service routine for NRF data reception
{
    data_NRF=read_data_NRF();

}

ISR(GSM_data_reception_interrput)//interrupt service routine for GSM data reception
{

    data_GSM=read_data_GSM();
}

*/

/************************INTERRUPT SERVICE ROUTINES_end*****************/

char* data_NRF;
char* data_GSM;

int  main(void)
{
   sei();//set global interrupts

   
   float adc_val=0;//stores values of adc
   intialise_ADC();//
   intialise_serial_GSM();//GSM module uses serial communications
   intialise_spi_NRF();//NRF uses spi module

   char* nrf_data;//stores the nrf_data recieved
   char* gsm_data;//stores the gsm_data recieved
   char action;//choosed action for device

   char* coord;//calculate gun coordinates of gunshot event and store in this variable

   while(1)//infinite loop
   {
     if(is_data_recieved()==1)
      {
           //processs data
           if(device_recieved_data()=='N')
            {
               //for NRF
               nrf_data=compile_string(read_data_nrf());//get data from nrf
               action=extract_action(nrf_data);//retrieve intruction from data
               switch(action)//action performed upon reception of nrf_data/probably should save all data before sending so it can be resent
                 {
                     case 'a':
                      send_string_NRF(nrf_data);//pass data through network of devices
                     break;

                     case 'b':
                      //send_data_
                     break;
            
                     case 'c':
                       save_data_EEPROM(nrf_data);//might need to select secific location in memory
                       action='d';
                     break;
                     case 'd':
                         send_string_GSM(nrf_data);//pass data to server
                     break;
                 }
            }
            else if(device_recieved_data()=='G')//action performed upon recption of gsm_data
            {
                 //for GSM
                gsm_data=compile_string(read_data_gsm());
                action=extract_action(gsm_data);//retrieve instruction from data
                switch(action)
                 {
                    case 'a':
                      send_string_GSM(gsm_data);//send data to server 
                    break;
                  
                    case 'b':
                       if(event_devices()==3)
                        {
                           coord=calculate_gun_coordinates();//collect data from EEPROM and 
                            //calculate the coordinates of the event from known locations 
                            //of the devices based on ID
                        }
                    break;

                    case 'c':
                        save_data_EEPROM(gsm_data);//might need to select specific location on memory
                        action='b';//go to 
                    break;
                 }
            }
      }
      else
      {
         adc_val=read_ADC();
         
         if(is_gun_detected(adc_val)==1)
           {
              send_string_GSM(event_synthesis(adc_val));
             //or send_data_NRF(event_synthesis(adc_val)); for non node
           }    
      }
    
   }



  return 0;
}


