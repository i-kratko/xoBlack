int Sen = 36;
int led = 22;
void setup() {
  pinMode(Sen, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  Serial.print(0);
  Serial.print(" ");
  Serial.print(analogRead(Sen));
  Serial.print(" ");
  Serial.println(4096);
  delay(100);
  if(analogRead(Sen) >= 900)
  {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }

}