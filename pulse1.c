#include "stdlib.h"
#include "config.h"
#include "uart.h"
#include "adc.h"
#include "interrupt.h"
#include "gpio.h"
#include "led.h"
//A0: pulse output
//A1: MQ out
//A2: Hout
//A3: Tout
#define USEC_TO_SEC 		1000000
#define CONV_FACTOR            0.00244140625
#define PULSE_THRESHOLD  70 // threshold value for pulse
#define PULSE_INIT   60     // initial value for pulse
#define MQ_THRESHOLD_1 4  // threshold 1 for MQ sensor
#define MQ_THRESHOLD_2 8  // threshold 2 for MQ sensor
#define MQ_THRESHOLD_3 12 // threshold 3 for MQ sensor
#define MQ_INIT  1       // initial value for MQerature
#define H_THRESHOLD 3.0
#define T_THRESHOLD 2.5
#define MAX_VALUE 1000000   // mod to prevent buffer overflow

void main() 
{
	UI adc_data, MQ_data, hout_data, tout_data = 0;
    clock_t startTime, endTime;
    long int totaltime =0;
    int pulsecount, rate =0;
    int MQ_average=MQ_INIT,pulse_average=PULSE_INIT; //intializing MQ and pulse average values
    long int counter=1;
    float adc_out, MQ_out,hout, tout=0;
         startTime = read_csr(mcycle);
         endTime   = read_csr(mcycle);

    while(1)
    {
      totaltime = ((endTime - startTime)*0.025)/ USEC_TO_SEC;
       if (totaltime < 60) 
       {
	  endTime = read_csr(mcycle);

	}
	else
	{ 

	  rate = pulsecount; 
          startTime = read_csr(mcycle);
	  endTime  =  read_csr(mcycle);
	  pulsecount =0;
  	  printf("\r Pulse rate %d MQ %f",rate, MQ_data);
	  counter=(counter+1)%MAX_VALUE;
	  MQ_average=((MQ_data+MQ_average*counter)/(counter+1))%MAX_VALUE;
	  pulse_average=((rate+pulse_average*counter)/(counter+1))%MAX_VALUE;
	  if (MQ_average>MQ_THRESHOLD_3){
	    // set MQ alert red;
	     on_LED(PIN_19);
	   }else{
	     off_LED(PIN_19);
	   }
	  if (MQ_average<MQ_THRESHOLD_3 && MQ_average>MQ_THRESHOLD_2){
	    // set MQ alert blue;
	     on_LED(PIN_21);
	   }else{
	     off_LED(PIN_21);
	   }
	  if (MQ_average<MQ_THRESHOLD_2 && MQ_average>MQ_THRESHOLD_1){
	    // set MQ alert green;
	     on_LED(PIN_20);
	   }else{
	     off_LED(PIN_20);
	   }

	  if (pulse_average>PULSE_THRESHOLD){
	    // set pulse alert;
	      on_LED(PIN_17);
	  }else{
	      off_LED(PIN_17);
	  }
 	if (hout>H_THRESHOLD){
	    // set humidity alert;
	     on_LED(PIN_16);
	   }else{
	     off_LED(PIN_16);
	   }
 	if (tout>T_THRESHOLD){
	    // set tempreature alert red;
	     on_LED(PIN_22);
	   }else{
	     off_LED(PIN_22);
	   }


       }

 
       adc_data = adc_analogRead(A0);
       MQ_data= adc_analogRead(A1);
       hout_data = adc_analogRead(A2);
       tout_data = adc_analogRead(A3);
       adc_out  = adc_data*CONV_FACTOR;
       MQ_out = MQ_data*CONV_FACTOR;
       hout = hout_data * CONV_FACTOR;
       tout = tout_data*CONV_FACTOR;
       
       printf("\r conv %f pulse %d MQ %f hout %f tout %f tavg %d count %d", adc_out,pulsecount,MQ_out, hout, tout, MQ_average, counter);
       if(adc_out > 9.1)
       pulsecount++; 
       } 
    
  }   
	 
