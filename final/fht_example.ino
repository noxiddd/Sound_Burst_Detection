/*
    fht_adc.pde
    guest openmusiclabs.com 9.5.12
    example sketch for testing the fht library.
    it takes in data on ADC0 (Analog0) and processes them
    with the fht. the data is sent out over the serial
    port at 115.2kb.  there is a pure data patch for
    visualizing the data.
    */
   
   ///#define LOG_OUT 1 // use the log output function
   #define LIN_OUT8 1//use the lin out function
   #define FHT_N 32 // set to 32 point fht
  
   #include <FHT.h> // include the library


   int reference_fht[16]={2,45,55,19,8,19,11,3,9,9,4,3,6,5,1,1};//specrtum against which detection test is done
    
   void setup() {
     Serial.begin(57600); // use the serial port
     TIMSK0 = 0; // turn off timer0 for lower jitter
     ADCSRA = 0xe5; // set the adc to free running mode
     ADMUX = 0x40; // use adc0
     DIDR0 = 0x01; // turn off the digital input for adc0
     delay(5000);
   }
   
   void loop() {
     while(1) { // reduces jitter
       cli();  // UDRE interrupt slows this way down on arduino1.0
       for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
         while(!(ADCSRA & 0x10)); // wait for adc to be ready
         ADCSRA = 0xf5; // restart adc
         byte m = ADCL; // fetch adc data
         byte j = ADCH;
         int k = (j << 8) | m; // form into an int
         k -= 0x0200; // form into a signed int
         k <<= 6; // form into a 16b signed int
         fht_input[i] = k; // put real data into bins
       }
       fht_window(); // window the data for better frequency response
       fht_reorder(); // reorder the data before doing the fht
       fht_run(); // process the data in the fht
       //fht_mag_log(); // take the output of the fht
       fht_mag_lin8();
       sei();
       //Serial.write(255); // send a start byte
       //Serial.write(fht_lin_out8, FHT_N/2); // send out the data
       Serial.println("Begin");
         for(int i=0;i<FHT_N/2;i++)
          {
           Serial.print(i);
           Serial.print(" "); 
           Serial.println(fht_lin_out8[i]);
          }
        //  visualizer();
       Serial.println("End");
       float r=correlation();
          Serial.print("Correlation value: ");
          Serial.println(r);
        if(r>0.9)
        {
       
           while(1);
        }
       }
     
   }


  void visualizer()
  {
    for(int i=0;i<FHT_N/2;i++)
          {
           Serial.print(i);
           Serial.print(" "); 
           Serial.print("#");
           for(int y=0;y<fht_lin_out8[i];y=y+2)
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
