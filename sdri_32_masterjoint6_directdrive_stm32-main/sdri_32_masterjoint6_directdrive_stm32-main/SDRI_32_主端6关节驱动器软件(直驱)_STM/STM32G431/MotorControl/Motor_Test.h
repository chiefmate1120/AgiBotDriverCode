#ifndef __MOTOR_TEST_H
#define __MOTOR_TEST_H

#include <math.h>
#include <stdint.h>

#ifndef M_PI_F
    #define M_PI_F 3.14159265358979323846f
#endif
typedef enum 
{
	generator_disable = 0x00,
	generator_enable,
}generator_state_e;

typedef struct {
    float amplitude;      // 幅值 (Amplitude)
    float frequency;      // 频率 (Hz)
    float phase_accumulator; // 相位累加器 (弧度)
    float value;      // 频率 (Hz)
		uint32_t last_time_us;
		int32_t offset_value;
		generator_state_e  enable_flag;
} sine_wave_generator_t;

extern sine_wave_generator_t SineWaveGenerator;


void sine_wave_init(sine_wave_generator_t *gen, float amp, float freq, int offset_value);
void sine_wave_update_params(sine_wave_generator_t *gen, float amp, float freq);
float sine_wave_get_sample(sine_wave_generator_t *gen, uint32_t current_time_us);
void sine_wave_control(sine_wave_generator_t *gen, generator_state_e flag);

#endif
