#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/un.h>
#include <netdb.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/time.h>

#ifndef SHARED_BLASTING_H
#define SHARED_BLASTING_H

void print4bytes(char *str, int payload_length);

void set_pkt_type(char *ptr,char pkt_type);

int is_echo_valid(int echo_val);

int is_port_valid(int port_num);

uint32_t get_seq_number(void *packet);

int check_endian();

uint32_t convert_endian(uint32_t t);
#endif
