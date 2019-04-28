float duration, cm;

void setup() {
  Serial.begin(9600);
  pinMode(D0, OUTPUT);  //Tri
  pinMode(D1, INPUT);   //Echo

}

void loop() {
  digitalWrite(D0, LOW);
  delayMicroseconds(5);
  digitalWrite(D0, HIGH);
  delayMicroseconds(10);
  digitalWrite(D0, LOW);
  
  duration = pulseIn(D1, HIGH);
  cm = (duration/2)/29.1;

  Serial.println(cm);
  Serial.println("cm");
  Serial.println();

  delay(1000);

}
