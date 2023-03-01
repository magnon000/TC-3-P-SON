#include "src/picovoice.h"
#include "src/pv_porcupine.h"
#include "model/pv_porcupine_params.h"
#include "src/pv_audio.h"

// user defined
#define MEMORY_BUFFER_SIZE (100 * 1024)
static const char* ACCESS_KEY = "";

static uint8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));

static pv_porcupine_t *handle = NULL;

// KEYWORD_ARRAY_LENGTH & KEYWORD_ARRAY in Porcupine .h file
static const int32_t KEYWORD_MODEL_SIZES = KEYWORD_ARRAY_LENGTH;
static const void *KEYWORD_MODELS = KEYWORD_ARRAY;

static const float SENSITIVITY = 0.5f;

static void wake_word_callback(void) {
    Serial.println("Wake word detected!");
}

//extern const int16_t *get_next_audio_frame(void);


void setup() {
    Serial.begin(9600);
    while (!Serial);

    pv_status_t status = pv_audio_rec_init();
    if (status != PV_STATUS_SUCCESS) {
        Serial.print("Audio init failed with ");
        Serial.println(pv_status_to_string(status));
        while (1);
    }

    status = pv_porcupine_init(
            ACCESS_KEY,
            MEMORY_BUFFER_SIZE,
            memory_buffer,
            1,
            &KEYWORD_MODEL_SIZES,
            &KEYWORD_MODELS,
            &SENSITIVITY,
            &handle);
    if (status != PV_STATUS_SUCCESS) {
        Serial.print("Picovoice init failed with ");
        Serial.println(pv_status_to_string(status));
        while (1);
    }
    Serial.println("The board is listening for 'Show me temperature'...");
}

void loop()
{
    const int16_t *buffer = pv_audio_rec_get_new_buffer();
    if (buffer) {
        int32_t keyword_index;
        const pv_status_t status = pv_porcupine_process(handle, buffer, &keyword_index);
        if (status != PV_STATUS_SUCCESS) {
            Serial.print("Picovoice process failed with ");
            Serial.println(pv_status_to_string(status));
            while(1);
        }
        if (keyword_index != -1) {
            wake_word_callback();
        }
    }
}
//    const int16_t *pcm = pv_audio_rec_get_new_buffer();
//    int32_t keyword_index;
//    const pv_status_t status = pv_porcupine_process(porcupine, pcm, &keyword_index);
//    if (status != PV_STATUS_SUCCESS) {
//        // error handling logic
//        Serial.print("Picovoice failed");
//    }
//    if (keyword_index != -1) {
//        // detection event logic/callback
//        wake_word_callback();
//    }
