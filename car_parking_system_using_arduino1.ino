#include <Servo.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define t1 10
#define t2 9
#define t3 8

#define t4 7
#define t5 13

Servo myservo;

int distanceThreshold = 100;

int parkingAvailable = 3;
int barrierState = 0;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  Serial.begin (9600);
  myservo.attach(6);
  myservo.write(0);
}

long readDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}


void loop()
{
  float d1 = 0.01723 * readDistance(t1, t1);
  float d2 = 0.01723 * readDistance(t2, t2);
  float d3 = 0.01723 * readDistance(t3, t3);
  float d4 = 0.01723 * readDistance(t4, t4);
  float d5 = 0.01723 * readDistance(t5, t5);
  //get all sensor readings
  
  Serial.println("d1 = " + String(d1) + "cm");
  Serial.println("d2 = " + String(d2) + "cm");
  Serial.println("d3 = " + String(d3) + "cm");
  Serial.println("d4 = " + String(d4) + "cm");
  Serial.println("d5 = " + String(d5) + "cm");
  //display all sensor distance on serial monitor
  
/* For below command
barierState is used to ensure that the barrier either is closed or open for enter or exit

So while barrierState is:
0	barrier is closed
-1	barrier is open for enter
1	barrier is open for exit
-2	barrier is closed after the vehicle passed the barrier gate (enter)
2	barrier is closed after the vehicle passed the barrier gate (exit)

after it reached to -2 or 2 value, it will reset back to 0 when there is no vehicle detected
by two sensor.

For parkingAvailable, we set to 3 as we have 3 parking lot only
so it will automatically update while the barrier is open.
Therefore, the barrier will not be open while there is no more parking lot available.
*/
  
if (barrierState == 0)
{
  if (d4<100 && d5>=100 && parkingAvailable>0)
  {
     parkingAvailable -= 1;
     barrierState = -1;
     myservo.write(90);
  }
  if (d4>=100 && d5<100 && parkingAvailable<3)
  {
     parkingAvailable += 1;
     barrierState = 1;
     myservo.write(90);
  }
}
else if (barrierState == -1)
{
  if (d4>=100 && d5<100)
  {
     barrierState = -2;
     myservo.write(0);
  }
}  
 else if (barrierState == 1)
{
    if (d5>=100 && d4<100)
  {
     barrierState = 2;
     myservo.write(0);
  }
}
 else if (barrierState == -2)
{
    if (d5>=100)
  {
     barrierState = 0;
  }
}  
else if (barrierState == 2)
{
    if (d4>=100)
  {
     barrierState = 0;
  }
}  

/*
The command below is used to print out the information on the LCD Screen
For the first row of the LCD Screen, it will show the number of parking lot available
While it full, it will show Parking Full

For the second row of the LCD Screen, it will show which parking lot is still empty
This allow users to know which parking lot is still available without wasting time
to find an empty parking lot.
*/  
lcd.setCursor(0,0);
if (parkingAvailable == 0)
{
  lcd.print("Parking Full  ");
}
else
{
  lcd.print("Parking left ");
  lcd.print(parkingAvailable);
}

  
  
if (d1>100 & d2>100 & d3>100)
{
	lcd.setCursor(0,1);
	lcd.print("Slot 1 2 3 Free");
	delay(500);
	}
else if((d1>100 & d2>100)|(d2>100 & d3>100)|(d3>100 & d1>100))
  {
	lcd.setCursor(0,1);
  	if(d1>100 & d2>100)
		lcd.print("Slot 1 & 2 Free");
  	else if(d1>100 & d3>100)
		lcd.print("Slot 1 & 3 Free");
  	else
		lcd.print("Slot 2 & 3 Free");
	}
else if(d1<100 & d2<100 & d3<100)
  {
	lcd.setCursor(0,1);
	lcd.print("Parking Full   ");
	}
else if((d1<100 & d2<100)|(d2<100 & d3<100)|(d3<100 & d1<100))
  {
	lcd.setCursor(0,1);
  	if(d1>100)
      lcd.print("Slot 1 is Free ");
	else if (d2>100)
       lcd.print("Slot 2 is Free ");
    else
       lcd.print("Slot 3 is Free ");
   }
  delay(100);
}