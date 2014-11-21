//	liuyang1,2011-11-16 22:58:04
//	using libpcap tp capture packet
//	using opendpi to filter http packet
//	my filter content of html doc
//
//	WINlab@ustc

#include <pcap.h>		//libpcap hdr file
#include <ipq_api.h>	//opendpi hdr file
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>

#ifdef __linux__
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#else
#error "must port to this OS"
#endif
#define PCAP_NUM	-1

static struct ipoque_detection_module_struct *ipoque_struct=NULL;
static u32 detection_tick_resolution=1000;

// custom my memory malloc&free function
static void *malloc_wrapper(unsigned long size){
	return malloc(size);
}
static void free_wrapper(void* freeable){
	free(freeable);
}

int init_dpi(){
	/// init dpi dection module
	//	param1:	dpi tick time frquency ( times per second)
	//	param2:	custom malloc wrapper function
	//	param3:	defaut to NULL
	//	return:	ipoque_struct
	ipoque_struct = ipoque_init_detection_module(detection_tick_resolution,malloc_wrapper,NULL);

	IPOQUE_PROTOCOL_BITMASK	http;
	IPOQUE_BITMASK_SET_ALL(http);//TODO modify all protocol to http protocol BITMASK
	ipoque_set_protocol_detection_bitmask2(ipoque_struct,&http);

	return 0;
}

// terminate opendpi module
int term_dpi(){
	ipoque_exit_detection_module(ipoque_struct,free_wrapper);
	return 0;
}

void packet_processing(const u64 time,const struct iphdr *iph,u16 ipsize,u16 rawsize){

	//detect http packet
	struct ipoque_id_struct *src=NULL,*dst=NULL;
	void *flow=NULL;
	unsigned int protocol;

	u32 src_ip,dst_ip;
	if(iph->version == 4 && ipsize>= sizeof(struct iphdr)){
		src_ip=iph->saddr;
		dst_ip=iph->daddr;
	}
	//TODO get_id to add func
	src=get_id(&src_ip);
	dst=get_id(&dst_ip);

	/// detection process 
	//	param1:	ipoque_struct
	//	param2:	flow
	//	param3:	pakcet ptr
	//	param4:	packet length
	//	param5:	packet timestamp
	//	param6:	src subscriber
	//	param7:	dst sbuscriber
	protocol = ipoque_detection_process_packet(ipoque_struct,flow,iph,ipsize,
			time,src,dst);
	static const char *protocol_long_str[]={IPOQUE_PROTOCOL_LONG_STRING};
	printf("protocol: %s\n",protocol_long_str[protocol]);
}

void my_pcap_handler(u_char *arg,const struct pcap_pkthdr *pkthdr,const u_char *packet){
	//calc timestamp
	u64	time;
	static u64 lasttime=0;
	time = ((u64)pkthdr->ts.tv_sec)*detection_tick_resolution
		+
		pkthdr->ts.tv_usec/(1000000/detection_tick_resolution);
	if(lasttime>time)
		time=lasttime;
	lasttime=time;
	
//	int *cnt=(int *)arg;
	//printf("Pakcet cnt: %d\n",++(*cnt));
	//printf("Packet size: %d\n",pkthdr->len);
	/*
	printf("Payload:\n");
	unsigned char i=0;
	for(i=0;i<pkthdr->len;i++){
		if(isprint(packet[i]))
			printf("%c ",packet[i]);
		else
			printf(". ");
		if(i%16==15||i==pkthdr->len-1)
			printf("\n");
	}
	*/
	
	// basic packet info
	// args to cnt No. of packet seq
	// packet length:	pkthdr->len
	// payload:			*packet
	// header of ethernet
	const struct ethhdr *ethernet=(struct ethhdr *)packet;
	// header of ip level
	struct iphdr *iph = (struct iphdr*)&packet[sizeof(struct ethhdr)];
	u16	type;
	type=ethernet->h_proto;
	if(type==htons(ETH_P_IP))
		printf("type: eth ip\n");
	if(pkthdr->caplen < pkthdr->len){
		printf("capture size is smaller than packet size\n");
	}else{
		// invoke packet processing func
		packet_processing(time,iph,pkthdr->len-sizeof(struct ethhdr),pkthdr->len);
	}
}

int my_pcap(){
	char *devname;
	char errbuf[PCAP_ERRBUF_SIZE];
	devname=pcap_lookupdev(errbuf);
	printf("opening device %s\n",devname);

	int snaplen=1518;
	/// open & live dev to cap packet
	//	param1:	dev name
	//	param2:	lenght of snap packet
	//	param3:	is MISC mode,0 not,other yes
	//			this is determinated by network device setting first
	//  param4:	error buffer
	//  return:	ptr to pcap_t
	pcap_t *p_pcap=NULL;
	p_pcap = pcap_open_live(devname,snaplen,0,10,errbuf);
	if(p_pcap==NULL){
		printf("[error]: %s\n",errbuf);
		exit(-1);
	}

	///	loop tp pcap
	//	param1:	ptr to pcap_t
	//	param2:	cnt num packet to capture
	//			-1,only ocurr error to exit
	//	param3:	handler packet function ptr
	//	param4: user?
	int cnt;
	pcap_loop(p_pcap,PCAP_NUM,my_pcap_handler,(u_char*)&cnt);

	return 0;
}

int main(){
	init_dpi();
	my_pcap();
	term_dpi();
	return 0;
}
