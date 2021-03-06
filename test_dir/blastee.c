/*
 Malcolm Reid Jr., Minshan Tao
 CS 640
 Assignment 3

 blastee.c

 This code implements a UDP "blastee", which receive packets the blaster sends,
 subsequently logging them and printing receipt information.

 blastee -p <port> -c <echo>
*/

#include "shared.h"

const int MAX_BUFFER = 51*1024;
extern double MAX_SEC;
extern int HEADER_BYTES;

/* This function prints summary information about the test including total packets
   received, total bytes received, average packets/second, average bytes/second,
   and duration of the test.
*/
void print_summary(int total_pkts,int total_bytes, double duration_sec) {
  /* TO DO: Add formatting to this */
  double pkts_per_second = (double)total_pkts/duration_sec;
  double bytes_per_second = (double)total_bytes/duration_sec;
  printf("\nSummary statistics\n");
  printf("Total packets: %d\n",total_pkts);
  printf("Total bytes: %d\n",total_bytes);
  printf("Average packets per second: %4f\n",pkts_per_second);
  printf("Average bytes per second: %4f\n",bytes_per_second);
  printf("Duration of the test: %4f seconds\n",duration_sec);
}

/* This function is the main entry point into the blastee. This receives packets
   from the blaster, logs information about the packets, and optionally returns
   echo packets. Returns 0 on success.
*/
int main(int argc, char* argv[]){

  int MAX_BACKLOG = 0; // ??
  int total_packets_received = 0;
  int num_bytes = 0;
  int total_bytes_received = 0;
  int average_packets_sec;
  int average_bytes_sec;
  double duration; //duration of the test
  struct sockaddr foreign_addr;
  char type;
  int received_end = 0;//0:no end packet received, 1:end packet received
  int udp_socket;
  int port_num;
  struct sockaddr_in my_addr;
  char str[INET_ADDRSTRLEN];
  int opt;
  char port[256];
  int echo; //1 (expect echo) and 0 (no echo)
  char *buffer = (char *)malloc(MAX_BUFFER);
  time_t current_time;
  time_t start_time;
  socklen_t length;
  uint32_t seq_number;
  char *seq_number_ptr;
  struct timeval time_recv;  
 

    while ((opt = getopt(argc, argv, "p:c:"))!= -1) {
    switch (opt) {
      case 'p':
              strcpy(port, optarg);
              port_num = atoi(port);
            //  printf("port: %s\n", port);
              break;
      case 'c':
              echo = atoi(optarg);
         //     printf("echo: %d\n", echo);
              break;
      }
  }

  if (!is_echo_valid(echo)) {
    perror("Invalid echo value");
    exit(EXIT_FAILURE);
  }

  if (!is_port_valid(port_num)) {
    perror("Invalid port number");
    exit(EXIT_FAILURE);
  }

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port_num);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket == -1) {
    perror("something is wrong with creating socket!\n");
    exit(EXIT_FAILURE);
  }
 // puts("Socket created");
  if(bind(udp_socket, (const struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) == -1){
    perror("something is wrong with binding!\n");
    exit(EXIT_FAILURE);
  }
  //puts("bind done");

  start_time = time(NULL);
  current_time = start_time;
  length = sizeof(struct sockaddr);
  while (difftime(time(NULL),current_time)< MAX_SEC){
    //printf("i am inside the while loop\n");
    num_bytes = recvfrom(udp_socket, (void *)buffer, (size_t)MAX_BUFFER, MSG_DONTWAIT, &foreign_addr, &length); //CHECK THE FLAG///
    if (num_bytes > 0) {
      current_time = time(NULL);
      gettimeofday(&time_recv, NULL);

      type = buffer[0];
      if(type == 'D'){
        total_packets_received++;
    //  printf("Number of packets received %d",total_packets_received);
        total_bytes_received += num_bytes;
        if (echo) {
          set_pkt_type(buffer,'C');
          if (sendto(udp_socket, buffer, (size_t)num_bytes, 0, &foreign_addr, length) == -1) {
           perror("something is wrong with sendto!\n");
           exit(EXIT_FAILURE);
          }
          length = sizeof(struct sockaddr);
     //     printf("Time echo Packet sent: %s\n",ctime(&current_time));
      //    printf("Sent out an echo packet\n");
        }
        //printf("Received a Data packet\n");
        printf("From blaster IP address %s\n",inet_ntop(foreign_addr.sa_family,
            &(((struct sockaddr_in *)&foreign_addr)->sin_addr), str, INET_ADDRSTRLEN));
        printf("Port received on %d\n", port_num);
        //
        //seq_number_ptr = buffer;
        // seq_number_ptr++; //now seq_num_ptr is pointing to sequence
        // seq_number = *((uint32_t *)seq_number_ptr);
        printf("Packet's size in bytes: %d\n",num_bytes);
        printf("Packet Sequence Number %u\n", get_seq_number(buffer));
        printf("Time Packet Received: %s ,milliseconds: %lld \n",ctime(&current_time), time_recv.tv_usec/1000);
        
        printf("First 4 bytes of payload: ");
        print4bytes(buffer+HEADER_BYTES, num_bytes-HEADER_BYTES);
        printf("\n\n");
      }
      else if(type == 'E'){
       // printf("Received an End packet\n");
        received_end = 1;
        break;
      }
      else{
        printf("Received a non-standard packet %c\n",type);
      }
    }
  }
  if (!received_end){
    printf("No end packet received, timeout after 5s \n");
    current_time=time(NULL);
  }
  print_summary(total_packets_received,total_bytes_received,difftime(current_time,start_time));
  close(udp_socket);
  free(buffer);
  return 0;

}
