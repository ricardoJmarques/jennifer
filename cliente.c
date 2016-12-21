#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //needed for read() & write()
#include <stdlib.h> //needed for malloc()
#include <string.h> //needed for strlen()
#include <stdio.h>  //needed for printf() scanf()
#include "cliente_cmd.h"

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
  char pipe[] ="cliente1";
  char *cmd;
  welcome();
  /* username request and processing */
  write(1, "Pseudo> ", 8);
  userSize = read(0, &username, 1024);
  if(userSize>2 && userSize<41){
    username[userSize-1]='\0';
    write(1, "--== WeLcOmE To tHe cHatRoOm, ", 30);
    write(1, username, userSize);
    write(1, " ==--\n", 6);
    comando = HELOcmd(pipe, username);
    write(1, comando, strlen(comando));
    write(1, "\n", 1);
    free(comando);
  }
  else{
    write(1, "--== Username Error: size [2-40] ==--\n", 39);
    quit=1;
  }

  /* standard Input read and process */
  while(!quit && (readSize=read(0, &buffer, 4096))>0){
    if(readSize>1){
      strcpy(buffer2, buffer);
      aux = strtok(buffer, "\n");
      comando = strtok(aux, " ");
      if(strcmp(comando, "quit")==0){//quit application
        cmd = BYEEcmd(1234);
        printf("%s\n", cmd);
        free(cmd);
        printf("--== Bye Bye ==--\n");
        quit = 1;
      }
      else if(strcmp(comando, "help")==0){//quit application
        help();
      }
      else if(strcmp(comando, "private")==0){//send private message
        cmd = PRVTcmd(1234, "sujeito", "mensagem para sujeito");
        printf("%s\n", cmd);
        free(cmd);
        printf("--== PrIvAtE ==--\n");
      }
      else if(strcmp(comando, "send")==0){//send file
        printf("--== SeNd fIlE ==--\n");
      }
      else if(strcmp(comando, "list")==0){//list users
        cmd = LISTcmd(1234);
        printf("%s\n", cmd);
        free(cmd);
        printf("--== LiSt uSerS ==--\n");
      }
      else if(strcmp(comando, "shutdown")==0){//list users
        cmd = SHUTcmd(1234);
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
        cmd = BCSTcmd(1234, "mensagem para todos");
        printf("%s\n", cmd);
        free(cmd);
        write(1, &username, userSize);
        write(1, "> ", 2);
        write(1, &buffer2, readSize);
      }
    }
  }
  
  return 0;
}
