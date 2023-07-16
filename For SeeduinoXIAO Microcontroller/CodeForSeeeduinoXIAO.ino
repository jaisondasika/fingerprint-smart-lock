/********************************************************************

     --------- FINGERPRINT DOOR UNLOCK + LED LIGHTS ---------

               This script is adapted to run on the
                        Seeeduino Xiao M0 board

        Please watch the full youtube video by Jaison Dasika for
          the wiring diagram and details explaining the setup

 *********************************************************************/

#include <Adafruit_Fingerprint.h>
#include <FastLED.h>
#include <Servo.h>
#include <Wire.h>
#include <TFLI2C.h>

//Define pin locations on the board
#define SERVO_ENCODER_PIN A2
#define HALL_PIN A1
#define BUTTON_PIN 10
#define BUZZER_PIN 9
#define LED_PIN 8
#define SERVO_PIN 3

//Define attributes
#define NUM_LEDS 300
#define BRIGHTNESS 100    //Brigtness of LED strip
#define TIME_DELAY 4000    //how long till the door automatically locks
#define DOOR_CLOSED_HALL_VAL 789    //Reading for the hall sensor when the door closes
#define UNLOCKED_SERVO_VAL 0   //Servo value to unlock the door
#define LOCKED_SERVO_VALL 90   //Servo value to lock the door
#define DEADBOLT_JAMMED_VAL 300 //
#define DEADBOLT_OPENED_VAL 320

//Define variables with the least amount of memory needed for their respective tasks
unsigned long previousMillis, currentMillis = 0;
bool doorIsLocked = true, deadboltIsJammed = false, ledRingBreathing = false;
float hue = 1, hue2 = 1;
int16_t bulbs2light, tfDist, tfAddr = TFL_DEF_ADR;  //Lidar address


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1
#endif

//Initialize objects
CRGB leds[NUM_LEDS];
Servo servo;
TFLI2C tflI2C; //TFmini LiDAR sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//Runs only once on startup
void setup() {
  delay(300);
  LEDS.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
  delay(10);
  Wire.begin();           // Initalize Wire library
  tone(BUZZER_PIN,  444);
  delay(80);
  noTone(BUZZER_PIN);
  delay(10);
  Serial.println(F("\n\nAdafruit finger detect test"));
  // set the data rate for the sensor serial port
  //finger.begin(57600); //alternate baud rate
  finger.begin(58824);
  delay(10);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print(F("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example."));
  }
  else {
    Serial.println(F("Waiting for valid finger..."));
    Serial.print(F("Sensor contains ")); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

//This function loops repeatidly
void loop() {
  delay(5);
  lidarLight();   //Makes the lights follow your position to the door
  getFingerprintID();   //Poll for fingerprint
  burglerMode();
  currentMillis = millis();
  Serial.print(analogRead(HALL_PIN));
  Serial.print("  ");

  Serial.println(analogRead(SERVO_ENCODER_PIN));

  //When button is pressed, unlock the door
  if (digitalRead(BUTTON_PIN) == 1) {
    delay(250);
    unlockDoor();
    holdDoorUnlocked();
  }

  //When door is closed, lock the door
  if (doorIsLocked == false && analogRead(HALL_PIN) >= DOOR_CLOSED_HALL_VAL
      && currentMillis - previousMillis >= TIME_DELAY ) {
    previousMillis = currentMillis;
    delay(100);
    lockDoor();
  }
}

//Unlocks the door
void unlockDoor() {
  doorIsLocked = false;
  previousMillis = currentMillis;
  servo.attach(SERVO_PIN);
  servo.write(UNLOCKED_SERVO_VAL);
  uint8_t buzzerDelay = 90;
  tone(BUZZER_PIN,  698.46);
  delay(buzzerDelay);
  tone(BUZZER_PIN, 880.00);
  delay(buzzerDelay);
  tone(BUZZER_PIN, 1046.50);
  delay(buzzerDelay);
  tone(BUZZER_PIN, 1396.91);
  delay(buzzerDelay);
  noTone(BUZZER_PIN);
  delay(200);
  servo.detach();
}

//Locks the door
void lockDoor() {
  doorIsLocked = true;
  servo.attach(SERVO_PIN);
  servo.write(LOCKED_SERVO_VALL);
  finger.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_BLUE, 10);
  uint8_t buzzerDelay = 110;
  tone(BUZZER_PIN, 1396.91);
  delay(buzzerDelay);
  tone(BUZZER_PIN, 1046.50);
  delay(buzzerDelay);
  tone(BUZZER_PIN, 880.00);
  delay(buzzerDelay);
  tone(BUZZER_PIN,  698.46);
  noTone(BUZZER_PIN);
  delay(100);
  servo.detach();

  //test to see if deadbolt got jammed
  if (analogRead(SERVO_ENCODER_PIN) < DEADBOLT_JAMMED_VAL) {
    deadboltJammed();
  }
  else {
    deadboltIsJammed = false;
  }
}

void deadboltJammed() {
  deadboltIsJammed = true;
  servo.attach(SERVO_PIN);
  servo.write(UNLOCKED_SERVO_VAL);
  alert(12);
  servo.detach();

}

//Alarms and fights back if deadbolt has turned unauthorized
void burglerMode() {
  if (analogRead(SERVO_ENCODER_PIN) < DEADBOLT_OPENED_VAL && doorIsLocked == true && deadboltIsJammed == false) {
    alert(20);
    lockDoor(); //Servo fights back against lockpicking attempts
    Serial.print(F("Burgler mode activated!!"));
  }
}

//makes a fast beeping noise
void alert(uint8_t numBeeps) {
  for (uint8_t i = 0; i < numBeeps; i++) {
    tone(BUZZER_PIN, 329.625);
    delay(50);
    noTone(BUZZER_PIN);
    delay(30);
  }
}

//Fingerprint is not in the database
void noFingerMatch() {
  tone(BUZZER_PIN, 400);
  fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show();
  finger.LEDcontrol(FINGERPRINT_LED_FLASHING, 4, FINGERPRINT_LED_RED, 16 );
  delay(80);
  noTone(BUZZER_PIN);
  fill_solid( leds, NUM_LEDS, CRGB::Black); FastLED.show();
  //finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 100, FINGERPRINT_LED_RED);
}

//Read the fingerprint (credit to Adafruit)
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(F("No finger detected"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Found a print match!"));
    finger.LEDcontrol(FINGERPRINT_LED_BLUE, 8, FINGERPRINT_LED_OFF, 12 );
    unlockDoor();
    fill_solid( leds, NUM_LEDS, CRGB::Green); FastLED.show(); delay(50);
    theaterChaseRainbow(7);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(F("Did not find a match"));
    noFingerMatch();
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }

  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID);
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);
  return finger.fingerID;
}

//Returns -1 if failed, otherwise returns ID # (Adafruit)
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID);
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);
  return finger.fingerID;
}

//Make the lights react to the LiDAR distance
void lidarLight() {
  if (tflI2C.getData(tfDist, tfAddr)) {
    if (ledRingBreathing == false) {
      ledRingBreathing = true;
      finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 600, FINGERPRINT_LED_PURPLE);
    }
    Serial.println(tfDist);
    delay(1);
    FastLED.clear();
    int bulbs2light = map(tfDist, 10, 600, 150, 0);

    if (bulbs2light < 0) bulbs2light = 0;
    if (bulbs2light > 147) {
      hue += 1.2;
      hue2 = hue;
    }

    for (int i = 0; i <= bulbs2light; i++) {
      leds[i] = CHSV (hue2 + i * 0.7, 150, 255);
      leds[NUM_LEDS - i] = CHSV (hue2 + i * 0.7, 150,  255);
    }
    FastLED.show();
  }
  else {
    //if nothing in front of LiDAR or sensor malfunctions, clear LEDs
    FastLED.clear(); FastLED.show();
    if (ledRingBreathing == true) {
      ledRingBreathing = false;
      finger.LEDcontrol(false);
      finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_OFF);
    }
  }
}

//Helper method for theaterChaseRainbow()
byte * Wheel(byte WheelPos) {
  static byte c[3];
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }
  return c;
}

//Rainbow effect when correct finger is read
void theaterChaseRainbow(int SpeedDelay) {
  byte *c;
  for (int j = 0; j < 156; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 7; q++) {
      for (int i = 0; i < NUM_LEDS; i = i + 7) {
        c = Wheel( (i + j) % 255);
        leds[i + q] = CRGB(*c, *(c + 1), *(c + 2));
      }
      FastLED.show();
      delay(SpeedDelay);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i + q] = CRGB(0, 0, 0);
      }
      //Stop the animation when the door opens
      if (analogRead(HALL_PIN) < DOOR_CLOSED_HALL_VAL)  {
        break;
      }
    }
  }
}

//When the button is held down, the door will stay unlocked until the button is pressed
void holdDoorUnlocked() {
  if (digitalRead(BUTTON_PIN) == 1) {
    delay(140);
    int buzzerDelay = 220;
    tone(BUZZER_PIN, 349.23);
    delay(buzzerDelay);
    tone(BUZZER_PIN, 523.25);
    delay(buzzerDelay);
    tone(BUZZER_PIN, 587.23);
    delay(buzzerDelay);
    tone(BUZZER_PIN, 659.25);
    delay(buzzerDelay);
    tone(BUZZER_PIN, 698.46 );
    delay(buzzerDelay);
    noTone(BUZZER_PIN);
    FastLED.clear(); FastLED.show();
    while (true) {
      delay(1);
      lidarLight();
      if (digitalRead(BUTTON_PIN) == 1) {
        break;
        delay(30);
      }
    }
  }
}
