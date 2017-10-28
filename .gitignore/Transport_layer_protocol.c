#include <stdio.h>
#include <pcap.h>
#include<arpa/inet.h>
int length=0;
struct ether_header
{
    u_int8_t ether_dhost[6];
    /*目的以太网地址*/
    u_int8_t ether_shost[6];
    /*源以太网地址*/
    u_int16_t ether_type;
    /*以太网类型*/
} ;

/*下面是IP地址格式*/
typedef u_int32_t in_addr_t;
/*struct in_addr
{
    in_addr_t s_addr;
} ;
*/
/*下面是IP协议格式*/
struct ip_header
{
    #ifdef WORDS_BIGENDIAN
        u_int8_t ip_version:4,/*IP协议版本*/
        ip_header_length:4;/*IP协议首部长度*/
    #else
        u_int8_t ip_header_length:4,ip_version:4;
    #endif
        u_int8_t ip_tos;
        /*TOS服务质量*/
        u_int16_t ip_length;
        /*总长度*/
        u_int16_t ip_id;
        /*标识*/
        u_int16_t ip_off;
        /*偏移*/
        u_int8_t ip_ttl;
        /*生存时间*/
        u_int8_t ip_protocol;
        /*协议类型*/
        u_int16_t ip_checksum;
        /*校验和*/
        struct in_addr ip_souce_address;
        /*源IP地址*/
        struct in_addr ip_destination_address;
        /*目的IP地址*/
} ;
struct udp_header
{
    u_int16_t udp_source_port;
    u_int16_t udp_destination_port;
    u_int16_t udp_length;
    u_int16_t udp_checksum;
};
void udp_protocol_packet_callback(u_char* argument,const struct pcap_pkthdr *packet_header,const u_char*packet_content)
{
    struct udp_header *udp_protocol;
    u_short source_port;
    u_short destination_port;
    u_short length;
    udp_protocol=(struct udp_header*)(packet_content+14+20);
   source_port=ntohs(udp_protocol->udp_source_port);
   destination_port=ntohs(udp_protocol->udp_destination_port);
   length=ntohs(udp_protocol->udp_length);
   printf("---------------UDP Protocol   (Transport Layer)---------------\n");
   printf("Source port:%d\n",source_port);
   printf("Dsetination port:%d\n",destination_port);
       switch(destination_port)/*判断协议类型的值*/
       {
            case 138:
               printf("NETBIOS Datagram Service\n");
           break;
       case 137:
              printf("NETBIOS Name Service\n");
       break;
       case 139:
           printf("NETBIOS Session Service\n");
           break;
       case 53:
           printf("name-domain Service\n");
       break;
       default:
           break;
       }
       printf("Length:%d\n",length);
       printf("Checksum:%d\n",ntohs(udp_protocol->udp_checksum));
}
void ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr
*packet_header,const u_char *packet_content)
{
    struct ip_header *ip_protocol;
    /*IP协议变量*/
    u_int header_length;
    /*长度*/
    u_int offset;
     /*偏移*/
    u_char tos;
    /*服务质量*/
    u_int16_t checksum;
    /*校验和*/
    printf("---------------IP Protocol   (Network Layer)---------------\n");
    ip_protocol=(struct ip_header*)(packet_content + 14);
        /*获得IP协议数据内容，去掉以太网头*/
        checksum=ntohs(ip_protocol->ip_checksum);
        /*获得校验和*/
        header_length=ip_protocol->ip_header_length *4;
        /*获得长度*/
        tos=ip_protocol->ip_tos;
        /*获得TOS*/
        offset = ntohs(ip_protocol->ip_off);
        /*获得偏移量*/
        printf("IP Version:%d\n",ip_protocol->ip_version);
        printf("Header length:%d\n",header_length);
        printf("TOS:%d\n",tos);
        printf("Total length:%d\n",ntohs(ip_protocol->ip_length));
        /*获得总长度*/
        printf("Identification:%d\n",ntohs(ip_protocol->ip_id));
        /*获得标识*/
        printf("Offset:%d\n",(offset &0x1fff) *8);
        printf("TTL:%d\n",ip_protocol->ip_ttl);
        /*获得TTL*/
        printf("Protocol:%d\n",ip_protocol->ip_protocol);
        /*获得该协议类型*/
        switch(ip_protocol->ip_protocol)/*判断协议类型的值*/
        {
        case 6:
            printf("The Transport Layer Protocol is TCP\n");
            break;
        /*如果协议类型是6，表示上层协议为TCP协议*/
        case 17:
            printf("The Transport Layer Protocol is ICMP\n");
            break;
        /*如果协议类型为17，表示上层协议为UDP协议*/
        case 1:
            printf("The Transport Layer Protocol is ICMP\n");
            break;
        /*如果协议类型为1，表示上层协议为ICMP协议*/
        default:
            break;

    }
        printf("Header checksum:%d\n",checksum);
        printf("Source address:%s\n",inet_ntoa(ip_protocol->ip_souce_address));
        /*获得源IP地址*/
        printf("Destination address:%s\n",inet_ntoa(ip_protocol->ip_destination_address));
        /*获得目的IP地址*/
        switch(ip_protocol->ip_protocol)
        {
        case 17:
            udp_protocol_packet_callback(argument,packet_header,packet_content);
            break;
        default:
            break;

        }
    }
void ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr
*packet_header,const u_char *packet_content)
{
    packet_header->ts.tv_sec;
    packet_header->ts.tv_usec;
    length+=packet_header->len;
    time_t t,t1;
    u_short ethernet_type;
    /*以太网类型*/
    t=time(NULL);
    struct ether_header *ethernet_protocol;
    /*以太网协议变量*/
    u_char *mac_string;
    static int packet_number=1;
    if(packet_number==1)
    {
        t1=t;
    }
    printf("************************************************************\n");
    printf("Time the packet is capture is %ld分,%ld秒\n",(t-t1)/60,(t-t1)%60);
    printf("The total length is %d\n",length);
    printf("The %d IP packet is captured.\n",packet_number);
    //printf("Time the packet is captured is %d:\n",t-t1);
    printf("---------------   Ehternet Protocol (Link Layer)  -------------\n");
    ethernet_protocol=(struct ether_header*)packet_content;
    /*获得以太网协议数据内容*/
    printf("Ethernet type is :\n");
    ethernet_type=ntohs(ethernet_protocol->ether_type);
    /*获得以太网类型*/
    printf("%04x\n",ethernet_type);
    switch(ethernet_type)/*判断以太网类型的值*/
    {
        case 0x0800:
            printf("The network layer is IP protocol\n");
            break;
            /*如果为0x800，表示上层协议为IP协议*/
        case 0x0806:
            printf("The network layer is ARP protocol\n");
            break;
            /*如果为0x806，表示上层协议为ARP协议*/
        case 0x0835:
            printf("The network layer is RARP protocol\n");
            break;
            /*如果为0x800，表示上层协议为RARP协议*/
            default:
                break;
    }
    printf("Mac Source Address is :\n");
    mac_string=ethernet_protocol->ether_shost;
    printf("%02x:%02x:%02x:%02x:%02x:%02x:\n",*mac_string,*(mac_string+1),*(mac_string+2),
    *(mac_string+3),*(mac_string+4),*(mac_string+5));
    /*获得源以太网地址*/
    printf("Mac Destination Address is : \n");
    mac_string=ethernet_protocol->ether_dhost;
    printf("%02x:%02x:%02x:%02x:%02x:%02x:\n",*mac_string,*(mac_string+1),*(mac_string+2),
    *(mac_string+3),*(mac_string+4),*(mac_string+5));
    /*获得目的以太网地址*/
    switch(ethernet_type)
    {
        case 0x0800:
            ip_protocol_packet_callback(argument,packet_header,packet_content);
            break;
            default:
                break;
    }
    printf("***************************************************************\n");
    packet_number++;
}

void main()
{
        pcap_t *pcap_handle;
        /*Libpcap句柄*/
        char error_content[PCAP_ERRBUF_SIZE];
        /*错误信息*/
        char *net_interface;
        /*网络接口*/
        struct bpf_program bpf_filter;
        /*bpf过滤规则*/
        char bpf_filter_string[]="udp";
        /*过滤规则字符串，此时表示本程序只是捕获IP协议的网络数据包*/
        bpf_u_int32 net_mask;
        /*网络掩码*/
        bpf_u_int32 net_ip;
        /*网络地址*/
        net_interface=pcap_lookupdev(error_content);
        /*获得网络接口*/
        pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content);
        /*获得网络地址和掩码*/
        pcap_handle=pcap_open_live(net_interface,BUFSIZ,1,0,error_content);
        /*打开网络接口*/
        pcap_compile(pcap_handle,&bpf_filter,bpf_filter_string,0,net_ip);
        /*编译bpf过滤规则*/
        if(pcap_datalink(pcap_handle)!=DLT_EN10MB)
            return ;
        pcap_loop(pcap_handle,10000,ethernet_protocol_packet_callback,NULL);
        /*注册回调函数，循环捕获数据包*/
        pcap_close(pcap_handle);
        /*关闭Libpcap操作*/
    }
