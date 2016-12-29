#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //needed for read() & write()
#include <stdlib.h> //needed for malloc()
#include <string.h> //needed for strlen()
#include <stdio.h>  //needed for printf() scanf()
#include "cliente_cmd.h"

#define serverFIFOPATH "/tmp/sobusrv" /*Named Pipe for server listening*/
#define tamCMD 4096
#define maxUserSize 40

int help(){
  printf("########################################################\n");
  printf("##                   -= Help Menu =-                  ##\n");
  printf("## private to $user        -> private message to user ##\n");
  printf("## send $file to $username -> sends file to user      ##\n");
  printf("## quit                    -> quits chat              ##\n");
  printf("## help                    -> prints help menu        ##\n");
  printf("########################################################\n");
  return 0;
}

int welcome(){
  printf("########################################################\n");
  printf("##                                                    ##\n");
  printf("##   Welcome to Interconnect Pipe Chat Cliente- IPC   ##\n");
  printf("##                                                    ##\n");
  printf("########################################################\n");
  return 0;
}

char *getUser(){
  char buffer[1024];
  char *username = (char*)malloc(sizeof(char)*maxUserSize);
  int userSize=0;
  write(1, "Pseudo> ", 8);
  if(username > 0){
    userSize = read(0, &buffer, 1024);
    if(userSize>2 && userSize<maxUserSize-1){
      strcpy(username, buffer);
      username[userSize-1]='\0';
    }
    else{
      free(username);
      username = NULL;
      while((userSize = read(0, &buffer, 1024))>0);
      printf("Username size must be between 2-40 characters\n");
    }
  }
  return username;
}

char *parseInput(char *buffer, int pid){
  char *buff_copy = NULL;
  char *comando = NULL;
  char *aux = NULL;
  char *cmd = NULL;
  char *to = NULL;
  char *msg = NULL;
  buff_copy = (char*)malloc(sizeof(char)*strlen(buffer));
  strcpy(buff_copy, buffer);
  aux = strtok(buffer, "\n");
  if(strcmp(aux, "quit")==0)//quit application
    cmd = BYEEcmd(pid);
  else{
    comando = strtok(aux, " ");
    if(strcmp(comando, "help")==0)//quit application
      help();
    else if(strcmp(comando, "private")==0){//send private message
      aux = strtok(NULL, " ");
      to = strtok(NULL, " ");
      msg = strtok(NULL, "\0");
      cmd = PRVTcmd(pid, to, msg);
    }
    else if(strcmp(comando, "send")==0){//send file
      printf("--== SeNd fIlE ==--\n");
    }
    else if(strcmp(comando, "list")==0)//list users
      cmd = LISTcmd(pid);
    else if(strcmp(comando, "shutdown")==0)//Shutdown command
      cmd = SHUTcmd(pid);
    else if(strcmp(comando, "debug")==0)  //debug command
      cmd = DEBGcmd();
    else{//default - send message to everyone
      cmd = BCSTcmd(pid, strtok(buff_copy, "\n"));
    }
  }
  return cmd;
}

int main(){
  char buffer[4096] = {0};
  int readSize=0;
  char *cmd = NULL;
  char *username = NULL;
  char quit=0;
  int fd_read;
  int fd_write;
  int pid=getpid();
  char fifo[128];
  int i=0;
  sprintf(fifo, "/tmp/%d", pid);
  /*create FIFO*/
  if ((mkfifo(fifo, 0666)) != 0) {
    printf("Error opening FIFO!");
    quit = 1;
  }
  else {
    printf("Client started with ID: %d\n", pid);
  }
  fd_write=open(serverFIFOPATH, O_WRONLY);
  
  if(!quit){
    welcome();
    while(username == NULL && i<5){
      username=getUser();
      i++;
    }
    if(username != NULL){
      cmd = HELOcmd(fifo, username);
      write(fd_write, cmd, strlen(cmd));
      free(cmd);
      fd_read=open(fifo, O_RDONLY);
      write(1, "--== WeLcOmE To tHe cHatRoOm, ", 30);
      write(1, username, strlen(username));
      write(1, " ==--\n", 6);
      write(fd_write, cmd, strlen(cmd));
    }
    else{
      write(1, "--== Username Error: size [2-40] ==--\n", 39);
      quit=1;
    }
  }
  for (i=0; i<tamCMD; i++) buffer[i] = '\0'; /*clear buffer*/

  fork();
  if(getpid()==pid){
    /* standard Input read and process */
    while(!quit && (readSize=read(0, &buffer, 4096))>0){
      if(readSize>1){
        cmd = parseInput(buffer, pid);
        if(cmd!=NULL)
          write(fd_write, cmd, strlen(cmd));
      }
    }
    for (i=0; i<tamCMD; i++) buffer[i] = '\0'; /*clear buffers*/
  }
//  else {
//    fd_read=open(fifo, O_RDONLY); 
//  }
  return 0;
}
