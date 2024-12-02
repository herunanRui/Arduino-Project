#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>

//0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD Object

// Creation of the Real Time Clock Object
virtuabotixRTC myRTC(2, 3, 4);

const int relay1 = 5;
const int relay2 = 6;

SoftwareSerial sim800l(7, 8); //SIM800L Tx & Rx is connected to Arduino #7 & #8

const int TRIGGER_PIN = 9;    // Trig pin of the ultrasonic sensor
const int ECHO_PIN = 10;   // Echo pin of the ultrasonic sensor
const int buzzerPin = 11;  // Digital pin connected to the buzzer

// Flag to track whether SMS has been sent
bool smsSent = false;


void setup()  {
  Serial.begin(9600);
  sim800l.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(0, 35, 12, 3, 5, 12, 2023);

  pinMode(TRIGGER_PIN, OUTPUT);  // Set the trigger pin as an output
  pinMode(ECHO_PIN, INPUT);      // Set the echo pin as an input

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}

void loop()  {
  // Measure distance using the ultrasonic sensor
  long duration;    // Variable to store the duration of the echo pulse
  float distance;   // Variable to store the calculated distance

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the duration of the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance based on the speed of sound (343 meters/second)
  // Divide the duration by 2, since the sound travels forth and back
  distance = duration * 0.0343 / 2;

  // Check if the distance is below 70 units
  if (distance < 12) {

    //add delay for better and accurate information
    delay(5000); 

      if (distance < 12) {
        
      // the buzzer triggers 8x
      digitalWrite(buzzerPin, HIGH);//1
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//2
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//3
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//4
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//5
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//6
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//7
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      delay(300);
      digitalWrite(buzzerPin, HIGH);//8
      delay(300); // Sound the buzzer for 1 second
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
      

      Serial.println("Distance below 12 units. Sending SMS...");

      Serial.println("Initializing..."); 
        delay(1000);

        sim800l.println("AT"); //Once the handshake test is successful, it will back to OK
        updateSerial();

        sim800l.println("AT+CMGF=1"); // Configuring TEXT mode
        updateSerial();
        sim800l.println("AT+CMGS=\"+639352287063\"");//Ilene's phone number as recipient
        updateSerial();
        sim800l.print("PBRVM's storage is Full and ready to collect"); //text content
        updateSerial();
        sim800l.write(26);

        delay(300000); //it will sends another sms after 5 minutes
      }
      else {

      }
      
  }
  else {

  }

  //call time function 
  time();

  //call relay function
  Relay();

  //call sms function
  buzzer();

}


void time() {
  // This allows for the update of variables for time or accessing the individual elements.
  myRTC.updateTime();

  // Start printing elements as individuals
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);

  // Delay so the program doesn't print non-stop

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  lcd.print("/");
  lcd.print(myRTC.year);
  

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  lcd.print(myRTC.seconds);
  
  delay(300);
}


void Relay() {

  int hours = myRTC.hours;


  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  // Arduino 2 switch

  delay(300);

}

void buzzer() {
  long duration;    // Variable to store the duration of the echo pulse
  float distance;   // Variable to store the calculated distance

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the duration of the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance based on the speed of sound (343 meters/second)
  // Divide the duration by 2, since the sound travels forth and back
  distance = duration * 0.0343 / 2;

}


//checking SIM800l if it is reponsive
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    sim800l.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800l.available()) 
  {
    Serial.write(sim800l.read());//Forward what Software Serial received to Serial Port
  }
}