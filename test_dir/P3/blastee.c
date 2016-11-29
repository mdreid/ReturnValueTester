blaster2.c                                                                                          0000640 0051666 0051666 00000020114 13016074627 012440  0                                                                                                    ustar   reid-jr                         reid-jr                                                                                                                                                                                                                /*
 Malcolm Reid Jr., Minshan Tao
 CS 640
 Assignment 3

 blaster.c

 This code implements a UDP "blaster", which sends a specified number of
 specified length packets to a specified host. It also logs information about
 each packet that it sends.

 blaster -s <hostname> -p <port> -r <rate> -n <num> -q seq_no> -l <length>
 -c <echo>
*/

#include "shared.h"

//const int MAX_BUFFER = 51*1024;
extern int HEADER_BYTES;
extern double MAX_SEC;

/* This function takes an uint32_t representing the length of a packet. The
   function returns 1 if the packet length is smaller than MAX_PACKET_LENGTH,
   0 otherwise.
*/
int is_length_valid(uint32_t pkt_length) {
  uint32_t MAX_PACKET_LENGTH = 50*1024;
  return pkt_length < MAX_PACKET_LENGTH;
}

/* This function takes a pointer to the memory location in which the packet will
   be stored, a character representing the packet type ("D"=DATA, "E"="END", C=
   ECHO), and uint32_t for the sequence number, and an uint32_t for the
   packet length. The function formats the fields accordingly and stores the
   header at the specified memory location.
*/
void construct_header(char *ptr,char pkt_type,uint32_t seq_num,uint32_t pkt_length) {
  uint32_t *seq_num_ptr;
  uint32_t *pkt_length_ptr;

  set_pkt_type(ptr,pkt_type);

  //check_endian() returns 1 if little-endian, 0 if big-endian, convert if little-endian
  if(check_endian()){
  	seq_num = convert_endian(seq_num);
  	pkt_length = convert_endian(pkt_length);
  }
  
  seq_num_ptr = (uint32_t *)(ptr+1);
  *seq_num_ptr = seq_num;
  
  pkt_length_ptr = (uint32_t *)(ptr+5);
  *pkt_length_ptr = pkt_length;
}

/* This function takes two clock_t parameters, t1 and t2. It returns an int
   representing the difference between the two times (t2-t1) in ms.
*/
int diff_clock(clock_t t1, clock_t t2) {
  return ((double)(t2 - t1)/CLOCKS_PER_SEC) * 1000.00;
}

// double diff_clock(clock_t t1, clock_t t2) {
//   return (double)((t2 - t1)/CLOCKS_PER_SEC);
// }

/* This function takes an uint32_t representing the sequence number, an int
  representing the length of the payload, a char ptr that points to the
  start of the packet, and an int representing whether this is an echo
  packet. The function prints the sequence number and first four bytes of the
  packet's payload to standard output. The function does not return anything.
*/
void print_info(uint32_t seq_num,int payload_length, char *ptr, int is_echo) {
  if (is_echo) {
    printf("echo\t");
  }
  else {
    printf("\t");
  }
  printf("Sequence number: %u\t",seq_num);
  printf("First 4 bytes: ");
  //print4bytes(ptr+HEADER_BYTES, num_bytes);
  print4bytes(ptr+HEADER_BYTES, payload_length);
  printf("\n");
}

/* This is the main calling point into blaster. It takes arguments from command
line (see blaster.c header). This sends variable sized UDP packets based on the
command line parameters. Returns 0 on success.
*/
int main(int argc, char* argv[]){
  int udp_socket;
  // struct sockaddr_in my_addr;
  int opt;
  int cnt=0;
  char hostname[256];
  char port[256];
  void *packet;// = malloc(HEADER_BYTES);
  char *pt;// = (char *)packet;
  int rate;
  int num_packets;
  int int_port;
  struct sockaddr_in my_addr;
  uint32_t seq_number;
  uint32_t echo_seq_number;
  char * echo_seq_number_ptr;////////
  uint32_t length;
  int echo; //1 for true, 0 for false
  struct hostent * hstnm;
  struct addrinfo *addr_info;
  char output[4];
  clock_t proc_time;
  int increment_ms;
  //double increment_sec;
  int echo_bytes;
  socklen_t size_of_struct = sizeof(struct sockaddr);
  struct sockaddr foreign_addr;
  int echo_count = 0;
  time_t curr_time;
  time_t current_time;

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = 0; /////
  my_addr.sin_addr.s_addr = INADDR_ANY;
  while ((opt = getopt(argc, argv, "s:p:r:n:q:l:c:")) != -1) {
    switch (opt) {
      case 's':
              strcpy(hostname, optarg);
              //printf("hostname: %s\n", hostname);
              break;
      case 'p':
              strcpy(port, optarg);
              int_port = atoi(port);
              //printf("port: %s\n", port);
              break;
      case 'r':
              rate = atoi(optarg);
              //printf("rate: %d\n", rate);
              break;
      case 'n':
              num_packets = atoi(optarg);
              //printf("number of packets: %d\n", num_packets);
              break;
      case 'q':
              // this should be unsigned
              seq_number = (uint32_t)atoi(optarg);
              //printf("sequence number: %u\n", seq_number);
              break;
      case 'l':
              length = (uint32_t)atoi(optarg);
              //printf("length: %u\n", length);
              break;
      case 'c':
              echo = atoi(optarg);
              //printf("echo: %d\n", echo);
              break;
    }
  }

  if (!is_port_valid(int_port)) {
    perror("Invalid port number");
    exit(EXIT_FAILURE);
  }

  if (!is_length_valid(length)) {
    perror("Invalid packet length");
    exit(EXIT_FAILURE);
  }

  if (!is_echo_valid(echo)) {
    perror("Invalid echo value");
    exit(EXIT_FAILURE);
  }

  increment_ms = (1.00/rate) * 1000;
  //printf("Increment ms: %d\n",increment_ms);

  //increment_sec = (1.0/rate);

  // create a socket
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket == -1) {
    perror("something is wrong with creating socket!\n");
    exit(EXIT_FAILURE);
  }

  if (getaddrinfo(hostname,port,NULL,&addr_info) < 0){
     perror("something is wrong with getaddrinfo! \n");
     exit(EXIT_FAILURE);
   }

  //construct and send the packets
  packet = malloc((HEADER_BYTES + length)*sizeof(char));//packet is void*
  if(packet == NULL){
    perror("Error allocating memory by malloc!\n");
    exit(1);
  }
  pt = (char *)packet;
  proc_time = clock();
  while (cnt < num_packets) {
    if (diff_clock(proc_time,clock()) >= increment_ms) {
//    if (diff_clock(proc_time,clock()) > increment_sec)
      //time(&current_time);
      //printf("Time entered main packet sending/receiving loop: %s\n",ctime(&current_time));
      proc_time = clock();
      construct_header(pt,'D',seq_number,length);
      if (sendto(udp_socket, packet, (size_t)(HEADER_BYTES + length), 0, addr_info->ai_addr, sizeof(struct sockaddr)) == -1) {
       perror("something is wrong with sendto!\n");
       exit(EXIT_FAILURE);
      }
      cnt++;
      print_info(seq_number,length,pt,0);
      seq_number = seq_number + length;
    }
      if (echo) {
        //receive packet
        echo_bytes = recvfrom(udp_socket, packet, (size_t)(HEADER_BYTES + length),MSG_DONTWAIT , &foreign_addr, &size_of_struct); // change this size maybe?
         if (echo_bytes > 0) {
           //time(&current_time);
           //printf("Time echo Packet Received: %s\n",ctime(&current_time));
      //     printf("Here is the number of bytes we got: %d\n",echo_bytes);
           echo_count++;
           echo_seq_number = get_seq_number(packet);
           print_info(echo_seq_number,length,pt,1);
           echo_bytes = 0;
         }
        size_of_struct = sizeof(struct sockaddr);
      }
  }

  curr_time = time(NULL);
  //construct and send the end packet
  construct_header(pt,'E',seq_number,HEADER_BYTES);
  if (sendto(udp_socket, packet, (size_t)HEADER_BYTES, 0, addr_info->ai_addr, sizeof(struct sockaddr)) == -1) {
   perror("something is wrong with sendto!\n");
   exit(EXIT_FAILURE);
  }
  // if we're still waiting on echo pkts, wait some more
  while ((echo_count < num_packets) && echo) {
    if (difftime(time(NULL),curr_time) > MAX_SEC) {
      break;
    }
    echo_bytes = recvfrom(udp_socket, packet, (size_t)(HEADER_BYTES + length), MSG_DONTWAIT, &foreign_addr, &size_of_struct);
//time(&current_time);
    //printf("Time echo Packet Received: %s\n",ctime(&current_time));
    if(echo_bytes > 0){
    //  printf("Here are the number of bytes we got after: %d\n",echo_bytes);
      echo_count++;
      size_of_struct = sizeof(struct sockaddr);
      echo_seq_number = get_seq_number(packet);
      print_info(echo_seq_number,length,pt,1) ;
      echo_bytes=0;
    }
  }
  //printf("Echo count: %d\n",echo_count);   ////////////delete this before submission~~~~

   close(udp_socket);
   free(packet);
   //free(buffer);/////
   return 0;

}
                                                                                                                                                                                                                                                                                                                                                                                                                                                    blaster.c                                                                                           0000640 0051666 0051666 00000020114 13016071674 012355  0                                                                                                    ustar   reid-jr                         reid-jr                                                                                                                                                                                                                /*
 Malcolm Reid Jr., Minshan Tao
 CS 640
 Assignment 3

 blaster.c

 This code implements a UDP "blaster", which sends a specified number of
 specified length packets to a specified host. It also logs information about
 each packet that it sends.

 blaster -s <hostname> -p <port> -r <rate> -n <num> -q seq_no> -l <length>
 -c <echo>
*/

#include "shared.h"

//const int MAX_BUFFER = 51*1024;
extern int HEADER_BYTES;
extern double MAX_SEC;

/* This function takes an uint32_t representing the length of a packet. The
   function returns 1 if the packet length is smaller than MAX_PACKET_LENGTH,
   0 otherwise.
*/
int is_length_valid(uint32_t pkt_length) {
  uint32_t MAX_PACKET_LENGTH = 50*1024;
  return pkt_length < MAX_PACKET_LENGTH;
}

/* This function takes a pointer to the memory location in which the packet will
   be stored, a character representing the packet type ("D"=DATA, "E"="END", C=
   ECHO), and uint32_t for the sequence number, and an uint32_t for the
   packet length. The function formats the fields accordingly and stores the
   header at the specified memory location.
*/
void construct_header(char *ptr,char pkt_type,uint32_t seq_num,uint32_t pkt_length) {
  uint32_t *seq_num_ptr;
  uint32_t *pkt_length_ptr;

  set_pkt_type(ptr,pkt_type);

  //check_endian() returns 1 if little-endian, 0 if big-endian, convert if little-endian
  if(check_endian()){
  	seq_num = convert_endian(seq_num);
  	pkt_length = convert_endian(pkt_length);
  }
  
  seq_num_ptr = (uint32_t *)(ptr+1);
  *seq_num_ptr = seq_num;
  
  pkt_length_ptr = (uint32_t *)(ptr+5);
  *pkt_length_ptr = pkt_length;
}

/* This function takes two clock_t parameters, t1 and t2. It returns an int
   representing the difference between the two times (t2-t1) in ms.
*/
int diff_clock(clock_t t1, clock_t t2) {
  return ((double)(t2 - t1)/CLOCKS_PER_SEC) * 1000.00;
}

// double diff_clock(clock_t t1, clock_t t2) {
//   return (double)((t2 - t1)/CLOCKS_PER_SEC);
// }

/* This function takes an uint32_t representing the sequence number, an int
  representing the length of the payload, a char ptr that points to the
  start of the packet, and an int representing whether this is an echo
  packet. The function prints the sequence number and first four bytes of the
  packet's payload to standard output. The function does not return anything.
*/
void print_info(uint32_t seq_num,int payload_length, char *ptr, int is_echo) {
  if (is_echo) {
    printf("echo\t");
  }
  else {
    printf("\t");
  }
  printf("Sequence number: %u\t",seq_num);
  printf("First 4 bytes: ");
  //print4bytes(ptr+HEADER_BYTES, num_bytes);
  print4bytes(ptr+HEADER_BYTES, payload_length);
  printf("\n");
}

/* This is the main calling point into blaster. It takes arguments from command
line (see blaster.c header). This sends variable sized UDP packets based on the
command line parameters. Returns 0 on success.
*/
int main(int argc, char* argv[]){
  int udp_socket;
  // struct sockaddr_in my_addr;
  int opt;
  int cnt=0;
  char hostname[256];
  char port[256];
  void *packet;// = malloc(HEADER_BYTES);
  char *pt;// = (char *)packet;
  int rate;
  int num_packets;
  int int_port;
  struct sockaddr_in my_addr;
  uint32_t seq_number;
  uint32_t echo_seq_number;
  char * echo_seq_number_ptr;////////
  uint32_t length;
  int echo; //1 for true, 0 for false
  struct hostent * hstnm;
  struct addrinfo *addr_info;
  char output[4];
  clock_t proc_time;
  int increment_ms;
  //double increment_sec;
  int echo_bytes;
  socklen_t size_of_struct = sizeof(struct sockaddr);
  struct sockaddr foreign_addr;
  int echo_count = 0;
  time_t curr_time;
  time_t current_time;

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = 0; /////
  my_addr.sin_addr.s_addr = INADDR_ANY;
  while ((opt = getopt(argc, argv, "s:p:r:n:q:l:c:")) != -1) {
    switch (opt) {
      case 's':
              strcpy(hostname, optarg);
              //printf("hostname: %s\n", hostname);
              break;
      case 'p':
              strcpy(port, optarg);
              int_port = atoi(port);
              //printf("port: %s\n", port);
              break;
      case 'r':
              rate = atoi(optarg);
              //printf("rate: %d\n", rate);
              break;
      case 'n':
              num_packets = atoi(optarg);
              //printf("number of packets: %d\n", num_packets);
              break;
      case 'q':
              // this should be unsigned
              seq_number = (uint32_t)atoi(optarg);
              //printf("sequence number: %u\n", seq_number);
              break;
      case 'l':
              length = (uint32_t)atoi(optarg);
              //printf("length: %u\n", length);
              break;
      case 'c':
              echo = atoi(optarg);
              //printf("echo: %d\n", echo);
              break;
    }
  }

  if (!is_port_valid(int_port)) {
    perror("Invalid port number");
    exit(EXIT_FAILURE);
  }

  if (!is_length_valid(length)) {
    perror("Invalid packet length");
    exit(EXIT_FAILURE);
  }

  if (!is_echo_valid(echo)) {
    perror("Invalid echo value");
    exit(EXIT_FAILURE);
  }

  increment_ms = (1.00/rate) * 1000;
  //printf("Increment ms: %d\n",increment_ms);

  //increment_sec = (1.0/rate);

  // create a socket
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket == -1) {
    perror("something is wrong with creating socket!\n");
    exit(EXIT_FAILURE);
  }

  if (getaddrinfo(hostname,port,NULL,&addr_info) < 0){
     perror("something is wrong with getaddrinfo! \n");
     exit(EXIT_FAILURE);
   }

  //construct and send the packets
  packet = malloc((HEADER_BYTES + length)*sizeof(char));//packet is void*
  if(packet == NULL){
    perror("Error allocating memory by malloc!\n");
    exit(1);
  }
  pt = (char *)packet;
  proc_time = clock();
  while (cnt < num_packets) {
    if (diff_clock(proc_time,clock()) >= increment_ms) {
//    if (diff_clock(proc_time,clock()) > increment_sec)
      //time(&current_time);
      //printf("Time entered main packet sending/receiving loop: %s\n",ctime(&current_time));
      proc_time = clock();
      construct_header(pt,'D',seq_number,length);
      if (sendto(udp_socket, packet, (size_t)(HEADER_BYTES + length), 0, addr_info->ai_addr, sizeof(struct sockaddr)) == -1) {
       perror("something is wrong with sendto!\n");
       exit(EXIT_FAILURE);
      }
      cnt++;
      print_info(seq_number,length,pt,0);
      seq_number = seq_number + length;
    }
      if (echo) {
        //receive packet
        echo_bytes = recvfrom(udp_socket, packet, (size_t)(HEADER_BYTES + length),MSG_DONTWAIT , &foreign_addr, &size_of_struct); // change this size maybe?
         if (echo_bytes > 0) {
           //time(&current_time);
           //printf("Time echo Packet Received: %s\n",ctime(&current_time));
      //     printf("Here is the number of bytes we got: %d\n",echo_bytes);
           echo_count++;
           echo_seq_number = get_seq_number(packet);
           print_info(echo_seq_number,length,pt,1);
           echo_bytes = 0;
         }
        size_of_struct = sizeof(struct sockaddr);
      }
  }

  curr_time = time(NULL);
  //construct and send the end packet
  construct_header(pt,'E',seq_number,HEADER_BYTES);
  if (sendto(udp_socket, packet, (size_t)HEADER_BYTES, 0, addr_info->ai_addr, sizeof(struct sockaddr)) == -1) {
   perror("something is wrong with sendto!\n");
   exit(EXIT_FAILURE);
  }
  // if we're still waiting on echo pkts, wait some more
  while ((echo_count < num_packets) && echo) {
    if (difftime(time(NULL),curr_time) > MAX_SEC) {
      break;
    }
    echo_bytes = recvfrom(udp_socket, packet, (size_t)(HEADER_BYTES + length), MSG_DONTWAIT, &foreign_addr, &size_of_struct);
//time(&current_time);
    //printf("Time echo Packet Received: %s\n",ctime(&current_time));
    if(echo_bytes > 0){
    //  printf("Here are the number of bytes we got after: %d\n",echo_bytes);
      echo_count++;
      size_of_struct = sizeof(struct sockaddr);
      echo_seq_number = get_seq_number(packet);
      print_info(echo_seq_number,length,pt,1) ;
      echo_bytes=0;
    }
  }
  //printf("Echo count: %d\n",echo_count);   ////////////delete this before submission~~~~

   close(udp_socket);
   free(packet);
   //free(buffer);/////
   return 0;

}
                                                                                                                                                                                                                                                                                                                                                                                                                                                    Makefile                                                                                            0000664 0051666 0051666 00000000377 13016071674 012234  0                                                                                                    ustar   reid-jr                         reid-jr                                                                                                                                                                                                                all: blaster blastee
blaster: blaster.c shared.o shared.h
	gcc -o blaster blaster.c shared.o
shared.o: shared.c shared.h
	gcc -c shared.c
blastee: blastee.c shared.o shared.h
	gcc -o blastee blastee.c shared.o
clean:
	rm blaster blastee shared.o
                                                                                                                                                                                                                                                                 shared.c                                                                                            0000640 0051666 0051666 00000005153 13016071674 012175  0                                                                                                    ustar   reid-jr                         reid-jr                                                                                                                                                                                                                /*
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
                                                                                                                                                                                                                                                                                                                                                                                                                     shared.h                                                                                            0000640 0051666 0051666 00000001231 13016071674 012173  0                                                                                                    ustar   reid-jr                         reid-jr                                                                                                                                                                                                                #include <sys/socket.h>
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
                                                                                                                                                                                                                                                                                                                                                                       shared.o                                                                                            0000664 0051666 0051666 00000005050 13016071674 012213  0                                                                                                    ustar   reid-jr                         reid-jr                                                                                                                                                                                                                ELF          >                    �          @     @  
 UH��H��0H�}؉u��E�   �E�E��E�    H�E�H�E��E�;E�}"�EԉE��H�E�� �����    H�E��E��E�;E�|���UH��H�}����E�H�E��U���UH��}��}� t�}�u�   ��    ��UH��}��E�   �E�   �E�;E�~�E�;E�}�   ��    ��UH��SH��H�}�H��H���E�   �E�Hc�H��H�U�H�H��H��H��H��H)�H��H��H��H��H�E�H�E�H�E��E�    �#�u�E܃�+E�H�HE��H�U�Hcƈ�E��E�;E�|�H�EЋ H��H�]���UH��H�� H�}�H�E�H�E�H�E��    �    ��uH�E�� �H�E�� ���    ��UH���E�   H�E�� <������      @	    GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-16)              zR x�            _    A�CZ     <           A�CT      \       !    A�C\      |       3    A�Cn       �       �    A�CN��      �       >    A�Cy       �           A�CW        .symtab .strtab .shstrtab .rela.text .data .bss .rodata .comment .note.GNU-stack .rela.eh_frame                                                                                              @       �                                                   8	      H                           &                                                           ,                                                           1                                                          9      0                     .                             B                      :                                     W                     @                                    R                      �	      �                                                 @      a                                                    �      �         	                 	                      �      �                                                          ��                                                                                                                                                                          
                                                    _       +                      3     _              @     x       !       N     �       3       \     �       �       k     f      >       z     �              shared.c MAX_SEC HEADER_BYTES print4bytes putchar set_pkt_type is_echo_valid is_port_valid convert_endian get_seq_number check_endian  H             ���������            ���������            ��������                      @             _       `             x       �             �       �             �       �             f      �             �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              