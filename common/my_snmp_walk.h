
#include "net-snmp/net-snmp-config.h"
#include "net-snmp/net-snmp-includes.h"
#include "net-snmp/utilities.h"


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>

#define NETSNMP_DS_WALK_INCLUDE_REQUESTED	        1
#define NETSNMP_DS_WALK_PRINT_STATISTICS	        2
#define NETSNMP_DS_WALK_DONT_CHECK_LEXICOGRAPHIC	3
#define NETSNMP_DS_WALK_TIME_RESULTS     	        4
#define NETSNMP_DS_WALK_DONT_GET_REQUESTED	        5
#define NETSNMP_DS_WALK_TIME_RESULTS_SINGLE	        6


#ifndef MY_SNMP_WALK_H
#define MY_SNMP_WALK_H

#ifndef MY_SNMP_VAL_H
#define MY_SNMP_VAL_H

typedef union {
	long	integer;
	
	u_char	string[256];
	u_char	bitstring[256];
	long	objid[256];
	
	float	floatVal;
	double	doubleVal;
	struct	counter64 *counter64;
}my_snmp_val;

#endif

#ifndef MY_OID_RESULT_H
#define MY_OID_RESULT_H

typedef struct my_oid_result {

	unsigned long	oid_name[128];   
	size_t	oid_name_length;
	
	size_t	objid_length;
	u_char	type;   
	size_t	val_len;
	my_snmp_val val;
	
}my_oid_result;
#endif


#ifndef GET_WALK_OID_VALUES_H
#define GET_WALK_OID_VALUES_H
void get_walk_oid_values(netsnmp_variable_list *vars, my_oid_result *oid_result);
#endif

void my_snmp_walk_perror(unsigned int ratval);
int  my_snmp_walk(const char *peername, const char *comity, const char *oid_argv, my_oid_result oid_results[], unsigned int *oid_results_nums);


#endif
