#include "stdlib.h"
#include "config.h"
#include "uart.h"
#include "adc.h"
#include "interrupt.h"
#define USEC_TO_SEC 		1000000
#define CONV_FACTOR            0.00244140625
#define PULSE_THRESHOLD  70; // threshold value for pulse
#define PULSE_INIT   60;     // initial value for pulse
#define TEMP_THRESHOLD 7;   // threshold value for pulse(0-10)
#define TEMP_INIT  60;       // initial value for temperature
void main() 
{
	UI adc_data, temp_data = 0;
    clock_t startTime, endTime;
    long int totaltime =0;
    int pulsecount, rate =0;
    int temp_average=TEMP_INIT,pulse_average=PULSE_INIT; //intializing temperature and pulse average values
    int counter=1;
    float adc_out, temp_out=0;
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

	  rate = pulsecount/60; 
          startTime = read_csr(mcycle);
	  endTime  =  read_csr(mcycle);
	  pulsecount =0;
  	  printf("\rPulse rate is %d", rate);
	  counter++;
	  temp_average=(temp_data+temp_average)/counter;
	  pulse_average=(pulse_data+pulse_average)/counter;

	  if (temp_average>TEMP_THRESHOLD){
	    // set temp alert;
	     on_LED(PIN_16);
	   }else{
	     off_LED(PIN_16);
	   }
	  if (pulse_average>PULSE_THRESHOLD){
	    // set pulse alert;
	      on_LED(PIN_17);
	  }else{
	      off_LED(PIN_17);
	  }

       }

 
       adc_data = adc_analogRead(A0);
       temp_data= adc_analogRead(A1);
       adc_out  = adc_data*CONV_FACTOR;
       temp_out = temp_data*CONV_FACTOR;
       printf("\rtemp %x",temp_data);
       if(adc_out > 9.1)
       pulsecount++; 
       } 
    
  }   
	 
