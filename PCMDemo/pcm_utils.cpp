//
//  pcm_utils.cpp
//  PCMDemo
//
//  Created by BZF on 2021/11/1.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//分离PCM16LE双声道音频采样数据的左声道和右声道
int pcm16le_split(char *url){
    FILE *fp = NULL;
    if((fp = fopen(url, "rb+")) == NULL){
        printf("Fail to open file!\n");
        return -1;
    }
    
    FILE *fpLeft = fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_l.pcm", "wb+");
    FILE *fpRight = fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_r.pcm", "wb+");
    
    unsigned char *sample = (unsigned char *)malloc(4);
    
    while (!feof(fp)) {
        fread(sample, 1, 4, fp);
        fwrite(sample, 1, 2, fpLeft);
        fwrite(sample + 2, 1, 2, fpRight);
    }
    free(sample);
    fclose(fp);
    fclose(fpLeft);
    fclose(fpRight);
    
    
    return 0;
    
}

//将PCM16LE双声道音频采样数据中左声道的音量降一半
int pcm16le_halfvolumeleft(char *url){
    FILE *fp = NULL;
    if((fp = fopen(url, "rb+")) == NULL){
        printf("Fail to open file!\n");
        return -1;
    }
    FILE *fpOut = fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_halfleft.pcm", "wb+");
    
    int cnt = 0;
    unsigned char *sample = (unsigned char *)malloc(4);
    
    
    while (!feof(fp)) {
        short *samplenum=NULL;
        fread(sample, 1, 4, fp);
        samplenum = (short *)sample;
        *samplenum = *samplenum/2;
        fwrite(sample, 1, 2, fpOut);
        fwrite(sample+2, 1, 2, fpOut);
        
        cnt++;
        
    }
    printf("Sample Cnt:%d\n", cnt);
    
    free(sample);
    fclose(fp);
    fclose(fpOut);
    
    return 0;
}


//将PCM16LE双声道音频采样数据的声音速度提高一倍
int pcm16le_doublespeed(char *url){
    FILE *fp = NULL;
    
    if((fp=fopen(url, "rb+"))==NULL){
        printf("Fail to open file!\n");
        return -1;
    }
    FILE *fpOut = fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_doublespeed.pcm", "wb+");
    
    int cnt = 0;
    
    unsigned char *sample = (unsigned char *)malloc(4);
    
    while (!feof(fp)) {
        fread(sample, 1, 4, fp);
        if(cnt%2!=0){
            fwrite(sample, 1, 2, fpOut);
            fwrite(sample+2, 1, 2, fpOut);
        
        }
        cnt++;
    }
    printf("采样次数：%d\n", cnt);
    free(sample);
    fclose(fp);
    fclose(fpOut);
    
    return 0;
}


//将PCM16LE双声道音频采样数据转换为PCM8音频采样数据
//PCM16LE格式的采样数据的取值范围是-32768到32767，而PCM8格式的采样数据的取值范围是0到255。所以PCM16LE转换到PCM8需要经过两个步骤：第一步是将-32768到32767的16bit有符号数值转换为-128到127的8bit有符号数值，第二步是将-128到127的8bit有符号数值转换为0到255的8bit无符号数值
int pcm16le_to_pcm8(char *url){
    FILE *fp=NULL;
    if((fp=fopen(url,"rb+"))==NULL){
        printf("Fail to open file!\n");
        return -1;
    }
    FILE *fp1=fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_8.pcm", "wb+");
    
    int cnt=0;
    unsigned char *sample=(unsigned char *)malloc(4);
    
    while (!feof(fp)) {
        
        short *samplenum16=NULL;
        char samplenum8=0;
        unsigned char samplenum8_u=0;
        fread(sample, 1, 4, fp);
        
        //(-32768-32767)
        samplenum16 = (short *)sample;
        samplenum8=(*samplenum16)>>8;
        
        //(0-255)
        samplenum8_u=samplenum8 + 128;
        
        fwrite(&samplenum8_u, 1, 1, fp1);
        
        samplenum16 = (short *)(sample+2);
        samplenum8=(*samplenum16)>>8;
        samplenum8_u = samplenum8 + 128;
        
        fwrite(&samplenum8_u, 1, 1, fp1);
        
        cnt++;
    }
    printf("采样次数：%d\n",cnt);
    
    free(sample);
    fclose(fp);
    fclose(fp1);
    
    return 0;
}


//从PCM16LE单声道音频采样数据中截取一部分数据
/**
 * Cut a 16LE PCM single channel file.
 * @param url        Location of PCM file.
 * @param start_num  start point
 * @param dur_num    how much point to cut
 */
int pcm16le_cut_singlechannel(char *url, int start_num, int dur_num){
    FILE *fp = NULL;
    if((fp=fopen(url, "rb+"))==NULL){
        printf("Fail to open file!\n");
        return -1;
    }
    FILE *fpCut = fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_cut.pcm", "wb+");
    FILE *fpCutTxt = fopen("/Users/bzf/Desktop/XcodeDemo/PCMDemo/outputs/output_cut.txt", "wb+");
    
    unsigned char *sample = (unsigned char *)malloc(2);
    
    int cnt=0;
    while (!feof(fp)) {
        fread(sample, 1, 2, fp);
        if(cnt>start_num && cnt<=(start_num+dur_num)){
            fwrite(sample, 1, 2, fpCut);
            
            short samplenum = sample[1];
            samplenum = samplenum*256;
            samplenum =samplenum + sample[0];
            
            fprintf(fpCutTxt, "%6d", samplenum);
            if(cnt%10==0){
                fprintf(fpCutTxt, "\n", samplenum);
            }
        }
        cnt++;
    }
    free(sample);
    fclose(fp);
    fclose(fpCut);
    fclose(fpCutTxt);
    
    return 0;
}


/**
 * 将PCM16LE双声道音频采样数据转换为WAVE格式音频数据
 * WAVE格式音频（扩展名为“.wav”）是Windows系统中最常见的一种音频。该格式的实质就是在PCM文件的前面加了一个文件头
*/
int pcm16le_to_wave(const char *pcmpath, int channels, int sample_rate, const char *wavepath){
    
    typedef  struct WAVE_HEADER{
        char fccID[4];
        //是整个文件的长度减去ID和Size的长度, 占用4个字节
        unsigned int dwSize;
        char fccType[4];
    }WAVE_HEADER;
    
    typedef struct WAVE_FMT{
        char fccID[4];
        unsigned int dwSize;
        unsigned short wFormatTag;
        unsigned short wChannels;
        unsigned int dwSamplesPerSec;
        unsigned int dwAvgBytesPerSec;
        unsigned short wBlockAlign;
        unsigned short uiBitsPerSample;
    }WAVE_FMT;
    
    typedef struct WAVE_DATA{
        char fccID[4];
        unsigned int dwSize;
    }WAVE_DATA;

    if(channels==0 || sample_rate==0){
        channels = 2;
        sample_rate = 44100;
    }
    int bits = 16;
    WAVE_HEADER pcmHEADER;
    WAVE_FMT pcmFMT;
    WAVE_DATA pcmDATA;
    
    unsigned short m_pcmData;
    FILE *fp,*fpout;
    
    if((fp = fopen(pcmpath, "rb+")) == NULL){
        printf("Fail to open file!\n");
        return -1;
    }
    if((fpout=fopen(wavepath, "wb+")) == NULL){
        printf("Fail create wav file\n");
        return -1;
    }
    memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
    memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
    fseek(fpout, sizeof(WAVE_HEADER), SEEK_CUR);
    
    pcmFMT.dwSamplesPerSec = sample_rate;
    pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec*sizeof(m_pcmData);
    pcmFMT.uiBitsPerSample = bits;
    memcpy(pcmFMT.fccID, "fmt ", strlen("fmt "));
    pcmFMT.dwSize = 16;
    pcmFMT.wBlockAlign = 2;
    pcmFMT.wChannels = channels;
    pcmFMT.wFormatTag = 1;
    
    fwrite(&pcmFMT, sizeof(WAVE_FMT), 1, fpout);
    
    memcpy(&pcmDATA, "data", strlen("data"));
    pcmDATA.dwSize=0;
    fseek(fpout, sizeof(WAVE_DATA), SEEK_CUR);
    
    fread(&m_pcmData, sizeof(unsigned short),1, fp);
    while (!feof(fp)) {
        pcmDATA.dwSize +=2;
        fwrite(&m_pcmData, sizeof(unsigned short), 1, fpout);
        fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    }
    
    pcmHEADER.dwSize = 44 + pcmDATA.dwSize;
    
    rewind(fpout);
    fwrite(&pcmHEADER, sizeof(WAVE_HEADER), 1, fpout);
    fseek(fpout, sizeof(WAVE_FMT), SEEK_CUR);
    fwrite(&pcmDATA, sizeof(WAVE_DATA), 1, fpout);
    
    fclose(fp);
    fclose(fpout);
    
    return 0;
}
