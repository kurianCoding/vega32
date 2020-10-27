#include "stdlib.h"
#include "config.h"
#include "uart.h"
#include "adc.h"
#include "interrupt.h"
#define USEC_TO_SEC 		1000000
#define CONV_FACTOR            0.00244140625
void main() 
{
	UI adc_data, temp_data = 0;
    clock_t startTime, endTime;
    long int totaltime =0;
    int pulsecount, rate =0;
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
	 
