#include <stdint.h>
#include <stdlib.h>
#include <Audio.h>
#include "pv_audio.h"
#include "picovoice.h"

#define PV_AUDIO_REC_AUDIO_FREQUENCY (16000U)
#define PV_AUDIO_REC_CHANNEL_NUMBER (1)
#define PV_AUDIO_REC_RECORD_BUFFER_SIZE (512)

#define AUDIO_IN_PCM_BUFFER_SIZE (PV_AUDIO_REC_RECORD_BUFFER_SIZE)

static int16_t record_pcm_buffer[AUDIO_IN_PCM_BUFFER_SIZE];
// static int16_t ping_pong_buffer[2][PV_AUDIO_REC_RECORD_BUFFER_SIZE];

static int32_t last_read_index = -1;
static int32_t read_index = 1;
static int32_t write_index = 0;
static int32_t buffer_index = 0;

static int16_t buffer[PV_AUDIO_REC_RECORD_BUFFER_SIZE];

static AudioRecordQueue queue1;
// static AudioInputAnalog aia1;

struct {
  uint32_t channel_number;
  uint32_t audio_frequency;
  uint32_t record_buffer_size;
  int16_t *record_pcm_buffer;
  bool is_recording;
} pv_audio_rec;


pv_status_t pv_audio_rec_init(void) {
    pv_audio_rec.channel_number = PV_AUDIO_REC_CHANNEL_NUMBER;
    pv_audio_rec.audio_frequency = PV_AUDIO_REC_AUDIO_FREQUENCY;
    pv_audio_rec.record_buffer_size = PV_AUDIO_REC_RECORD_BUFFER_SIZE;
    pv_audio_rec.record_pcm_buffer = record_pcm_buffer;

    queue1.begin();

    pv_audio_rec.is_recording = true;

    return PV_STATUS_SUCCESS;
}


const int16_t *pv_audio_rec_get_new_buffer(void) {

    if (!pv_audio_rec.is_recording) {
        return NULL;
    }

    if (!queue1.available()) {
        return NULL;
    }

    for (int i = 0; i < PV_AUDIO_REC_RECORD_BUFFER_SIZE; i++) {
        buffer[i]=*(queue1.readBuffer());
    }

    queue1.freeBuffer();

    return buffer;
}

