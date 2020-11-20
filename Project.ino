#include <SoftwareSerial.h>
SoftwareSerial GSM(P1_6, P1_7); // RX, TX
float latitude=0;
float logitude=0;
#define led P1_0
const int vibrationSensor=PUSH2;
int i=0;
//Vibration sensor signal functionality can be described in the given function (which will filter
//small/false vibrations as well).
#define count_max 25
char SensorRead(int pin) // read sw with debounce
{
char count_low=0,count_high = 0;
do
{
delay(1);
if (digitalRead(pin) == HIGH)
{
count_high++;
count_low = 0;
}
else
{
count_high = 0;
count_low++;
}
}
while(count_low< count_max && count_high < count_max);
if (count_low >= count_max)
return LOW;
else
return HIGH;
}
//The function below detects the vibration sensors’s HIGH output and calls gpsEvent() function
//to get GPS coordinate and calls the Send() function to send SMS.
void loop()
{
if(SensorRead(vibrationSensor) == HIGH)
{
digitalWrite(led, HIGH);
gpsEvent();
Send();
digitalWrite(led, LOW);
delay(2000);
}
}
//Given Function isresponsible for getting GPS stringsfrom GPS module, extract the coordinates
//from them and convert them in degree-decimal format.
void gpsEvent()
{
char gpsString[55];
char test[]="RMC";
i=0;
while(1)
{
while (Serial.available()) //Serial incoming data
//from GPS
{
char inChar = (char)Serial.read();
gpsString[i]=inChar; //store incoming data
//from GPS to temporary
//string str[]
i++;
if (i < 4)
{
if(gpsString[i-1] != test[i-1]) //check for right string
i=0;
}
int degree=0;
degree=gpsString[16]-48;
degree*=10;
degree+=gpsString[17]-48;
int minut_int=0;
minut_int=gpsString[18]-48;
minut_int*=10;
minut_int+=gpsString[19]-48;
int minut_dec=0;
minut_dec+= (gpsString[21]-48)*10000;
minut_dec+= (gpsString[22]-48)*1000;
minut_dec+= (gpsString[23]-48)*100;
minut_dec+= (gpsString[24]-48)*10;
minut_dec+= (gpsString[25]-48);
float minut= ((float)minut_int +
((float)minut_dec/100000.0))/60.0;
latitude= ((float)degree + minut);
//Send() to send the SMS to the user number that is inserted
void Send()
{
GSM.print("AT+CMGS=");
GSM.print(’"’);
GSM.print("xxxxxxxxxx"); // Mobile number
GSM.println(’"’);
delay(500);
// GSM.print("Latitude:");
// GSM.println(latitude);
GSM.println("Accident Happened");
delay(500);
// GSM.print(" longitude:");
// GSM.println(longitude);
GSM.println("Click On link to see Location");
GSM.print("http://maps.google.com/maps?&z=15&mrt=yp&t=
k&q=");
GSM.print(latitude,6);
GSM.print("+");
GSM.print(longitude,6);
GSM.write(26);
delay(4000);
}