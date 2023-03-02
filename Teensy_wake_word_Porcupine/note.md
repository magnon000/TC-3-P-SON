# note
## teensy voice recognition
* https://forum.pjrc.com/threads/57401-Teensy-4-0-and-Voice-Recognition?highlight=recognition
* https://forum.pjrc.com/threads/57363-Take-a-look-at-my-Voice-to-MIDI-(pitch-recognition)-work-in-progress?highlight=recognition
* 

## modifiy sample rate for VAD
in control_sgtl5000.cpp
```
#define CHIP_CLK_CTRL			0x0004
// 5:4	RATE_MODE	Sets the sample rate mode. MCLK_FREQ is still specified
//			relative to the rate in SYS_FS
//				0x0 = SYS_FS specifies the rate
//				0x1 = Rate is 1/2 of the SYS_FS rate
//				0x2 = Rate is 1/4 of the SYS_FS rate
//				0x3 = Rate is 1/6 of the SYS_FS rate
// 3:2	SYS_FS		Sets the internal system sample rate (default=2)
//				0x0 = 32 kHz
//				0x1 = 44.1 kHz
//				0x2 = 48 kHz
//				0x3 = 96 kHz
// 1:0	MCLK_FREQ	Identifies incoming SYS_MCLK frequency and if the PLL should be used
//				0x0 = 256*Fs
//				0x1 = 384*Fs
//				0x2 = 512*Fs
//				0x3 = Use PLL
//				The 0x3 (Use PLL) setting must be used if the SYS_MCLK is not
//				a standard multiple of Fs (256, 384, or 512). This setting can
//				also be used if SYS_MCLK is a standard multiple of Fs.
//				Before this field is set to 0x3 (Use PLL), the PLL must be
//				powered up by setting CHIP_ANA_POWER->PLL_POWERUP and
//				CHIP_ANA_POWER->VCOAMP_POWERUP.  Also, the PLL dividers must
//				be calculated based on the external MCLK rate and
//				CHIP_PLL_CTRL register must be set (see CHIP_PLL_CTRL register
//				description details on how to calculate the divisors).
```
* for now use default
* https://github.com/PaulStoffregen/cores/blob/adca415331771bbf1560d7a008ccb150b0ec603a/teensy3/AudioStream.h
* https://forum.pjrc.com/threads/62169-Accept-audio-at-16khz-16bit-via-USB?highlight=recognition


## Audio data is stored in 128 sample blocks
https://github.com/PaulStoffregen/cores/blob/adca415331771bbf1560d7a008ccb150b0ec603a/teensy3/AudioStream.h

## teensy uid
https://github.com/sstaub/TeensyID

## ASR papers
https://arxiv.org/ftp/arxiv/papers/1003/1003.4083.pdf
[6] matlab code
## existing module
* easyVR
* https://forum.pjrc.com/threads/44368-Porting-Elechouse-VoiceRecognitionV3-library?highlight=recognition

## picovoice
* https://forum.pjrc.com/threads/70477-picovoice-for-teensy?highlight=recognition

## TTS
* https://forum.pjrc.com/threads/44587-TTS-(Text-to-Speech)-Library-Port?highlight=recognition

## voice auth
https://forum.pjrc.com/threads/66288-Voice-Authentication-with-FFT?highlight=recognition
