#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void readCmd(char *buffer){
  int size=0;
  char type[5];
  char *corps=NULL;
  int i=0;
  if (strlen(buffer)<8)
    printf("Comando lenght error!");
  else{
    for(i=0; i<4; i++){
      if(buffer[i] >= '0' && buffer[i] <= '9')
        size = (size*10)+(buffer[i]-'0');
      type[i] = buffer[i+4];
    }
    type[i] = '\0';
    corps = (char*)malloc(sizeof(char)*size);
    strcpy(corps, &buffer[i+4]);
    printf("%s - %s - %d\n", type, corps, size);
  }
  if(corps!=NULL)
    free(corps);
  return;
}

char *HELOcmd(char *pipe, char *pseudo){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "HELO%04d%s%04d%s",(int)strlen(pseudo),pseudo,(int)strlen(pipe),pipe);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *BYEEcmd(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BYEE%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *BCSTcmd(int id, char *msg){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BCST%04d%04d%s",id,(int)strlen(msg),msg);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *PRVTcmd(int id, char *pseudo, char *msg){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "PRVT%04d%s%04d%s",id, pseudo,(int)strlen(msg),msg);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *LISTcmd(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "LIST%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *SHUTcmd(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "SHUT%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *DEBGcmd(){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "DEBG");
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+4));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

