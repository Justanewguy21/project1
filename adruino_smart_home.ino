#include <Servo.h>
#include <Keypad.h>

// ----- Window Servo Setup -----
Servo windowServo;
const int windowServoPin = 12;
int rainSensorPin = A3;
int rainValue;
bool isRaining = false;
unsigned long lastRainTime;

// ----- Door Servo Setup -----
Servo doorServo;
const int doorServoPin = 3;

// ----- Keypad Setup -----
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', 'A', '2', '3'},
    {'4', 'B', '5', '6'},
    {'7', 'C', '8', '9'},
    {'*', 'D', '0', '#'}
};

// Adjust pin numbers based on your wiring
byte rowPins[ROWS] = {4, 5, 6, 7}; 
byte colPins[COLS] = {8, 9, 10, 11}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ----- Password Setup -----
const String password = "1234"; 
String inputPassword = "";

// ----- Timers -----
unsigned long doorUnlockTime = 0;
bool doorIsUnlocked = false;

void setup() {
    Serial.begin(9600);

    // Attach servos
    windowServo.attach(windowServoPin);
    doorServo.attach(doorServoPin);

    // Initial positions
    windowServo.write(0); // Open window initially
    doorServo.write(0);   // Lock door initially

    pinMode(rainSensorPin, INPUT);

    Serial.println("Enter 4-digit Password:");
}

void loop() {
    checkRainSensor(); // Monitor rain sensor & control window
    checkKeypad();     // Monitor keypad & control door
}

// ----- Rain Sensor Function -----
void checkRainSensor() {
    rainValue = map(analogRead(rainSensorPin), 0, 1023, 255, 0);

    if (rainValue > 130 && !isRaining) {  
        // Close window
        moveServoSmooth(windowServo, 180);
        isRaining = true;
        lastRainTime = millis(); // Reset timer
    } 
    else if (rainValue < 130 && isRaining) {
        // Wait 10 sec before reopening window
        if (millis() - lastRainTime >= 10000) {
            moveServoSmooth(windowServo, 0);
            isRaining = false;
        }
    } 
    else {
        lastRainTime = millis(); // Keep resetting if still raining
    }
}

// ----- Keypad & Door Lock Function -----
void checkKeypad() {
    char key = keypad.getKey();

    if (key) {
        Serial.print(key);

        if (key == '#') { // Confirm password
            if (inputPassword.length() == 4) { 
                Serial.println("\nEntered Password: " + inputPassword);
                if (inputPassword == password) {
                    unlockDoor();
                } else {
                    incorrectPassword();
                }
            } else {
                Serial.println("\n❌ Password must be 4 digits!");
            }
            inputPassword = "";  
        } 
        else if (key == '*') { // Clear input
            inputPassword = "";
            Serial.println("\n🔄 Password cleared.");
        } 
        else if (inputPassword.length() < 4) { // Allow only 4 characters
            inputPassword += key;
        }
    }

    // Automatically lock the door after 5 seconds
    if (doorIsUnlocked && millis() - doorUnlockTime >= 5000) {
        lockDoor();
    }
}

void unlockDoor() {
    Serial.println("✅ Correct Password! Unlocking...");
    moveServoSmooth(doorServo, 90);
    doorUnlockTime = millis(); // Start unlock timer
    doorIsUnlocked = true;
}

void lockDoor() {
    Serial.println("🔒 Locking Door...");
    moveServoSmooth(doorServo, 0);
    doorIsUnlocked = false;
    Serial.println("Enter 4-digit Password:");
}

void incorrectPassword() {
    Serial.println("❌ Wrong Password! Try Again.");
}

// ----- Smooth Servo Movement -----
void moveServoSmooth(Servo &servo, int targetAngle) {
    int currentAngle = servo.read();
    int step = (targetAngle > currentAngle) ? 5 : -5;

    for (int pos = currentAngle; pos != targetAngle; pos += step) {
        servo.write(pos);
        delay(15); // Smooth movement
    }
    servo.write(targetAngle);
}
