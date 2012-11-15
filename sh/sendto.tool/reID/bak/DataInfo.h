/*
 * =================================================================
 *
 *       Filename:  DataInfo.h
 *
 *    Description:  datainfo for isend
 *
 *        Version:  1.0
 *        Created:  08/15/2012  3:02:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *        Company:  auto@ustc
 *
 * =================================================================
 */

#ifndef _DATAINFO_H_
#define _DATAINFO_H_

#include "interface.h"


typedef struct
{
	uint32_t srcIP;
	uint32_t dstIP;
	uint16_t srcPort;
	uint16_t dstPort;
	uint8_t  priority;
}isend_IPInfo_t;


/***************************************************/

int GetStreamID(isend_IPInfo_t* pIPInfo);

int ReleaseStreamID(uint64_t streamid); 

/***************************************************/

int InitDataInfo(isend_SendDataInfo_t* pSendDataInfo,uint64_t streamid);

int PushData(isend_SendDataInfo_t* pSendDataInfo,char *buf,int bufsize);

int SendData(isend_SendDataInfo_t* pSendDataInfo);

int GetPort();//???
#endif
