#include <LiquidCrystal_I2C.h> // Library for I2C LCD 
#include <Wire.h>  //Library that needed to read the wire connected on LCD
#include <Servo.h>  //Libary to run the servo motors

//this variable will be use to call the first servo
Servo servo1;
//this variable will be use to call the second servo
Servo servo2;

// Define the pins for the IR sensor for object detection
int irSensorPin = 2;
// Define the pins for the IR sensor for second value
int irSensorPin2 = 3;
// Define the pins for the IR sensor for third value
int irSensorPin3 = 4;

// Define a variable to count the number of detections
int detectionCount = 0;
// Set the value of every plastic inserted
float plasticValue = 0.0;

// Define the pins for capacitive proximity sensor
int inductiveSensorPin = 7;
//for opening the container
int servo1Pin = 6; 
//for opening the return path
int servo2Pin = 5; 

int butClaim = 8;


#define I2C_ADDR 0x27
// Set the number of columns and rows on the LCD
#define LCD_COLS 16
#define LCD_ROWS 2


// Initialize the LCD object with the I2C address and the number of columns and rows
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

int totalColumns = 16;
int totalRows = 2;

String staticMessage = "     HELLO";
String scrollingMessage = "Welcome to Plastic Bottle Reverse Vending Machine";

void scrollMessage(int row, String message, int delayTime, int totalColumns) {
  for (int i=0; i < totalColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int position = 0; position < message.length(); position++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(position, position + totalColumns));
    delay(400);
  }
}


void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  // Set the IR sensor pin as input
  pinMode(irSensorPin, INPUT);
  // Set the IR sensor pin as input
  pinMode(irSensorPin2, INPUT);
  // Set the IR sensor pin as input
  pinMode(irSensorPin3, INPUT);
  // Set the LED pin as output
  pinMode(LED_BUILTIN, OUTPUT);
  // Set the Capcitive pin as input component
  pinMode(inductiveSensorPin, INPUT_PULLUP);

  servo1.attach(servo1Pin); // Attach servo1 to pin

  servo2.attach(servo2Pin); // Attach servo2 to pin

  pinMode(butClaim, INPUT_PULLUP);
  
  // Initialize the I2C interface
  Wire.begin();
  // Set up the LCD display
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
  // Print a welcome message to the LCD
  lcd.setCursor(0, 0);
  lcd.print(staticMessage);
  scrollMessage(1, scrollingMessage, 250, totalColumns);
}


void loop() {

  // Read the IR and inductive sensor sensor value
  int irSensorValue = digitalRead(irSensorPin);
  int irSensorValue2 = digitalRead(irSensorPin2); 
  int irSensorValue3 = digitalRead(irSensorPin3); 
  int inductiveValue = digitalRead(inductiveSensorPin);
  
  int butClaimVal = digitalRead(butClaim);
  // If the IR sensor detects an object
  if (irSensorValue == LOW) {
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Initializing");
        delay(1500);

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Please Wait");
        lcd.setCursor(3, 1);
        lcd.print("for awhile");
        delay(1500);

        if (inductiveValue == LOW) {
        // Increment the detection count
        detectionCount++;

            if (irSensorValue == LOW && irSensorValue2 == HIGH && irSensorValue3 == HIGH) {
              plasticValue+=0.12;
            }
            else if (irSensorValue == LOW && irSensorValue2 == LOW && irSensorValue3 == HIGH) {
              plasticValue+=0.20;
            }
            else if (irSensorValue == LOW && irSensorValue2 == LOW && irSensorValue3 == LOW) {
              plasticValue+=0.35;
            }
            else{
            }
          
        // move the servo to open the container
        delay(1500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Verified");
        delay(500);

        servo1.write(120);
        delay(1500);

        digitalWrite(LED_BUILTIN, HIGH);

        // Print a message to the serial monitor
        Serial.print("Object Detected! Detection Count: ");
        Serial.println(detectionCount);
        Serial.print("Total Value: ");
        Serial.println(plasticValue);


        // display the records of plastic inserted and the value
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Inserted: ");
        lcd.setCursor(12, 0);
        lcd.print(detectionCount);

        lcd.setCursor(0, 1);
        lcd.print("Total Point: ");
        lcd.setCursor(12, 1);
        lcd.print(plasticValue);
        delay(500); 

        servo1.write(0);
        delay(1500);
        }


        else if (inductiveValue == HIGH) {
        // move the servo motors
        delay(1000);
        servo2.write(120);
        delay(2000);
        servo1.write(120);

        digitalWrite(LED_BUILTIN, HIGH);
        // Print a message to the serial monitor
        Serial.print("Object not recognized as Plastic!");

        // display an invalid message on lCD
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Error!");
        lcd.setCursor(5, 1);
        lcd.print("Invalid");
        delay(1500);

        servo2.write(0);
        delay(500);
        servo1.write(0);

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Please Try");
        lcd.setCursor(5, 1);
        lcd.print("Again");

        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please insert");
        lcd.setCursor(0, 1);
        lcd.print("Plastic Bottle");
        delay(500);
        }
        else {

        }
   
  }
  else {
      if ( butClaimVal == LOW) {
            delay(500);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Are you sure you");
            lcd.setCursor(0, 1);
            lcd.print("want to Donate?");
            delay(1000);

              if (butClaimVal == HIGH) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Thank You");
              lcd.setCursor(0, 1);
              lcd.print("Very MUCH!!");
              delay(1000);
              plasticValue = 0;
              detectionCount = 0;
              return;
              }

              else if (butClaimVal == LOW) {
              delay(500);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Thank You");
              lcd.setCursor(0, 1);
              lcd.print("Very MUCH!!");
              delay(1000);
              lcd.clear();
              plasticValue = 0;
              detectionCount = 0;
              return;
              }

          }
          else {
            
          }
    // wala munang gagawin and system
    delay(3000);
    servo2.write(0);
    servo1.write(0);
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  // Wait for a short period of time before checking the sensor again
  delay(500);
}