//
//  main.cpp
//  PCMDemo
//
//  Created by BZF on 2021/11/1.
//

#include <iostream>
#include "pcm_utils.h"

int main(int argc, const char * argv[]) {
    
    char *url = "/Users/bzf/Desktop/XcodeDemo/PCMDemo/asset/NocturneNo2inEflat_44.1k_s16le.pcm";
    
//    pcm16le_split(url);
    
//    pcm16le_halfvolumeleft(url);
    
//    pcm16le_doublespeed(url);
    
//    pcm16le_to_pcm8(url);
    
//    pcm16le_cut_singlechannel("/Users/bzf/Desktop/XcodeDemo/PCMDemo/asset/drum.pcm", 2360, 120);
    
    pcm16le_to_wave(url, 2, 44100, "/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/out_nocturne.wav");
    
    return 0;
}
