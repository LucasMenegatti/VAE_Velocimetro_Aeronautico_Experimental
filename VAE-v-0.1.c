
#include "16F873A.h"
#device adc=10
#include <math.h>
#USE DELAY (CLOCK=10000000)

#Fuses HS, NOWDT, PUT, NOPROTECT, NOLVP

#byte portb=0x06
#byte portC=0x07
#define cont_h 10

byte cont;
int16 adc_value;

#int_rtcc
le_adc(){
if (--cont==0){
   adc_value = read_adc();
   delay_us(5);
   cont=cont_h;
}
return(adc_value);
}

int z = 1;

#int_ext
temp_or_spd(){
 if (z==1){
 z=0;
  set_adc_channel(0);
  delay_us(30);
 }
 else
 {
 z=1;
 set_adc_channel(1);
 delay_us(30);
 }
delay_ms(200);
return(z);
}

unsigned Dig[4];
int16 Out0;
double BrA=0;
double BrB=0;
double BrC=0;

void main(){

   set_rtcc(0);
   setup_counters(RTCC_INTERNAL, RTCC_DIV_128); 
   enable_interrupts(INT_RTCC);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
   
   setup_adc_ports(AN0_AN1_AN3);
   setup_adc(ADC_CLOCK_DIV_16);
   set_adc_channel(1);
   delay_us(30);
   adc_value = read_adc();
   delay_us(5);

   Set_Tris_A(0b00000111);
   Set_Tris_B(0b1);
   Set_Tris_C(0b0);

while(1){

   if(z==0){
   set_adc_channel(0);
   delay_us(30);
   Out0=(((adc_value)-518)*2);
   delay_ms(1);
   }
   else
   {
   BrA=((9.8*(adc_value-251.6))/1.29);
   BrB=Sqrt(BrA);
   BrC=BrB*1.943846;
   Out0=BrC;
   delay_ms(1);
   }

   Dig[0]=((Out0)%10) << 1;
   Dig[1]=((Out0/10)%10) << 1;
   Dig[2]=((Out0/100)%10) << 1;
   Dig[3]=((Out0/1000)%10) << 1;
   
   Output_High(PIN_C4);
   portb=Dig[3];
   delay_ms(1);
   Output_Low(PIN_C4);
   
   Output_High(PIN_C3);
   portb=Dig[2];
   delay_ms(1);
   Output_Low(PIN_C3);
   
   Output_High(PIN_C2);
   portb=Dig[1];
   delay_ms(1);
   Output_Low(PIN_C2);

   Output_High(PIN_C1);
   portb=Dig[0];
   delay_ms(1);
   Output_Low(PIN_C1);

}
}

