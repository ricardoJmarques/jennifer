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

int main(){
  char buffer[4096] = {0};
  char buffer2[4096] = {0};
  int readSize=0;
  char username[1024] = {0};
  int userSize=0;
  char *aux;
  char *comando;
  char quit=0;
  char *cmd;
  //int fd_read;
  int fd_write;
  int pid=getpid();
  char fifo[128];
  int i=0;
  sprintf(fifo, "/tmp/%d", pid);
  /*create FIFO*/
  if ((mkfifo(fifo, 0422)) == 0) {
    printf("Client started with ID: %d\n", pid);
  }
  else {
    printf("Error opening FIFO!");
    quit = 1;
  }
//  fd_read=open(fifo, O_RDONLY);
  fd_write=open(serverFIFOPATH, O_WRONLY);
  if(!quit){
    welcome();
    /* username request and processing */
    write(1, "Pseudo> ", 8);
    userSize = read(0, &username, 1024);
    if(userSize>2 && userSize<41){
      username[userSize-1]='\0';
      write(1, "--== WeLcOmE To tHe cHatRoOm, ", 30);
      write(1, username, userSize);
      write(1, " ==--\n", 6);
      comando = HELOcmd(fifo, username);
      write(fd_write, comando, strlen(comando));
      free(comando);
    }
    else{
      write(1, "--== Username Error: size [2-40] ==--\n", 39);
      quit=1;
    }
  }
  for (i=0; i<tamCMD; i++) buffer[i] = '\0'; /*clear buffer*/

  /* standard Input read and process */
  while(!quit && (readSize=read(0, &buffer, 4096))>0){
    if(readSize>1){
      strcpy(buffer2, buffer);
      aux = strtok(buffer, "\n");
      comando = strtok(aux, " ");
      if(strcmp(comando, "quit")==0){//quit application
        cmd = BYEEcmd(pid);
        printf("%s\n", cmd);
        free(cmd);
        printf("--== Bye Bye ==--\n");
        quit = 1;
      }
      else if(strcmp(comando, "help")==0){//quit application
        help();
      }
      else if(strcmp(comando, "private")==0){//send private message
        cmd = PRVTcmd(pid, "jennifer", "mensagem para jennifer");
        printf("%s\n", cmd);
        free(cmd);
        printf("--== PrIvAtE ==--\n");
      }
      else if(strcmp(comando, "send")==0){//send file
        printf("--== SeNd fIlE ==--\n");
      }
      else if(strcmp(comando, "list")==0){//list users
        cmd = LISTcmd(pid);
        printf("%s\n", cmd);
        free(cmd);
        printf("--== LiSt uSerS ==--\n");
      }
      else if(strcmp(comando, "shutdown")==0){//list users
        cmd = SHUTcmd(pid);
        printf("%s\n", cmd);
        free(cmd);
        printf("--== ShUtDoWn mOdE ==--\n");
      }
      else if(strcmp(comando, "debug")==0){//list users
        cmd = DEBGcmd();
        printf("%s\n", cmd);
        free(cmd);
        printf("--== DeBuG MoDe ==--\n");
      }
      else{//default - send message to everyone
        cmd = BCSTcmd(pid, strtok(buffer2, "\n"));
//        printf("%s\n", cmd);
        write(fd_write, cmd, strlen(cmd));
        free(cmd);
//        write(1, &username, userSize);
//        write(1, "> ", 2);
//        write(1, &buffer2, readSize);
//        write(1, "\n", 1);
      }
    }
    for (i=0; i<tamCMD; i++){
      buffer[i] = '\0'; /*clear buffers*/
      buffer2[i] = '\0';
    }
  }
  
  return 0;
}
