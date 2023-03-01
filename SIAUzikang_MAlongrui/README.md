# SON: 3TC Audio Project
## _The Last Markdown Editor, Ever_

During this project, we programmed an embedded system in Teensy 4.0 for 
real-time audio signal processing applications. We developed the voice 
recognition technology using **frequency detection** that has flexibility 
for different languages and dialects, rapid real-time processing and high 
accuracy to assure reliable and efficient functioning.

## Features
- When the wake word - "Hallo" is detected, "Hey" is replied to through 
the headset
- The program waits for the commands (Switch on/off and change)
- When "Switch on" is detected, the program will switch on the RGB LED
- To call another command, wake word - "Hallo" should be spoken
- We then can call "change" to change the colours of RGB LED
- RGB LED will be switched off when "Switch off" is detected after the 
wake word 

**The frequency of a spoken word can be influenced by various factors, 
including the speaker's age, gender, accent and emotion. Thus, the program 
needs to be customised for every different users.**

For this project, the user and frequency range for "Ha" between 125 and 
145 Hz, while "llo" between 100 and 125 Hz. 

## Frequency
**The frequency range can be detected** : 
- Online pitch detector
- "Examples"->"Audio"->"Analysis"->"NoteFrequency" in Teensyduino.

The audio signal needs to be filtered and amplified before reading the 
frequency
**Input** from I2S
```sh
AudioInputI2S            i2s1; 
```

**Filtering**
```sh
AudioFilterStateVariable filter1; 
filter1.frequency(30); // filter out DC & extremely low frequencies
```
**Amplifying**
```sh
AudioAmplifier           amp1; 
amp1.gain(8.5);        // amplify sign to useful range
```
**Reading the frequency**
```sh
AudioAnalyzeNoteFrequency notefreq;
float frequency = notefreq.read(); //read the frequency from the MIC
```
**Audio Connection**
```sh
AudioConnection          patchCord1(i2s1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 2, amp1, 0);
AudioConnection          patchCord3(amp1, notefreq);
```

## "Hey" replied
- Record the audio and save into a SD card
- Slot the SD card in the Audio Shield

**Verify the SD card**
```sh
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

SPI.setMOSI(SDCARD_MOSI_PIN);
SPI.setSCK(SDCARD_SCK_PIN);
if (!(SD.begin(SDCARD_CS_PIN))) {
  while (1) {
    Serial.println("Unable to access the SD card");
    delay(500);
  }
}
```
**Play the audio**
```sh
AudioPlaySdWav           playWav1;
AudioOutputI2S           audioOutput; 
AudioConnection          patchCord4(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord5(playWav1, 1, audioOutput, 1);

playWav1.play(filename);
```
## LED
Connect GND and different legs to available Digital Pins in Teensy 4.0

**LED State**
- LOW : Off
- HIGH : On

**Control the LED**
```sd
digitalWrite(ledPin, ledState);
```
**ledPin** : Digital Pins connected to Teensy 4.0










