#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "database_netTopoAutoCatch.h"


/* Connect the netTopoAutoCatch mysql database */

Status Database_Connect(MYSQL *my_connection)
{
	if(my_connection==NULL)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[ERROR] database_netTopoAutoCatch: The parameter of the database connect function is invalid.\n");
#endif
		return DATABASE_INFEASIBLE;
	}
	mysql_init(my_connection);

	if(mysql_real_connect(my_connection, DB_IPADDR, DB_USER, DB_PASSWORD, DB_NAME, 0, NULL, 0))
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Connection netTopoAutoCatch DataBase Successful.\n");
		printf("[DEBUG] database_public: Waiting the netTopoAutoCatch DataBase Closed...\n");
#endif
		if(mysql_set_character_set(my_connection, "utf8")){ 
			fprintf(stderr ,"error , %s/n" ,mysql_error(my_connection) ) ; 
		}
		return DATABASE_OK;
	}
	else
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Connection netTopoAutoCatch DataBase Failure.\n");
#endif
		if(mysql_errno(my_connection))
		{
#ifdef DATABASE_DEBUG_MESSAGE
			fprintf(stderr,"[ERROR] database_public: Connection error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		}
		return DATABASE_ERROR;
	}

/*
	if(!mysql_real_connect(my_connection, DB_IPADDR, DB_USER, DB_PASSWORD, DB_NAME, 0, NULL, 0)) {  
	
        printf("Error connecting to Mysql!\n");  
    }else {  
        printf("Connected Mysql successful!\n");  
    }  
*/
}
 


/* DisConnect the NetWork_Management_DataInfo mysql database (New Version)*/
Status Database_DisConnect(MYSQL *my_connection)
{
	if(my_connection==NULL)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[ERROR] database_netTopoAutoCatch: The parameter of the database disconnect function is invalid.\n");
#endif
		return DATABASE_INFEASIBLE;
	}
	
	mysql_close(my_connection);
	
#ifdef DATABASE_DEBUG_MESSAGE
	printf("[DEBUG] database_netTopoAutoCatch: The DataInfo1 DataBase is Closed.\n");
#endif

	return DATABASE_OK;
}


/* ---------------------------------------------------------- */
/*                Router_Table   functions                    */
/* ---------------------------------------------------------- */


/*Insert information into Router table*/

Status Router_Info_Table_Insert(MYSQL *my_connection, RouterID RouterID, char *RouterName, 
								char *FlagIpAddr, unsigned int IpAddrTableNum, unsigned int SubnetVectorNum )
{

	int res;
	char sqlbuff[SQLBUFFLEN];

	
	sprintf(sqlbuff,"insert into Router(RouterID,RouterName,FlagIpAddr,IpAddrTableNum,SubnetVectorNum) values(%d,'%s','%s',%d,%d)",RouterID,RouterName,FlagIpAddr,IpAddrTableNum,SubnetVectorNum);

	res=mysql_query(my_connection,sqlbuff);
	if(!res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_netTopoAutoCatch: Insert %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	}
	else
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_netTopoAutoCatch: Insert error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
	
}

/* Functions:  Get RouterInfo */
Status Get_Router_Info(MYSQL *my_connection,ROUTER_INFO *router_info, unsigned int *router_info_nums, const unsigned int RouterID)

{
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	int count=0;
	int res,j;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"select RouterID,RouterName,FlagIpAddr,IpAddrTableNum,SubnetVectorNum from Router where RouterID=%d",RouterID);
	res=mysql_query(my_connection,sqlbuff);
	if(res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Select error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return	DATABASE_ERROR;
	}
	else
	{
		res_ptr=mysql_store_result(my_connection);
		if(res_ptr)
		{
#ifdef DATABASE_DEBUG_MESSAGE
			printf("[DEBUG] database_public: Retrieved %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));
#endif
			*router_info_nums=(unsigned int)mysql_num_rows(res_ptr);


			while((sqlrow=mysql_fetch_row(res_ptr)))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				printf("[DEBUG] database_public: Fetched data...\n");
#endif

				router_info[count].routerID=atoi(sqlrow[0]);
				
				router_info[count].routerName = (char*)malloc(sizeof(char) *64);
				strcpy(router_info[count].routerName,sqlrow[1]);
				router_info[count].flagIpAddr = (char*)malloc(sizeof(char) *16);
				strcpy(router_info[count].flagIpAddr,sqlrow[2]);
				router_info[count].ipAddrTableNum = atoi(sqlrow[3]);
				router_info[count].subnetVectorNum= atoi(sqlrow[4]);
				
				count++;
				
			}
			if(mysql_errno(my_connection))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				fprintf(stderr,"[ERROR] database_public: Retrieve error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
				return DATABASE_ERROR;
			}
			mysql_free_result(res_ptr);
		}
	
		return DATABASE_OK;
	}
}


/* delete in the Router table where RouterID = ? */
Status Delete_Router_Table_By_RouterID(MYSQL *my_connection,const int RouterID) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"delete from Router where RouterID = %d", RouterID);
	res = mysql_query(my_connection, sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Delete %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Delete error %d: %s\n", mysql_errno(my_connection), mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
}
/*update Router Table (int)*/
Status Update_Router_info_int(MYSQL *my_connection, unsigned int RouterID , char *Rowname,unsigned int value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Router set %s = '%d' where RouterID = '%d'", Rowname, value, RouterID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}
/*update Router Table (char)*/
Status Update_Router_info_char(MYSQL *my_connection, unsigned int RouterID , char *Rowname,char *value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Router set %s = '%s' where RouterID = '%d'", Rowname, value, RouterID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}


/* ---------------------------------------------------------- */
/*              IpAddrTable_Table   functions                 */
/* ---------------------------------------------------------- */

/*Insert information into IpAddrTable table*/
Status IpAddrTable_Info_Table_Insert(MYSQL *my_connection, RouterID RouterID, unsigned int IpAddrTableEntIfIndex, char *IpAddrTableEntIp, 
								char *IpAddrTableEntMask )
{

	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"insert into IpAddrTable(RouterID,IpAddrTableEntIfIndex,IpAddrTableEntIp,IpAddrTableEntMask) values(%d,%d,'%s','%s')",RouterID,IpAddrTableEntIfIndex,IpAddrTableEntIp,IpAddrTableEntMask);
	res=mysql_query(my_connection,sqlbuff);
	if(!res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_netTopoAutoCatch: Insert %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	}
	else
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_netTopoAutoCatch: Insert error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
	
}


/*Get IpAddrTable_Info*/
Status Get_IpAddrTable_Info(MYSQL *my_connection,IPADDRTABLE_INFO *ipaddrtable_info, unsigned int *ipaddrtable_info_nums, const unsigned int RouterID)
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	int count=0;
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"select RouterID,IpAddrTableEntIfIndex,IpAddrTableEntIp,IpAddrTableEntMask from IpAddrTable where RouterID=%d",RouterID);
	res=mysql_query(my_connection,sqlbuff);
	if(res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Select error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return	DATABASE_ERROR;
	}
	else
	{
		res_ptr=mysql_store_result(my_connection);
		if(res_ptr)
		{
#ifdef DATABASE_DEBUG_MESSAGE
			printf("[DEBUG] database_public: Retrieved %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));
#endif
			*ipaddrtable_info_nums=(unsigned int)mysql_num_rows(res_ptr);
			while((sqlrow=mysql_fetch_row(res_ptr)))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				printf("[DEBUG] database_public: Fetched data...\n");
#endif
				ipaddrtable_info[count].routerID=atoi(sqlrow[0]);
				ipaddrtable_info[count].ipAddrTableEntIfIndex = atoi(sqlrow[1]);
			//	ipaddrtable_info[count].ipAddrTableEntIp = (char*)malloc(sizeof(char) *16);
				strcpy(ipaddrtable_info[count].ipAddrTableEntIp,sqlrow[2]);
			//	ipaddrtable_info[count].ipAddrTableEntMask = (char*)malloc(sizeof(char) *16);
				strcpy(ipaddrtable_info[count].ipAddrTableEntMask,sqlrow[3]);
				count++;
			}
			if(mysql_errno(my_connection))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				fprintf(stderr,"[ERROR] database_public: Retrieve error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
				return DATABASE_ERROR;
			}
			mysql_free_result(res_ptr);
		}
	}
	return DATABASE_OK;
	
}

/* delete in the IpAddrTable table where RouterID = ? */
Status Delete_IpAddrTable_Table_By_RouterID(MYSQL *my_connection,const int RouterID) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"delete from IpAddrTable where RouterID = %d", RouterID);
	res = mysql_query(my_connection, sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Delete %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Delete error %d: %s\n", mysql_errno(my_connection), mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
}
/*update IpAddrTable Table (int)*/
Status Update_IpAddrTable_info_int(MYSQL *my_connection, unsigned int RouterID , char *Rowname,unsigned int value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update IpAddrTable set %s = '%d' where RouterID = '%d'", Rowname, value, RouterID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}
/*update IpAddrTable Table (char)*/
Status Update_IpAddrTable_info_char(MYSQL *my_connection, unsigned int RouterID , char *Rowname,char *value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Router set %s = '%s' where RouterID = '%d'", Rowname, value, RouterID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}

/* ---------------------------------------------------------- */
/*                 RouterConnect    functions                 */
/* ---------------------------------------------------------- */

/*Insert information into RouterConnect table*/
Status RouterConnect_Info_Table_Insert(MYSQL *my_connection, LocalRID LocalRID, PeerRID PeerRID, char *LocalRAddr, 
								char *PeerRAddr)
{

	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"insert into RouterConnect(LocalRID,PeerRID,LocalRAddr,PeerRAddr) values(%d,%d,'%s','%s',%d,%d)",LocalRID,PeerRID,LocalRAddr,PeerRAddr);
	res=mysql_query(my_connection,sqlbuff);
	if(!res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_netTopoAutoCatch: Insert %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	}
	else
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_netTopoAutoCatch: Insert error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
	
}


/*Get RouterConnect_Info*/
Status Get_RouterConnect_Info(MYSQL *my_connection,ROUTER_CONNECT_INFO *router_connect, unsigned int *routerconnect_info_nums, const unsigned int LocalRID)
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	int count=0;
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"select LocalRID,PeerRID,LocalRaddr,PeerRaddr,LocalRIfIndex,PeerRIfIndex from RouterConnect where LocalRID=%d",LocalRID);
	res=mysql_query(my_connection,sqlbuff);
	if(res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Select error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return	DATABASE_ERROR;
	}
	else
	{
		res_ptr=mysql_store_result(my_connection);
		if(res_ptr)
		{
#ifdef DATABASE_DEBUG_MESSAGE
			printf("[DEBUG] database_public: Retrieved %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));
#endif
			*routerconnect_info_nums=(unsigned int)mysql_num_rows(res_ptr);
			while((sqlrow=mysql_fetch_row(res_ptr)))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				printf("[DEBUG] database_public: Fetched data...\n");
#endif
				router_connect[count].localRID=atoi(sqlrow[0]);
				router_connect[count].peerRID=atoi(sqlrow[1]);
			//	router_connect[count].localRAddr = (char*)malloc(sizeof(char) *16);
				strcpy(router_connect[count].localRAddr,sqlrow[2]);
			//	router_connect[count].peerRAddr = (char*)malloc(sizeof(char) *16);
				strcpy(router_connect[count].peerRAddr,sqlrow[3]);
				router_connect[count].localRIfIndex=atoi(sqlrow[4]);
				router_connect[count].peerRIfIndex=atoi(sqlrow[5]);
				count++;
			}
			if(mysql_errno(my_connection))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				fprintf(stderr,"[ERROR] database_public: Retrieve error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
				return DATABASE_ERROR;
			}
			mysql_free_result(res_ptr);
		}
	}
	return DATABASE_OK;
	
}

/* delete in the RouterConnect table where LocalRID and PeerRID */
Status Delete_RouterConnect_Table_By_LocalRID_PeerRID(MYSQL *my_connection,const int LocalRID,const int PeerRID) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"delete from RouterConnect where LocalRID = %d and PeerRID = %d", LocalRID,PeerRID);
	res = mysql_query(my_connection, sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Delete %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Delete error %d: %s\n", mysql_errno(my_connection), mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
}
/*update RouterConnect Table (int)*/
Status Update_RouterConnect_info_int(MYSQL *my_connection, unsigned int LocalRID ,unsigned int PeerRID, char *Rowname,unsigned int value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update RouterConnect set %s = '%d' where LocalRID = '%d' and PeerRID = '%d'", Rowname, value, LocalRID,PeerRID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}
/*update RouterConnect Table (char)*/
Status Update_RouterConnect_info_char(MYSQL *my_connection, unsigned int LocalRID , unsigned int PeerRID, char *Rowname,char *value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Router set %s = '%s' where LocalRID = '%d' and PeerRID = '%d'", Rowname, value, LocalRID, PeerRID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}


/* ---------------------------------------------------------- */
/*                      Subnet    functions                   */
/* ---------------------------------------------------------- */

/*Insert information into Subnet table*/
Status Subnet_Info_Table_Insert(MYSQL *my_connection, RouterID RouterID, unsigned int SubnetIfIndex,char *SubnetIp, 
								char *SubnetMask,  unsigned int HostVectorNum)
{

	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"insert into Subnet(RouterID,SubnetIfIndex,SubnetIp,SubnetMask,HostVectorNum) values(%d,%d,'%s','%s',%d)",RouterID,SubnetIfIndex,SubnetIp,SubnetMask,HostVectorNum);
	res=mysql_query(my_connection,sqlbuff);
	if(!res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_netTopoAutoCatch: Insert %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	}
	else
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_netTopoAutoCatch: Insert error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
	
}


/*Get Subnet_Info*/
Status Get_Subnet_Info(MYSQL *my_connection,SUBNET_INFO *subnet_info, unsigned int *subnet_info_nums, const unsigned int RouterID)
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	int count=0;
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"select RouterID,SubnetIfIndex,SubnetIp,SubnetMask,HostVectorNum where RouterID=%d",RouterID);
	res=mysql_query(my_connection,sqlbuff);
	if(res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Select error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return	DATABASE_ERROR;
	}
	else
	{
		res_ptr=mysql_store_result(my_connection);
		if(res_ptr)
		{
#ifdef DATABASE_DEBUG_MESSAGE
			printf("[DEBUG] database_public: Retrieved %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));
#endif
			*subnet_info_nums=(unsigned int)mysql_num_rows(res_ptr);
			while((sqlrow=mysql_fetch_row(res_ptr)))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				printf("[DEBUG] database_public: Fetched data...\n");
#endif
				subnet_info[count].routerID=atoi(sqlrow[0]);
				subnet_info[count].subnetIfIndex=atoi(sqlrow[1]);
			//	subnet_info[count].subnetIp = (char*)malloc(sizeof(char) *16);
				strcpy(subnet_info[count].subnetIp,sqlrow[2]);
			//	subnet_info[count].subnetMask = (char*)malloc(sizeof(char) *16);
				strcpy(subnet_info[count].subnetMask,sqlrow[3]);
				subnet_info[count].hostVectorNum=atoi(sqlrow[4]);
				count++;
			}
			if(mysql_errno(my_connection))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				fprintf(stderr,"[ERROR] database_public: Retrieve error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
				return DATABASE_ERROR;
			}
			mysql_free_result(res_ptr);
		}
	}
		return DATABASE_OK;
	
}

/* delete in the Subnet table where RouterID and SubnetIfIndex */
Status Delete_IpAddrTable_Table_By_RouterID_SubnetIfIndex(MYSQL *my_connection,const int RouterID,int SubnetIfIndex) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"delete from IpAddrTable where RouterID = %d and SubnetIfIndex = %d", RouterID,SubnetIfIndex);
	res = mysql_query(my_connection, sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Delete %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Delete error %d: %s\n", mysql_errno(my_connection), mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
}
/*update Subnet Table (int)*/
Status Update_Subnet_info_int(MYSQL *my_connection, unsigned int RouterID ,int SubnetIfIndex, char *Rowname,unsigned int value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Subnet set %s = '%d' where RouterID = '%d' and SubnetIfIndex = %d", Rowname, value, RouterID,SubnetIfIndex);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}
/*update Subnet Table (char)*/
Status Update_Subnet_info_char(MYSQL *my_connection, unsigned int RouterID ,int SubnetIfIndex, char *Rowname,char *value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Subnet set %s = '%s' where RouterID = '%d' and SubnetIfIndex = %d", Rowname, value, RouterID,SubnetIfIndex);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}


/* ---------------------------------------------------------- */
/*                        Host    functions                   */
/* ---------------------------------------------------------- */

/*Insert information into Host table*/
Status Host_Info_Table_Insert(MYSQL *my_connection, unsigned int HostID, unsigned int SubnetIfIndex,char *HostName, unsigned int HostType,char *HostIpAddr,  char *HostAddrMask)
{

	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"insert into Host(HostID,SubnetIfIndex,HostName,HostType,HostIpAddr,HostAddrMask) values(%d,%d,'%s',%d,'%s','%s')",HostID,SubnetIfIndex,HostName,HostType,HostIpAddr,HostAddrMask);
	res=mysql_query(my_connection,sqlbuff);
	if(!res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_netTopoAutoCatch: Insert %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	}
	else
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_netTopoAutoCatch: Insert error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
	
}

/*Get Host_Info*/
Status Get_Host_Info(MYSQL *my_connection,HOST_INFO *host_info, unsigned int *host_info_nums, const unsigned int SubnetIfIndex)
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	int count=0;
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"select HostID,SubnetIfIndex,HostName,HostType,HostIpAddr,HostAddrMask where SubnetIfIndex=%d",SubnetIfIndex);
	res=mysql_query(my_connection,sqlbuff);
	if(res)
	{
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Select error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return	DATABASE_ERROR;
	}
	else
	{
		res_ptr=mysql_store_result(my_connection);
		if(res_ptr)
		{
#ifdef DATABASE_DEBUG_MESSAGE
			printf("[DEBUG] database_public: Retrieved %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));
#endif
			*host_info_nums=(unsigned int)mysql_num_rows(res_ptr);
			while((sqlrow=mysql_fetch_row(res_ptr)))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				printf("[DEBUG] database_public: Fetched data...\n");
#endif
			//	host_info[count].hostID = (char*)malloc(sizeof(char) *64);
				strcpy(host_info[count].hostID,sqlrow[0]);
				host_info[count].parentIfIndex=atoi(sqlrow[1]);
			//	host_info[count].hostName = (char*)malloc(sizeof(char) *64);
				strcpy(host_info[count].hostName,sqlrow[2]);
				host_info[count].hostType=atoi(sqlrow[3]);
			//	host_info[count].hostIpAddr = (char*)malloc(sizeof(char) *16);
				strcpy(host_info[count].hostIpAddr,sqlrow[4]);
			//	host_info[count].hostAddrMask = (char*)malloc(sizeof(char) *16);
				strcpy(host_info[count].hostAddrMask,sqlrow[5]);
				count++;
			}
			if(mysql_errno(my_connection))
			{
#ifdef DATABASE_DEBUG_MESSAGE
				fprintf(stderr,"[ERROR] database_public: Retrieve error %d: %s\n",mysql_errno(my_connection),mysql_error(my_connection));
#endif
				return DATABASE_ERROR;
			}
			mysql_free_result(res_ptr);
		}
	}
	return DATABASE_OK;
	
}

/* delete in the Host table where RouterID = ? */
Status Delete_Host_Table_By_RouterID(MYSQL *my_connection,const int HostID) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"delete from Host where HostID = %d", HostID);
	res = mysql_query(my_connection, sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Delete %lu rows\n",(unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Delete error %d: %s\n", mysql_errno(my_connection), mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}
}

//update Host Table (int)
Status Update_Host_info_int(MYSQL *my_connection, unsigned int HostID , char *Rowname,unsigned int value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Host set %s = '%d' where HostID = '%d'", Rowname, value, HostID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;
	}

}

//update Host Table (char)
Status Update_Host_info_char(MYSQL *my_connection, unsigned int HostID , char *Rowname,char *value) {
	int res;
	char sqlbuff[SQLBUFFLEN];
	sprintf(sqlbuff,"update Host set %s = '%s' where HostID = '%d'", Rowname, value, HostID);
	res = mysql_query(my_connection,sqlbuff);                                                                                                                  
	if(!res) {
#ifdef DATABASE_DEBUG_MESSAGE
		printf("[DEBUG] database_public: Update %lu rows\n", (unsigned long)mysql_affected_rows(my_connection));
#endif
		return DATABASE_OK;
	} else {
#ifdef DATABASE_DEBUG_MESSAGE
		fprintf(stderr,"[ERROR] database_public: Update error %d: %s\n", mysql_errno(my_connection),mysql_error(my_connection));
#endif
		return DATABASE_ERROR;

	}

}


/*
int main(){

	MYSQL mysql;
	Status i,j;

	i=Database_Connect(&mysql);
//	ROUTER_INFO router_info[64];
	int router_info_nums=0;
	Status router_info_insert=Router_Info_Table_Insert(&mysql,1,"Lao","192.168.200.2",NULL,NULL);
//	Status get_router_info=Get_Router_Info(&mysql,router_info,&router_info_nums,0);
//	printf("%d,%s,%s",router_info[0].routerID,router_info[0].routerName,router_info[0].flagIpAddr);
//        Status delete_router_table_by_routerID=Delete_Router_Table_By_RouterID(&mqsql,0);


	IPADDRTABLE_INFO ipAddrTable_info[64];
	int ipAddrTable_info_nums=0; 
	Status ipAddr_info_insert=IpAddrTable_Info_Table_Insert(&mysql,0,001,"192.0.0.0","255.255.255.0");
	Status get_ipaddr_info=Get_IpAddrTable_Info(&mysql,ipAddrTable_info,&ipAddrTable_info_nums,0);	
	printf("%d,%d,%s,%s",ipAddrTable_info[0].routerID,ipAddrTable_info[0].ipAddrTableEntIfIndex,ipAddrTable_info[0].ipAddrTable_info,ipAddrTable_info[0].ipAddrTableEntMask);


//	Status update_router_info=Update_Router_info_int(&mysql, 1 , "RouterID",9) ;
//	Status update_router_char=Update_Router_info_char(&mysql, 9 , "FlagIpAddr","199.192.100.100");
	j=Database_DisConnect(&mysql);
	return 	0;
}
*/
