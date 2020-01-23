const int ReedPin = A0;

void setup() 
{         
  Serial.begin(115200);
  pinMode(ReedPin, INPUT);
}

void loop() 
{
  int inputValA = analogRead(ReedPin);
  bool inputValD = digitalRead(ReedPin);
  bool inputValue = (inputValA > 512);
  Serial.print(inputValA);
  Serial.print(", ");
  Serial.println(inputValD);
  Serial.print(", ");
  Serial.println(inputValue);
  delay(200);
}
