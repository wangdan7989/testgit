#!/bin/bash

gcc my_snmp_bulkwalk.c -o walk -I ../netsnmp/include/net-snmp -L /usr/lib/netsnmp -lnetsnmp 
