#include "my_snmp_bulkwalk.h"
#include <stdio.h>

#define	        Arp_Dyn_Table                           ".1.3.6.1.4.1.2011.5.25.123.1.17.1"
#define         Arp_Dyn_MacAdd                          ".1.3.6.1.4.1.2011.5.25.123.1.17.1.11"
#define         Arp_Dyn_VlanId                          ".1.3.6.1.4.1.2011.5.25.123.1.17.1.12"
#define         Arp_Dyn_OutIfIndex                      ".1.3.6.1.4.1.2011.5.25.123.1.17.1.14"
#define         Arp_Dyn_ExpireTime                      ".1.3.6.1.4.1.2011.5.25.123.1.17.1.15"

#ifndef	IP_ArpDyn_TABLE_INFO_DEFINE
#define IP_ArpDyn_TABLE_INFO_DEFINE
typedef struct IP_ArpDyn_TABLE_INFO {
	unsigned int SwitchId;
	unsigned long ArpDynIfIndex;
	unsigned char ArpDynIpAdd[16];
	unsigned char ArpDynMacAdd[20];
	unsigned long ArpDynVrf;
	unsigned long  ArpDynVlanId;
	unsigned long  ArpDynOutIfIndex;
	unsigned long  ArpDynExpireTime;

	
	
}IP_ArpDyn_TABLE_INFO;
#endif

#ifndef 	MAX_MY_OID_RESULT_LEN_DEFINE
#define		MAX_MY_OID_RESULT_LEN		32
#endif

int get_ArpDyn_table_info_from_device_by_snmp(const unsigned char *ip, 
											   const unsigned char *comm,
											   IP_ArpDyn_TABLE_INFO *ip_arpdyn_table_info,
											   unsigned int *ip_arpdyn_table_rows);
