#include<mbed.h>
Ticker flipper;
DigitalOut led2(LED2);
void flip() {
    led2 = !led2;
}

int main()
{
    led2 = 1;
    flipper.attach(&flip, 0.002); // the address of the function to be attached (flip) and the interval (0.002 seconds)
    // spin in a main loop. flipper will interrupt it to call flip
    while(1) {
	printf("hello world!\n");
	wait_ms(1000);
    }
}
