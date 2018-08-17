
#include "mbed.h"
DigitalOut myled(LED2); //板子上指示灯灯闪烁
DigitalOut IO1(PB_3);
DigitalOut IO2(PB_5);
DigitalOut IO3(PB_10);
PwmOut LD1(PA_11);
PwmOut LD2(PA_0);
PwmOut LD3(PC_6);
PwmOut LD4(PB_6);
PwmOut LD5(PB_8);
PwmOut LD6(PB_9);
//The default period is 0.020s, and the default pulsewidth is 0.
//PwmOut test1(PA_10);
//PwmOut test2(PA_9);
void LD_Setting(int freq1,int freq2,int freq3,int freq4,int  freq5)
{
   LD1.period(1.0 / freq1);
   LD1.write(0.50);
   LD2.period(1.0 / freq2);
   LD2.write(0.01);
   LD3.period(1.0 / freq3);
   LD3.write(0.3);
   LD4.period(1.0 / freq4);
   LD4.write(0.3);
   LD5.period(1.0 / freq5);
   LD5.write(0.3);
   LD6.period(1.0 / freq5);
   LD6.write(0.3);
  // test1.period(1.0/1200);
  // test1.write(0.5);
  // test2.period(1.0/1500);
  // test2.write(0.5);
}
int main() {
    IO1=1;
    IO2=1;
    IO3=0;
    LD_Setting(500,600,1100,1200,1300);
    while (1) {
    myled=!myled;
    wait_ms(1000);
    }
}

