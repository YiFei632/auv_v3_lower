#include "SDCard.h"

BYTE WriteBuffer_test1[] = "This is STM32 working with FatFs\r\n";/* 写缓冲区 */
BYTE WriteBuffer_test2[] = "Test by xin at 20240903 11:00am\r\n";
BYTE WriteBuffer_expinfo_init[] = "exp000\r\n";
BYTE WriteBuffer_expinfo_plusone[9];
BYTE WriteBuffer_expinfo[11];
BYTE WriteBuffer_expdata[100];

BYTE ReadBuffer_expinfo[11];
file_in_flash nand_flash;

int exp_num_temp;

BYTE *ReadBuffer_expdata;

//union ReadTemp rt;

void SDCardInit()
{
	//nand_flash.f_res = f_mkfs("0:/",FM_EXFAT,0,&nand_flash.ReadBuffer,sizeof(nand_flash.ReadBuffer));//格式化SDNAND
	MountSDCard(&nand_flash);
//	DeleteAllExp(&nand_flash);
//	WriteSDCardTest(&nand_flash);
//	ReadSDCardTest(&nand_flash);
//	WriteExpInfoInit(&nand_flash);
	ReadExpInfo(&nand_flash);
//	DeleteFile(&nand_flash, "0:/exp000.txt");
	
  CheckFreeSpace(&nand_flash);
	ScanFiles(&nand_flash, "0:/");
	
	exp_num_temp = (&nand_flash)->exp_num;
	ReadBuffer_expdata = NULL;
}

_Bool SDCardRead(sdcard_command cmd)
{
//	ReadExpInfo(&nand_flash);//读取新的实验序号，确认
//	ReadExpData(&nand_flash);//读取一组IMU数据基本不花时间，1毫秒以内
//	switch(sd_cmd){
//		case SD_READ_INFO:{
//			
//		}
//	}
	if(cmd == SD_READ_INFO){
		ReadExpInfo(&nand_flash);
		return 1;
	}
	if(cmd == SD_INFO_PLUS){
		exp_num_temp ++ ;
		return 1;
	}
	if(cmd == SD_INFO_MINUS){
		if(exp_num_temp > 1){
		exp_num_temp -- ;
		}else{
			exp_num_temp = 1;
		}
		return 1;
		
	}
	if(cmd == SD_READ_DATA){
//		ReadBuffer_expdata = ReadExpData(&nand_flash);
		ReadExpData(&nand_flash);
		return 1;
		
		//return 0;
	}
	
	return 0;
	
	
}

void SDCardCmd(){
	_Bool tmp = SDCardRead(sd_cmd);
	if(tmp == 1){
		sd_cmd = SD_NO_COMMAND;
	}
}

void SDCardWrite()
{
	stop_all_IT();
	WriteExpInfoPlusOne(&nand_flash);//读取当前记录实验序号并+1
	ReadExpInfo(&nand_flash);//读取新的实验序号，确认
//	nand_flash.time.start_time = HAL_GetTick();
	
	if(is_record)
		WriteExpData(&nand_flash);
	else
		WriteExpDataTest(&nand_flash);//写一组IMU数据花费了约6毫秒
	
	memset(nand_flash.pathBuff, 0, sizeof(nand_flash.pathBuff));
	memset(nand_flash.file_size, 0, sizeof(nand_flash.file_size));
	CheckFreeSpace(&nand_flash);
	ScanFiles(&nand_flash, "0:/");
	
//	nand_flash.time.end_time = HAL_GetTick();
//	nand_flash.time.dur_time = nand_flash.time.end_time - nand_flash.time.start_time;
	start_all_IT();
}

void MountSDCard(file_in_flash * ff)
{
	ff->f_res = f_mount(&SDFatFS, "0:/",1);
	HAL_Delay(100);
	if(ff->f_res == FR_NO_FILESYSTEM)
	{
		ff->f_res = f_mkfs("0:/",FM_EXFAT|FM_SFD,0,&ff->ReadBuffer,sizeof(ff->ReadBuffer));//格式化SDNAND
		HAL_Delay(100);
		if(ff->f_res == FR_OK)
		{
			ff->f_res = f_mount(NULL,"0:/",1);//取消挂载
			ff->f_res = f_mount(&SDFatFS, "0:/",1);//重新挂载
		}
	}
	HAL_Delay(100);
}

void DeleteFile(file_in_flash * ff, char * file_path)
{
	//删除文件或目录--------------------------------------------------------------------
	ff->f_res = f_unlink(file_path);// "0:/log.txt"
}

void DeleteAllExp(file_in_flash * ff)
{
	//删除所有实验，重置实验序号--------------------------------------------------------------------
	ReadExpInfo(&nand_flash);
	for(int k = 0; k <= ff->exp_num; k++)
	{
		BYTE temp[20];
		sprintf((char *)temp, "0:/exp%03d.txt", k);
		DeleteFile(ff, (char *)temp);
	}
	WriteExpInfoInit(ff);
}

void WriteSDCardTest(file_in_flash * ff)
{
	//写测试--------------------------------------------------------------------
	ff->f_res = f_open(&SDFile,"helloWorld.txt",FA_CREATE_ALWAYS|FA_WRITE);
	HAL_Delay(100);
	ff->f_res = f_write(&SDFile,WriteBuffer_test2,sizeof(WriteBuffer_test2),&ff->fnum);
	HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
}

void ReadSDCardTest(file_in_flash * ff)
{
	//读测试--------------------------------------------------------------------
	ff->f_res = f_open(&SDFile,"helloWorld.txt",FA_OPEN_EXISTING | FA_READ);
	HAL_Delay(100);
	if(ff->f_res == FR_OK)
	{
		ff->f_res = f_read(&SDFile,ff->ReadBuffer,sizeof(ff->ReadBuffer),&ff->fnum);
	}
	HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
	HAL_Delay(100);
}

void WriteExpInfoInit(file_in_flash * ff)
{
	//写实验信息--------------------------------------------------------------------
	ff->f_res = f_open(&SDFile,"exp_info.txt",FA_CREATE_ALWAYS|FA_WRITE);
	HAL_Delay(100);
	ff->f_res = f_write(&SDFile,WriteBuffer_expinfo_init,sizeof(WriteBuffer_expinfo_init),&ff->fnum);
	HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
}

void ReadExpInfo(file_in_flash * ff)
{
	//读实验信息--------------------------------------------------------------------
	ff->f_res = f_open(&SDFile,"exp_info.txt",FA_OPEN_EXISTING | FA_READ);
	HAL_Delay(100);
	if(ff->f_res == FR_OK)
	{
		ff->f_res = f_read(&SDFile,ff->ReadBuffer,sizeof(ff->ReadBuffer),&ff->fnum);
	}
	sscanf((const char *)ff->ReadBuffer, "exp%d", &(ff->exp_num));
	exp_num_temp = ff->exp_num;
	HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
	HAL_Delay(100);
}

void WriteExpInfoPlusOne(file_in_flash * ff)
{
	//写实验信息，实验编号+1--------------------------------------------------------------------
	ff->f_res = f_open(&SDFile,"exp_info.txt",FA_CREATE_ALWAYS|FA_WRITE);
	HAL_Delay(100);
	sprintf((char *)WriteBuffer_expinfo_plusone, "exp%03d\r\n", ff->exp_num+1);
	ff->f_res = f_write(&SDFile,WriteBuffer_expinfo_plusone,sizeof(WriteBuffer_expinfo_plusone),&ff->fnum);
	HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
}

void WriteExpDataTest(file_in_flash * ff)
{
	//写实验数据--------------------------------------------------------------------
	sprintf((char *)WriteBuffer_expinfo, "exp%03d.txt", ff->exp_num);
	ff->f_res = f_open(&SDFile, (char *)WriteBuffer_expinfo, FA_OPEN_APPEND|FA_WRITE);
	//HAL_Delay(100);
	float nums[9];
	for(int k=0; k<3; k++)
	{
		nums[k] = IMU1.acc[k];
		nums[k+3] = IMU1.gyro[k];
		nums[k+6] = IMU1.angle[k];
	}
	for (int k = 0; k < 9; k++)
	{
    sprintf((char *)WriteBuffer_expdata + strlen((char *)WriteBuffer_expdata), "%10.4f ", nums[k]);
		sprintf((char *)WriteBuffer_expdata + strlen((char *)WriteBuffer_expdata) - 1, "");
  }
	sprintf((char *)WriteBuffer_expdata + strlen((char *)WriteBuffer_expdata) - 1, "\r\n");
	
	ff->f_res = f_write(&SDFile,WriteBuffer_expdata,sizeof(WriteBuffer_expdata),&ff->fnum);
	//HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
}


BYTE temp_1[100] = {0};
BYTE temp_2[12] = {0};
void WriteExpData(file_in_flash * ff)
{
	//写实验数据--------------------------------------------------------------------
	sprintf((char *)WriteBuffer_expinfo, "exp%03d.txt", ff->exp_num);
	ff->f_res = f_open(&SDFile, (char *)WriteBuffer_expinfo, FA_OPEN_APPEND|FA_WRITE);
	//HAL_Delay(100);
	for(int j = 0; j < record_num; j++)
	{
		float nums[9];
		for(int k=0; k<3; k++)
		{
			nums[k] = imu_record[j].acc[k];
			nums[k+3] = imu_record[j].gyro[k];
			nums[k+6] = imu_record[j].angle[k];
		}
		
		for (int k = 0; k < 9; k++)
		{
			sprintf((char *)temp_1 + strlen((char *)temp_1), "%10.4f ", nums[k]);
			sprintf((char *)temp_1 + strlen((char *)temp_1) - 1, "");
		}
		sprintf((char *)temp_1 + strlen((char *)temp_1) - 1, "\r\n");
		
		ff->f_res = f_write(&SDFile,temp_1,sizeof(temp_1),&ff->fnum);
	}
	
	for(int j = 0; j < record_num; j++)
	{
//		float num = wf5803_record[j].pressure;
//		
//		sprintf((char *)temp_2 + strlen((char *)temp_2), "%10.4f ", num);
//		sprintf((char *)temp_2 + strlen((char *)temp_2) - 1, "");
//		sprintf((char *)temp_2 + strlen((char *)temp_2) - 1, "\r\n");
//		
//		ff->f_res = f_write(&SDFile,temp_2,sizeof(temp_2),&ff->fnum);
	}
	//HAL_Delay(100);
//	for(int i = 0;i < record_num;i++){
//		ff->f_res = f_write(&SDFile,temp_1,sizeof(temp_1),&ff->fnum);
//		ff->f_res = f_write(&SDFile,temp_2,sizeof(temp_2),&ff->fnum);
//	}
	ff->f_res = f_close(&SDFile);
}

void ReadExpData(file_in_flash * ff)
{
	stop_all_IT();
	ff->exp_num = exp_num_temp;
	sprintf((char *)ReadBuffer_expinfo, "exp%03d.txt", ff->exp_num);
	//读测试--------------------------------------------------------------------
	ff->f_res = f_open(&SDFile,(char *)ReadBuffer_expinfo,FA_OPEN_EXISTING | FA_READ);
	//HAL_Delay(100);
	if(ff->f_res == FR_OK)
	{
		//Save data in extern buffer
//		ReadBuffer_expdata = (BYTE *)calloc(ff->file_size[ff->exp_num],sizeof(BYTE));
		//ff->f_res = f_read(&SDFile,ff->ReadBuffer,sizeof(ff->ReadBuffer),&ff->fnum);
		char r_temp[100];
		for(int i = 0;i < ((int)(ff->file_size[exp_num_temp]) / 100);i++){
			f_lseek(&SDFile,i * 100);
			f_gets(r_temp,100,&SDFile);
			printf("%s\n",r_temp);
			HAL_Delay(20);
		}
		
		//ff->f_res = f_read(&SDFile,ReadBuffer_expdata,sizeof(ReadBuffer_expdata),&ff->fnum);
	}
	//HAL_Delay(100);
	ff->f_res = f_close(&SDFile);
	//HAL_Delay(100);
	start_all_IT();
}

//Read and return the data buffer
//BYTE* ReadExpData(file_in_flash * ff)
//{
//	ff->exp_num = exp_num_temp;
//	sprintf((char *)ReadBuffer_expinfo, "exp%03d.txt", ff->exp_num);
//	//读测试--------------------------------------------------------------------
//	ff->f_res = f_open(&SDFile,(char *)ReadBuffer_expinfo,FA_OPEN_EXISTING | FA_READ);
//	//HAL_Delay(100);
//	BYTE *r_temp;
//	if(ff->f_res == FR_OK)
//	{
//		//Save data in extern buffer
//		//ReadBuffer_expdata = (BYTE *)ff_malloc((ff->file_size[ff->exp_num]) * sizeof(BYTE));
//		
//		//ff->f_res = f_read(&SDFile,ff->ReadBuffer,sizeof(ff->ReadBuffer),&ff->fnum);
//		ff->f_res = f_read(&SDFile,r_temp,sizeof(r_temp),&ff->fnum);
//	}
//	//HAL_Delay(100);
//	ff->f_res = f_close(&SDFile);
//	return r_temp;
//	//HAL_Delay(100);
//}

void CheckFreeSpace(file_in_flash * ff)
{
	ff->f_res = f_getfree("0:/", &ff->fre_clust, &ff->pfs);
	ff->totalSpace = (uint32_t)((ff->pfs->n_fatent - 2) * ff->pfs->csize * 0.5);
	ff->freeSpace = (uint32_t)(ff->fre_clust * ff->pfs->csize * 0.5);
}

void ScanFiles(file_in_flash * ff, char *path)
{
	DIR dir;
	static FILINFO fno;
	uint16_t temp_times = 0;
	ff->f_res = f_opendir(&dir,path); 
	
	if(ff->f_res == FR_OK) 
	{
		for(;;) 
		{
			ff->f_res = f_readdir(&dir, &fno); 
			if(ff->f_res != FR_OK || fno.fname[0] == 0) 
				break;
			if(fno.fattrib & AM_DIR)
			{
				ff->fnum = strlen(path); 
				sprintf(&path[ff->fnum],"/%s",fno.fname);
				//res = ScanFiles(path); 
				//if(res != FR_OK) break;
				//path[i] = 0; 
			}
			else
			{
				strcpy(ff->pathBuff[temp_times], fno.fname); 
				ff->file_size[temp_times++] = fno.fsize;
			}
		}
	}
	else
	{
	}
	f_closedir(&dir); 
}