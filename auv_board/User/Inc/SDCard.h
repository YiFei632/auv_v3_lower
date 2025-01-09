#ifndef __SDCARD_H__
#define __SDCARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sim_SPItwo.h"
#include "kernal.h"

typedef struct
{
	char pathBuff[64][16]; //�洢·��������64���ļ���ÿ���ļ������16���ֽ�
	uint32_t file_size[64];
	uint32_t totalSpace, freeSpace;//kb: totalSpace=0x78000KB=480MB
	
	FATFS *pfs;//�м�ṹ����
	DWORD fre_clust;
	FRESULT f_res; /* �ļ�������� */
	UINT fnum; /* �ļ��ɹ���д���� */
	BYTE ReadBuffer[512]; /* �������� */	
	
	int exp_num; //ʵ���������
//	time_manage_t time;
}file_in_flash;

//union ReadTemp{
//	BYTE b_temp[10];
//	char c_temp;
//};

//extern union ReadTemp rt;

extern int exp_num_temp;
extern file_in_flash nand_flash;

extern BYTE *ReadBuffer_expdata;


void SDCardInit(void);
_Bool SDCardRead(sdcard_command sd_cmd);
void SDCardCmd();
void SDCardWrite(void);
void DeleteFile(file_in_flash * ff, char * file_path);
void DeleteAllExp(file_in_flash * ff);
void MountSDCard(file_in_flash * ff);
void WriteSDCardTest(file_in_flash * ff);
void ReadSDCardTest(file_in_flash * ff);
void WriteExpInfoInit(file_in_flash * ff);
void ReadExpInfo(file_in_flash * ff);
void WriteExpInfoPlusOne(file_in_flash * ff);
void WriteExpDataTest(file_in_flash * ff);
void WriteExpData(file_in_flash * ff);
void ReadExpData(file_in_flash * ff);
//BYTE* ReadExpData(file_in_flash * ff);
void CheckFreeSpace(file_in_flash * ff);
void ScanFiles(file_in_flash * ff, char *path);
	

#ifdef __cplusplus
}
#endif
#endif /*__SDCARD_H__ */