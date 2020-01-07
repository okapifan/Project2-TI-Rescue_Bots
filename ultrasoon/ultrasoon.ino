#define echoPin 9
#define trigPin 8

#define trigPin2 10

long duration;
long distance;

void setup() 
{

  Serial.begin(9600);
  pinMode (echoPin, INPUT);
  pinMode (trigPin, OUTPUT);
  pinMode (trigPin2, OUTPUT);
}


void loop() 
{
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);

 digitalWrite(trigPin,LOW);
 duration = pulseIn (echoPin, HIGH);

 //This gives us distance in cm

 distance = duration/58.2;
 Serial.print ("Dit is de eerste meting: ");
 Serial.println (distance);



 //Milliseconds

 delay(100);

digitalWrite(trigPin2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10);

 digitalWrite(trigPin2,LOW);
 duration = pulseIn (echoPin, HIGH);

 //This gives us distance in cm

 distance = duration/58.2;
 Serial.print("Dit is de tweede meting: ");
 Serial.println (distance);



 //Milliseconds

 delay(100);

}
