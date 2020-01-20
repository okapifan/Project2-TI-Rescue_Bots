#define echoPin D4
#define trigPin D6

#define trigPin2 D7
#define trigPin3 D8

long duration;
long distance;

void setup() 
{

  Serial.begin(9600);
  pinMode (echoPin, INPUT);
  pinMode (trigPin, OUTPUT);
  pinMode (trigPin2, OUTPUT);
  pinMode (trigPin3, OUTPUT);
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
 digitalWrite(trigPin3, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin3, HIGH);
 delayMicroseconds(10);

 digitalWrite(trigPin3,LOW);
 duration = pulseIn (echoPin, HIGH);

 //This gives us distance in cm

 distance = duration/58.2;
 Serial.print("Dit is de derde meting: ");
 Serial.println (distance);



 //Milliseconds

 delay(100);

}
