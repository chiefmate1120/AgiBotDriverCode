#include "Motor_Test.h"
#include "arm_math.h"
uint32_t sineWaveCount;
sine_wave_generator_t SineWaveGenerator = {0};

void sine_wave_init(sine_wave_generator_t *gen, float amp, float freq, int offset_value)
{
    gen->amplitude = amp;
    gen->frequency = freq;
    gen->phase_accumulator = 0.0;
		gen->offset_value = offset_value;
}

void sine_wave_control(sine_wave_generator_t *gen, generator_state_e flag)
{
	gen->enable_flag = flag;
}

void sine_wave_update_params(sine_wave_generator_t *gen, float amp, float freq)
{
    if (gen == 0) return;
    gen->amplitude = amp;
    gen->frequency = freq;
}

float sine_wave_get_sample(sine_wave_generator_t *gen, uint32_t current_time_us)
{
    static const float INV_1E6 = 1.0f / 1000000.0f;
    static const float TWO_PI = 2.0f * M_PI_F;
   
		uint32_t time_diff_us;

    if (gen == 0) return 0.0f;
		
		if(gen->enable_flag == generator_disable) return 0.0f;
	
		time_diff_us = current_time_us-gen->last_time_us;
    float time_diff_sec = (float)time_diff_us * INV_1E6;
    
    float phase_increment = TWO_PI * gen->frequency * time_diff_sec;
    gen->phase_accumulator += phase_increment;

    if (gen->phase_accumulator >= TWO_PI) {
        gen->phase_accumulator -= TWO_PI;
    }

    float sample = gen->amplitude * arm_sin_f32(gen->phase_accumulator) + gen->offset_value;
		gen->value = sample;
		gen->last_time_us = current_time_us;
    return sample;
}
