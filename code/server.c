#include<stdio.h>
#include<stdlib.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<string.h>
#include<netinet/in.h>  
#include<pthread.h>
  
#define PORT 6000  
#define BUF_SIZE 2500  
#define CLADDR_LEN 99 

int ret,newsockfd; 
int sfdc, len;  
char buffer[BUF_SIZE],ch; 
struct sockaddr_in addr,cl_addr;
char clientAddr[CLADDR_LEN];   
pthread_t rThread;



int connection(int ret,int sfdc)
{
  listen(sfdc, 5);  
  
  len = sizeof(cl_addr);  
  newsockfd = accept(sfdc, (struct sockaddr *) &cl_addr, &len);  
  if (newsockfd < 0) {  
     printf("Error accepting connection!\n");  
     exit(1);  
   }   
  
  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN); 
  
  printf("connection from %s to accept press y else n \n",clientAddr); 
  scanf("%s",buffer);
  ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);    
  if (ret < 0) {    
     printf("Error sending data!\n");    
     exit(1);  
    } 

  return ret,newsockfd,len;


}
  
void * receiveMessage(void * socket) {  
 int sfdc, ret;  
 char buffer[BUF_SIZE];   
 sfdc = (int) socket;  
 memset(buffer, 0, BUF_SIZE);    
 while(1) {  
  ret = recvfrom(sfdc, buffer, BUF_SIZE, 0, NULL, NULL);    
  
   if(ret==0) 
       exit(1);
   printf("client: ");  
   fputs(buffer, stdout);  
      
 }  
}  
void main() 
{  
  pid_t childpid;  
  sfdc = socket(AF_INET, SOCK_STREAM, 0);  
  if (sfdc < 0) {  
     printf("Error creating socket\n");  
     exit(1);  
   }  
  memset(&addr, 0, sizeof(addr));  
  addr.sin_family = AF_INET;  
  addr.sin_addr.s_addr = INADDR_ANY;  
  addr.sin_port = PORT;  
   
  ret = bind(sfdc, (struct sockaddr *) &addr, sizeof(addr));  
  if (ret < 0) {  
    printf("Error binding\n");  
   exit(1);  
  }  
  printf("Waiting for a connection,acnowledge the same later\n");  
  connection(ret,sfdc);
  if(buffer[0]=='y' || buffer[0]=='Y') { 
    printf("Connection is sucessfull from %s\n", clientAddr);   
    memset(buffer, 0, BUF_SIZE);  
    printf("Enter your messages one by one and press return key!\n");  
    ret = pthread_create(&rThread, NULL, receiveMessage, (void *) newsockfd);  
    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {  
       ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);    
       if (ret < 0) {    
           printf("Error sending data!\n");    
            exit(1);  
           }  
        }
    }
    else if (buffer[0]=='n' || buffer[0]=='N')
    {
      exit(1);
    }     
  close(newsockfd);  
  close(sfdc);  
  pthread_exit(NULL);  
  
  return;  
} 
