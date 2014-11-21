/*
 * =================================================================
 *
 *       Filename:  DataInfo.c
 *
 *    Description:  interface of iSend
 *					include:
 *						1,apply and release streamid,and manager streamid
 *						2,push data to DataInfo
 *						3,Send DataInfo to iSend
 *						4,Init DataInfo
 *
 *        Version:  1.0
 *        Created:  08/15/2012  3:14:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *        Company:  auto@ustc
 *
 * =================================================================
 */

#include "DataInfo.h"

#define SEND_TIME_INTERVAL	40
#define INTERVAL_CYCLE		800000*SEND_TIME_INTERVAL
#define IP_TYPE				4

#define PAYLOAD_SIZE		1316

#define BANDWIDTH	38
#define IP_TYPE		4
#define ACTION		0

/********************************************************************/
#include <bitset>

std::bitset<240>	streamidflag;// all default to 0
int streamidtoken=0;

inline int StreamIDtoN(int freq, int streamtype, int iptype)				{ return 10*freq+streamtype*5+iptype; } 
inline int NtoStreamID(int n,uint64_t& freq, uint8_t& streamtype, uint8_t& iptype){
	freq=n/10;
	n=n%10;
	streamtype=n/5;
	n=n%5;
	iptype=n;
	return 0;
}
inline void setStreamIDFlag(int freq,int streamtype,int iptype,int val)		{ streamidflag.set(StreamIDtoN(freq,streamtype, iptype),1); }
inline void setStreamIDFlag(int n)											{ streamidflag.set(n,1); }
inline void unsetStreamIDFlag(int freq, int streamtype, int iptype)			{ streamidflag.set(StreamIDtoN(freq,streamtype, iptype),0); }
inline void unsetStreamIDFlag(int n)										{ streamidflag.set(n,0); }
inline int getStreamIDFlag(int freq, int streamtype, int iptype)			{ return streamidflag[StreamIDtoN(freq,streamtype, iptype)]; }
inline int getStreamIDFlag(int token)										{ return streamidflag[token]; }
// interface to get StreamID
int getStreamIDtoken(uint64_t &freq, uint8_t &streamtype, uint8_t &iptype){
	int ret=1;
	while(1){// loop to find a blank streamid
		ret=getStreamIDFlag(streamidtoken);
		if(ret==0)
			break;
		streamidtoken++;
	}
	setStreamIDFlag(streamidtoken);//set
	NtoStreamID(streamidtoken,freq, streamtype, iptype);
	streamidtoken++;
	if(streamidtoken==240){
		streamidtoken=0;
	}
	return 0;
}
// interface to release StreamID
void releaseStreamIDtoken(uint64_t id){
	unsetStreamIDFlag((id&&0xFF0000)>>16,(id&&0xFF00)>>8,(id&&0xFF));
}

isend_StreamInfo_t __GetIPv4L3Header(uint32_t srcIP, uint32_t dstIP, uint16_t srcPort,uint16_t dstPort, uint8_t IP_type, 
		uint64_t frequenceID, uint8_t priority, uint64_t bandwidth, uint8_t streamType, uint8_t action )
{
	isend_StreamInfo_t IPv4L3Header;
	memset(IPv4L3Header.u, 0, sizeof(isend_StreamInfo_t));
	IPv4L3Header.s.srcIP       = srcIP;
	IPv4L3Header.s.dstIP       = dstIP;
	IPv4L3Header.s.srcPort     = srcPort;
	IPv4L3Header.s.dstPort     = dstPort;
	IPv4L3Header.s.IP_type     = IP_type;
	IPv4L3Header.s.frequenceID = frequenceID;
	IPv4L3Header.s.priority    = priority;
	IPv4L3Header.s.bandwidth   = bandwidth;
	IPv4L3Header.s.streamType  = streamType;
	IPv4L3Header.s.action      = action;
	return IPv4L3Header;
}

int64_t __Get_Streams_ID(uint32_t srcIP,uint16_t srcPort,uint32_t dstIP,uint16_t dstPort,uint8_t priority)
{
	isend_StreamInfo_t IPv4L3Header;
	uint8_t IP_type=IP_TYPE;
	uint64_t bandwidth=BANDWIDTH;
	uint8_t action=ACTION;

	uint64_t frequenceID;
	uint8_t streamType;
	uint8_t iptype;
	getStreamIDtoken(frequenceID,streamType, iptype);
	memset(&IPv4L3Header,0,sizeof(IPv4L3Header));
	IPv4L3Header=__GetIPv4L3Header(srcIP,dstIP,srcPort,dstPort,IP_type,frequenceID,priority,bandwidth,streamType,action);
	int ret=Prepare_Send_Stream(&IPv4L3Header);
	if(ret<0){
		return -1;
	}
	return 0;
}

/********************************************************************/

int GetStreamID(isend_IPInfo_t* pIPInfo)
{
	int64_t ret=__Get_Streams_ID(pIPInfo->srcIP,pIPInfo->srcPort,pIPInfo->dstIP,pIPInfo->dstPort,pIPInfo->priority);
	return ret;
}

int ReleaseStreamID(uint64_t streamid){
	int ret=Free_Stream(streamid);
	releaseStreamIDtoken(streamid);
	return ret;
}

int InitDataInfo(isend_SendDataInfo_t* pSendDataInfo,uint64_t streamid)
{
	pSendDataInfo->streamID           = streamid;
	pSendDataInfo->Send_Time_Interval = SEND_TIME_INTERVAL;
	pSendDataInfo->sent_count         = 0;
	pSendDataInfo->used_count         = 0;
	pSendDataInfo->total_count        = 0;
	pSendDataInfo->Buffer_Head        = (uint64_t)NULL;
	pSendDataInfo->Buffer_End         = (uint64_t)NULL;
	return 0;
}
int ClearDataInfo(isend_SendDataInfo_t* pSendDataInfo)
{
	pSendDataInfo->used_count  = 0;
	pSendDataInfo->total_count = 0;
	pSendDataInfo->Buffer_Head = (uint64_t)NULL;
	pSendDataInfo->Buffer_End  = (uint64_t)NULL;
	return 0;
}

int PushData(isend_SendDataInfo_t* pSendDataInfo,char *buf,int bufsize)
{
	assert(bufsize>0);

	// apply for buffer
	int reqNum=bufsize/PAYLOAD_SIZE;
	if(bufsize%PAYLOAD_SIZE==0)
		reqNum++;
	Get_Buffer_Result gbRet=Get_Buffer(reqNum);
	if(gbRet.head==(uint64_t)NULL){
		return 1;
	}else{
		if(gbRet.count!=reqNum){
			Release_Buffer((LS_Packet*)gbRet.head);
			return 2;
		}
	}

	// copy data to buffer
	isend_Packet_t* head,*tail;
	head=(isend_Packet_t*)gbRet.head;
	tail=(isend_Packet_t*)gbRet.head;
	while(bufsize>PAYLOAD_SIZE){
		tail->IPtype=IP_TYPE;
		tail->payLoadLen=PAYLOAD_SIZE;	
		memcpy(tail->ipPacket.pkt.payLoad,buf,PAYLOAD_SIZE);
		buf+=PAYLOAD_SIZE;
		bufsize-=PAYLOAD_SIZE;

		tail=(isend_Packet_t*)tail->pktNext;//!!
	}
	tail->IPtype=IP_TYPE;
	tail->payLoadLen=bufsize;
	memcpy(tail->ipPacket.pkt.payLoad,buf,bufsize);
	tail=(isend_Packet_t*)tail->pktNext;

	//  link buffer queue to pSendDataInfo
	if(pSendDataInfo->Buffer_Head==(uint64_t)NULL){
		pSendDataInfo->Buffer_Head = (uint64_t)head; // first add to pSendDataInfo
		pSendDataInfo->Buffer_End  = (uint64_t)tail;
	}else{
		isend_Packet_t* endPtr=(isend_Packet_t*)pSendDataInfo->Buffer_End;
		endPtr->pktNext = (uint64_t)head; // add to pSendDataInfo
		pSendDataInfo->Buffer_End         = (uint64_t)tail;
	}

	return 0;
}

int SendData(isend_SendDataInfo_t* pSendDataInfo)
{
	if(pSendDataInfo->Buffer_Head==(uint64_t)NULL||pSendDataInfo->total_count==0||pSendDataInfo->used_count==0){
		return 0;
	}
	int ret=Send_Data(pSendDataInfo);
	ClearDataInfo(pSendDataInfo);	
	return ret;//todo???!!!
}


/*************************************************************/
//???
int port=2000;
int GetPort(){
	if(port>20000){
		port=10000;
	}else{
		port++;
	}
	return port;
}
