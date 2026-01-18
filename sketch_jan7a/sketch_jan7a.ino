//Slave side code

#define SIGNAL_PIN 2     // Signal from master (flame)
#define BUZZER_PIN 3     // Buzzer
#define GAS_PIN A0       // MQ-2 analog output

int gasThreshold = 450;  

void setup() {
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
  Serial.println("Slave ready: Flame + Gas detection");
}

void loop() {
  int flameSignal = digitalRead(SIGNAL_PIN);
  int gasValue = analogRead(GAS_PIN);

  bool gasDetected = (gasValue > gasThreshold);

  if (flameSignal == HIGH || gasDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.print("ALARM ON | Gas Value: ");
    Serial.println(gasValue);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.print("Safe | Gas Value: ");
    Serial.println(gasValue);
  }

  delay(200);
}




