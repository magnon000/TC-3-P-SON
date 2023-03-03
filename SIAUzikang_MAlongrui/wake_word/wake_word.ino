#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <iostream>
#include <string>
#include <DHT.h> 
#include <LiquidCrystal_I2C.h>

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
#define DHTPIN 22 
#define DHTTYPE DHT11
//---------------------------------------------------------------------------------------
AudioInputI2S            i2s1;           
AudioFilterStateVariable filter1;       
AudioAmplifier           amp1;           
AudioAnalyzeNoteFrequency notefreq;
AudioControlSGTL5000     audioShield;      
AudioPlaySdWav           playWav1;
AudioOutputI2S           audioOutput; 
//---------------------------------------------------------------------------------------
AudioConnection          patchCord1(i2s1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 2, amp1, 0);
AudioConnection          patchCord3(amp1, notefreq);
AudioConnection          patchCord4(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord5(playWav1, 1, audioOutput, 1);
//---------------------------------------------------------------------------------------
DHT dht(DHTPIN, DHTTYPE);
//---------------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x3F,16,2);  //config LCD address, length and line

int redPin = 1;
int greenPin = 2;
int bluePin = 3;
int ledState = LOW; //initialise switch off the LED
int ledPin = 1; //initialise Red LED

void setup() {
    Serial.begin(9600);
    AudioMemory(50);
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_MIC);
    audioShield.micGain(10); // in dB
    audioShield.volume(0.5);
    notefreq.begin(.15);
    filter1.frequency(30); // filter out DC & extremely low frequencies
    amp1.gain(8.5);        // amplify sign to useful range
    
    //Verify the SD card
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
      while (1) {
        Serial.println("Unable to access the SD card");
        delay(500);
      }
    }

    //Pins for LED
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    dht.begin();
    lcd.init(); 
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("   Listening    ");
    lcd.setCursor(0,1);
    lcd.print("Humidity|Tempera");    
}

std::string str (""); 
int change=0; //change colour
bool detected=false; //"Hallo" detected
bool switched_on = false;
bool wait=false; //"Delay" after "Hallo" is detected to avoid mistake 
char line[10]; // for display temp and humi

//function to play WAV file from SD card
void playFile(const char *filename)
{
  playWav1.play(filename);
  delay(25);
}

void loop() {
  if (notefreq.available()) {
    float frequency = notefreq.read(); //read the frequency from the MIC
    Serial.print("Frequency: ");
    Serial.println(frequency);
    if (detected == false){
      if (str.length()<3){
         if (frequency>100 && frequency<125){ //if "Ha" is detected
          str+="b";
          }
         else if (frequency>=125 && frequency<145){ //if "llo" is detected
          str+="a";
          }
         else{
          str=""; //empty the string
          }
      }
      else{
        str.erase(0,1);
         if (frequency>100 && frequency<125){ //if "Ha" is detected
          str+="b";
          }
          else if (frequency>=125 && frequency<145){ //if "llo" is detected
          str+="a";
          }
         else{
          str=""; //empty the string
          }
       }
       if (str=="aab"){ //We assume "Hallo" sounds like Haaaaa llo - so there should be two "Ha" frequency detected before one "llo" frequency
        Serial.println("Hallo");
        detected=true;
//        delay(100);
        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds (it's a very slow sensor)
        float h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        float t = dht.readTemperature();
        if (isnan(h) || isnan(t)) {
          Serial.println(F("Failed to read from DHT sensor!"));
        }
//        Serial.print(F("Humidity: "));
//        Serial.print(h);
//        Serial.print(F("%  Temperature: "));
//        Serial.print(t);
//        Serial.println(F("°C "));
        lcd.setCursor(0,0);
        lcd.print("   Hey there!   ");
        lcd.setCursor(0,1);
        lcd.print(String(h,1) + " % | " + String(t,1) + " C ");
        playFile("hey.wav"); //response "Hey" 
        } 
       else{
        }
       wait=true; //need to delay the frequency detection to avoid mistake
    }
    else{
      if (frequency>3000 && frequency<3800){  //if "Switch" is detected
        detected=false; //Command is called, need a new wake word
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) {
          Serial.println("Switch On");
          lcd.setCursor(0,0);
          lcd.print("    Light ON    ");
          ledState = HIGH;
          switched_on = true;
        } else {
          Serial.println("Switch Off");
          lcd.setCursor(0,0);
          lcd.print("    Light OFF   ");          
          ledState = LOW;
          switched_on = false;
        }
        delay(350);
        digitalWrite(ledPin, ledState); //Show the LED
      }
      if(switched_on ==true && frequency>100 && frequency <150){ //When the led is switched on and "change" is detected
        change+=1;
        Serial.println("Change");
        }
       else if (switched_on ==true && (frequency<100 || frequency >150)){ //when "change" is not detected
        change=0;
        }
       if(change==1){ //When change is detected
        if (ledPin < 6){ //If led is not Blue colour
          digitalWrite(ledPin, LOW); //Off the LED
          ledPin+=1; //Change the next colour
          digitalWrite(ledPin, HIGH); //On the next LED colour
          }
         else if (ledPin ==6){ //If led is Blue colour
          digitalWrite(ledPin, LOW); //Off Blue LED
          ledPin=4; //Set Led to Red colour
          digitalWrite(ledPin, HIGH); //On Red LED
          }
          detected=false; //Command is called, need a new wake word
          change=0; 
        }
        wait=false;
    }
  }
  if (wait==true){ 
    delay(250); //delay 0.25 s
//    lcd.setCursor(0,0);
//    lcd.print("");
//    lcd.print("   Listening");
//    lcd.setCursor(0,1);
//    lcd.print("");
//    lcd.print("Humidity|Tempera");  
    }
}
