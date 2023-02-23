#include <Audio.h>
#include <SD.h>
#include <SPI.h>

// Audio objects
AudioInputAnalog          adc;        // Audio input from analog pin
AudioRecordQueue          queue;      // Queue for audio samples
AudioOutputI2S            i2s;        // Audio output to I2S
AudioConnection           patchCord1(adc, 0, queue, 0);
AudioConnection           patchCord2(queue, 0, i2s, 0);
AudioConnection           patchCord3(queue, 0, i2s, 1);

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// SD card objects
//const int chipSelect = BUILTIN_SDCARD;
File audioFile;

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Initialize audio objects
  AudioMemory(10);
  queue.begin();
  i2s.begin();

  // Initialize SD card
  //SD.begin(chipSelect);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  // Delete the existing audio file
  SD.remove("audio1.wav");

  // Create a new audio file
  audioFile = SD.open("audio1.wav", FILE_WRITE);

  // Loop infinitely record audio with 3 seconds duration
  loopRecordAudio(3);
}

void loop() {
  // Check if audio is available in the queue
  if (queue.available() > 0) {
    // Read the audio data from the queue and write it to the audio file
    audioFile.write(queue.readBuffer(), queue.available() * sizeof(int16_t));

    // Wait for a short time to avoid overwhelming the SD card
    delay(500);
  }
}

void finishRecording() {
  // Stop recording and close the audio file
  queue.end();
  audioFile.close();

  Serial.println("Recording finished.");
}

void recordAudioForDuration(int durationSeconds) {
  // Set the duration of the recording in milliseconds
  int recordingDuration = durationSeconds * 1000;

  // Start recording
  queue.begin();
  Serial.println("Recording started.");
  // Wait for the recording to complete
  delay(recordingDuration);

  // Stop recording and close the audio file
  finishRecording();
}

void loopRecordAudio(int durationSeconds) {
  while (true) {
    // Record audio for the specified duration
    recordAudioForDuration(durationSeconds);

    // Delete the existing audio file
    SD.remove("audio1.wav");

    // Create a new audio file
    audioFile = SD.open("audio1.wav", FILE_WRITE);
  }
}
