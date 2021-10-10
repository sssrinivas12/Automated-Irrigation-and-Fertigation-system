#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,10);
int SensorPin = A5;
int tempPin = A1;
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(10);
  pinMode(2,INPUT);
}

void loop()
{
  if (Serial.available()>0)
   switch(Serial.read())
   {
    case 's':

       SendMessage();
       break;

    case 'r':

        RecieveMessage();
        break;
   }

  if (mySerial.available()>0)

     Serial.write(mySerial.read());
}

void SendMessage()
{
 int SensorValue = analogRead(SensorPin);   
 float SensorVolts = analogRead(SensorPin)*0.0048828125; 
 int dc = 100;
 mySerial.println("AT+CMGF=1");
 delay(10);
 mySerial.println("AT+CMGS = \"+916302938539\"\r");
 delay(10);
 mySerial.println("Soil moisture reading(in volts) = ");
 delay(10);
 mySerial.println(SensorVolts);
 delay(10);
 if(SensorVolts<1.5)
 {
  mySerial.println("Status : Watering required");
  delay(10);
 }
 if(SensorVolts>1.5)
 {
  mySerial.println("Status : Watering not required");
  delay(10);
 }

 int rain = digitalRead(2);

  if(rain==1)
  {
    mySerial.println("Status : Its raining, Fields should be covered");
    delay(10);
    analogWrite (9,dc);
    analogWrite (11,0);
    delay(10);
    mySerial.println("Status : dc motor switched on");
    delay(10);
  }
  else if(rain==0)
  {
    mySerial.println("Status : No rain, Fields shouldn't be covered");
    delay(10);
    mySerial.println("Status : dc motor remained off");
    delay(10);
  }
 
 int val;
 val = analogRead(tempPin);
  float mv = (val/1024)*5000;
  float cel = mv/10;
  float f = (cel*9)/5 +32;
 mySerial.println("Temperature(in Degree Celcius) = ");
 delay(10);
 mySerial.println(cel);
 delay(10);
 mySerial.println("Temperature(in Degree Fahrenheit) = ");
 delay(10);
 mySerial.println(f);
 delay(10);
 if(cel<38)
 {
  mySerial.println("Status : Sprinkling not required");
  delay(10);
 }
 if(cel>38)
 {
  mySerial.println("Status : Sprinkling required");
  delay(10);
 }

 mySerial.println((char)26);
 delay(1000);


}

void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}
