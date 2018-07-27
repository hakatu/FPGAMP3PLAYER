/*
 * chu_avalon_audio.h
 *
 *  Created on: 02-05-2018
 *      Author: Vuong Dinh Hung
 */

/****************************************************
 * Module: 	Audio codec driver header
 * File: 	chu_avalon_audio.h
 * Purpose:  routines to configure codec,retrieve data
 * 			from ADC and send data to DAC
 *****************************************************
 * For more information, please read file MP3_DAP.pdf
 ****************************************************/
#ifndef CHU_AVALON_AUDIO_H_
#define CHU_AVALON_AUDIO_H_

#include <alt_types.h>

#define CHU_AUD_I2C_DATA_REG	0
#define CHU_AUD_STATUS_REG 		1
#define CHU_AUD_DBUS_SEL_REG	1
#define CHU_AUD_DAC_DATA_REG 	2
#define CHU_AUD_ADC_DATA_REG 	3

int audio_i2c_is_idle(alt_u32 audio_base);
void audio_i2c_wr_cmd(alt_u32 audio_base, alt_u8 addr, alt_u16 cmd);
void audio_init(alt_u32 audio_base);

void audio_wr_src_sel(alt_u32 audio_base, int dac_sel, int adc_sel);

int audio_dac_fifo_full(alt_u32 audio_base);
void audio_dac_wr_fifo(alt_u32 audio_base, alt_u32 data);

int audio_adc_fifo_empty(alt_u32 audio_base);
alt_u32 audio_adc_rd_fifo(alt_u32 audio_base);

#endif /* CHU_AVALON_AUDIO_H_ */
