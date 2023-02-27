#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <iostream>
#include <string>

//---------------------------------------------------------------------------------------
AudioInputI2S            i2s1;           
AudioFilterStateVariable filter1;       
AudioAmplifier           amp1;           
AudioAnalyzeNoteFrequency notefreq;
AudioControlSGTL5000     audioShield;      
//---------------------------------------------------------------------------------------
AudioConnection          patchCord1(i2s1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 2, amp1, 0);
AudioConnection          patchCord3(amp1, notefreq);
//---------------------------------------------------------------------------------------

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
}

std::string str ("");
bool detected=false;

void loop() {
  if (notefreq.available()) {
    float frequency = notefreq.read();
    //Serial.print("Frequency: ");
    //Serial.println(frequency);
    
    if (str.length()<3){
       if (frequency>100 && frequency<=120){
        str+="b";
        }
       else if (frequency>=120 && frequency<145){
        str+="c";
        }
       else{
        str="";
        }
    }
    else{
      str.erase(0,1);
       if (frequency>100 && frequency<=120){
        str+="b";
        }
        else if (frequency>=120 && frequency<145){
        str+="c";
        }
       else{
        str="";
        }
     }
     if (str=="ccb"){
      Serial.println("Detected");
      detected=true;
      } 
     else{
      }
  }
  
  //delay(150);
}
