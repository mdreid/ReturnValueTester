/*
 Malcolm Reid Jr., Minshan Tao
 CS 640
 Assignment 3

 shared.c

 This code implements some shared helper functions used by blaster.c and blastee.c
*/

#include "shared.h"

const double MAX_SEC = 5.0;
const int HEADER_BYTES = 9;

/* This function takes a char pointer to the packet's payload and an int length
   representing the length of the packet payload. The function prints the first four
   bytes (as ASCII characters) to standard output. The function does not return
   anything.
*/
void print4bytes(char *str, int payload_length){
  int MAX_BYTES = 4;
  int num_bytes = MAX_BYTES;
  int i = 0;
  char* temp = str;
  if (payload_length < MAX_BYTES) {
    num_bytes = payload_length;
  }
  for(; i < num_bytes; i++){
    printf("%c",*temp);  //use %d instead of %c to get the ASCII????
    temp++;
  }
}
/* This function takes a char pointer to the packet type field of the packet and
   a character representing the packet type ("D"=DATA, "E"=END, "C"=ECHO). This
   function sets the paket's packet type field to the type. This function does
   not return anything.
*/
void set_pkt_type(char *ptr,char pkt_type) {
  ptr[0] = pkt_type;
}

/* This function takes a value for the command line parameter echo for both
   blaster and blastee. Returns 1 if echo is either 0 or 1 (i.e. a valid value),
   0 otherwise.
*/
int is_echo_valid(int echo_val) {
  return (echo_val==0 || echo_val==1);
}

/* This function takes an int representing a port_number. The function returns
   1 if the port number is between MIN_PORT_NUM and MAX_PORT_NUM, 0 otherwise.
*/
int is_port_valid(int port_num) {
  int MIN_PORT_NUM = 1024;
  int MAX_PORT_NUM = 65536;
  return ((port_num > MIN_PORT_NUM) && (port_num < MAX_PORT_NUM));
}

/* Converts endianness */
uint32_t convert_endian(uint32_t t) {
  int size_of_unsigned = sizeof(uint32_t);
  char array[size_of_unsigned];
  char *ptr;
  int i;
  ptr = (char *)&t;
  for (i = 0; i < size_of_unsigned; i++) {
    array[i] = ptr[size_of_unsigned-1-i]; 
  }
  return *((uint32_t *)array);
}

/* This function takes a void pointer to a packet and returns an uint32_t
   representing the sequence number in that packet's sequence number field.
*/
uint32_t get_seq_number(void *packet) {
  char *echo_seq_number_ptr;
  char array[4];
  echo_seq_number_ptr = (char*)packet;
  echo_seq_number_ptr++;  
  if (!check_endian()) return *((uint32_t *)echo_seq_number_ptr);
  return convert_endian(*((uint32_t*)echo_seq_number_ptr));
}

/* Returns 1 if little-endian, 0 if big-endian */
int check_endian() {
  int num=1;
  return (*(char *)&num == 1);
}
