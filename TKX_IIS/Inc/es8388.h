#ifndef __ES8388_H__
#define __ES8388_H__

#include "main.h"

#define ES8388_ADDR     0x10                        /* ES8388的器件地址,固定为0x10 */

/* 函数声明 */
uint8_t es8388_write_reg(uint8_t reg, uint8_t val);
uint8_t es8388_read_reg(uint8_t reg);
uint8_t es8388_init(void);
void es8388_i2s_cfg(uint8_t fmt, uint8_t len);
void es8388_hpvol_set(uint8_t volume);
void es8388_spkvol_set(uint8_t volume);
void es8388_3d_set(uint8_t depth);
void es8388_adda_cfg(uint8_t dacen, uint8_t adcen);
void es8388_output_cfg(uint8_t o1en, uint8_t o2en);
void es8388_mic_gain(uint8_t gain);
void es8388_alc_ctrl(uint8_t sel, uint8_t maxgain, uint8_t mingain);
void es8388_input_cfg(uint8_t in);

#endif // !__ES8388_H__