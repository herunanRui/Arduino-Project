#define TRIG_PIN 2 // Define the pin for the trigger
#define ECHO_PIN 3 // Define the pin for the echo

void setup() {
  pinMode(TRIG_PIN, OUTPUT); // Set the trigger pin as an output
  pinMode(ECHO_PIN, INPUT);  // Set the echo pin as an input
  Serial.begin(9600);        // Initialize serial communication
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);  // Set the trigger pin to low
  delayMicroseconds(2);         // Wait for 2 microseconds
  digitalWrite(TRIG_PIN, HIGH); // Set the trigger pin to high
  delayMicroseconds(10);        // Wait for 10 microseconds
  digitalWrite(TRIG_PIN, LOW);  // Set the trigger pin to low

  long duration = pulseIn(ECHO_PIN, HIGH); // Measure the duration of the echo pulse
  float distance_cm = duration / 58.0;     // Calculate the distance in centimeters
  float distance_in = distance_cm / 2.54;  // Convert the distance to inches

  Serial.print("Distance in inches: ");
  Serial.println(distance_in);   // Print the distance in inches to the serial monitor

  delay(1000); // Wait for 1 second before taking another measurement
}
