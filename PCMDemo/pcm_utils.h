//
//  pcm_utils.h
//  PCMDemo
//
//  Created by BZF on 2021/11/1.
//

#ifndef pcm_utils_h
#define pcm_utils_h

int pcm16le_split(char *url);

int pcm16le_halfvolumeleft(char *url);

int pcm16le_doublespeed(char *url);

int pcm16le_to_pcm8(char *url);

int pcm16le_cut_singlechannel(char *url, int start_num, int dur_num);

int pcm16le_to_wave(const char *pcmpath, int channels, int sample_rate, const char *wavepath);

#endif /* pcm_utils_h */
