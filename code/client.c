#include<stdio.h>   
#include<stdlib.h>    
#include<sys/types.h>    
#include<sys/socket.h>    
#include<string.h>
#include<netinet/in.h>    
#include<netdb.h>  
#include<pthread.h>  
    
#define PORT 6000   
#define BUF_SIZE 2500   

struct sockaddr_in addr, cl_addr;    
 int sfdc, set;    
 char buffer[BUF_SIZE];   
 char * serverAddr;  
 pthread_t rThread;
    
void * receiveMessage(void * socket) {  
 int sfdc, set;  
 char buffer[BUF_SIZE];   
 sfdc = (int) socket;  
 memset(buffer, 0, BUF_SIZE);    
   while(1) {  
     set = recvfrom(sfdc, buffer, BUF_SIZE, 0, NULL, NULL);    
     if (set < 0) {    
          printf("Error receiving data!\n");      
     } else { 
     if(set==0)
         exit(1);
   
    
   fputs(buffer, stdout); 
   printf("server: ");  
  }    
 }  
}  

int main(int argc, char**argv) {    
   
  
 if (argc < 2) {  
  printf("use client <ip-address>\n");  
  exit(1);    
 }
  
  
 serverAddr = argv[1];   
 sfdc = socket(AF_INET, SOCK_STREAM, 0);    
 if (sfdc < 0) {    
  printf("Error creating socket!\n");    
  exit(1);    
 }    
 printf("wait till the server accepts Your connection\n");   
 memset(&addr, 0, sizeof(addr));    
 addr.sin_family = AF_INET;    
 addr.sin_addr.s_addr = inet_addr(serverAddr);  
 addr.sin_port = PORT;       
  
 set = connect(sfdc, (struct sockaddr *) &addr, sizeof(addr));    
 if (set < 0) {    
  printf("Error connecting\n");    
  exit(1);    
  }  

 set = recvfrom(sfdc, buffer, BUF_SIZE, 0, NULL, NULL); 
 if(buffer[0]=='y' || buffer[0]=='Y')  {
   printf("Connection with server was sucessfull\n"); 
 memset(buffer, 0, BUF_SIZE);  
  
 set = pthread_create(&rThread, NULL, receiveMessage, (void *) sfdc);  
 while (fgets(buffer, BUF_SIZE, stdin) != NULL) {  
   set = sendto(sfdc, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));    
   if (set < 0) {    
     printf("Error sending data!\n\t-%s", buffer);    
   }  
  }  
 }
 else if (buffer[0]=='n' || buffer[0]=='N')
  {
    printf("Server rejected your connection\n");
    exit(1);
  }     
  close(sfdc);  
  pthread_exit(NULL);  
   
  return 0;      
} 

