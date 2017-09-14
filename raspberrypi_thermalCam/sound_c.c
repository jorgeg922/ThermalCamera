#include <stdio.h>
#include <wiringPi.h>
#define SOUND 16
int main(void)
{
wiringPiSetupGpio();
pinMode(SOUND, INPUT);

while(1)
{
if(digitalRead(SOUND)==LOW)
{
printf("Sound Detected! Please Silent");
delay(1000);
printf("\n");
}
else
{
printf("No Sound");
delay(1000);
printf("\n");
}
}
return 0;
}
