#include <Arduino.h>
#include <map>
#include <string>

#define TOUCH_PIN T4

const unsigned long DOT_DURATION = 100;
const unsigned long DASH_THRESHOLD = DOT_DURATION * 3;
const unsigned long INTER_CHAR_SPACE_THRESHOLD = DOT_DURATION * 7;
const unsigned long WORD_SPACE_THRESHOLD = DOT_DURATION * 10;

const int TOUCH_THRESHOLD = 20;

unsigned long touchStartTime = 0;
unsigned long lastTapTime = 0;
String currentMorseCode = "";
String decodedText = "";

std::map<String, char> morseCodeMap;

void setupMorseMap() {
    morseCodeMap[".-"] = 'A';
    morseCodeMap["-..."] = 'B';
    morseCodeMap["-.-."] = 'C';
    morseCodeMap["-.."] = 'D';
    morseCodeMap["."] = 'E';
    morseCodeMap["..-."] = 'F';
    morseCodeMap["--."] = 'G';
    morseCodeMap["...."] = 'H';
    morseCodeMap[".."] = 'I';
    morseCodeMap[".---"] = 'J';
    morseCodeMap["-.-"] = 'K';
    morseCodeMap[".-.."] = 'L';
    morseCodeMap["--"] = 'M';
    morseCodeMap["-."] = 'N';
    morseCodeMap["---"] = 'O';
    morseCodeMap[".--."] = 'P';
    morseCodeMap["--.-"] = 'Q';
    morseCodeMap[".-."] = 'R';
    morseCodeMap["..."] = 'S';
    morseCodeMap["-"] = 'T';
    morseCodeMap["..-"] = 'U';
    morseCodeMap["...-"] = 'V';
    morseCodeMap[".--"] = 'W';
    morseCodeMap["-..-"] = 'X';
    morseCodeMap["-.--"] = 'Y';
    morseCodeMap["--.."] = 'Z';
    morseCodeMap[".----"] = '1';
    morseCodeMap["..---"] = '2';
    morseCodeMap["...--"] = '3';
    morseCodeMap["....-"] = '4';
    morseCodeMap["....."] = '5';
    morseCodeMap["-...."] = '6';
    morseCodeMap["--..."] = '7';
    morseCodeMap["---.."] = '8';
    morseCodeMap["----."] = '9';
    morseCodeMap["-----"] = '0';
    morseCodeMap["..-.."] = '\n';
    morseCodeMap["--.--"] = '\b';
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\nMorse Code Input Device");

    Serial.println("Tap the wire to send Morse code.");
    setupMorseMap();
}

void loop() {
    int touchValue = touchRead(TOUCH_PIN);

    if (touchValue < TOUCH_THRESHOLD) {
        if (touchStartTime == 0) {
            touchStartTime = millis();
        }
    } else {
        if (touchStartTime != 0) {
            unsigned long touchDuration = millis() - touchStartTime;

            if (touchDuration < DASH_THRESHOLD) {
                currentMorseCode += ".";
                Serial.print(".");
            } else {
                currentMorseCode += "-";
                Serial.print("-");
            }
            
            touchStartTime = 0;
            lastTapTime = millis();
        }

        unsigned long timeSinceLastTap = millis() - lastTapTime;

        if (currentMorseCode.length() > 0 && timeSinceLastTap > INTER_CHAR_SPACE_THRESHOLD) {
            if (morseCodeMap.count(currentMorseCode)) {
                char decodedChar = morseCodeMap[currentMorseCode];
                decodedText += decodedChar;
                Serial.print(decodedChar);
                Serial.print(" ");
            } else {
                Serial.print(" -> ? ");
            }
            currentMorseCode = "";
            lastTapTime = millis();
        }

        if (decodedText.length() > 0 && timeSinceLastTap > WORD_SPACE_THRESHOLD) {
            decodedText += " ";
            lastTapTime = millis();
        }
    }
}
