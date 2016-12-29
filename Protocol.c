#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Message{
   int size;
   int id;
   char type[5];
   char *corps;
   char *pseudoFrom;
   char *pseudoTo;
   char *txt;
   char *pipe;
}Message;

void clearMessage(Message *msg){
  free(msg->corps);
  free(msg->pseudoFrom);
  free(msg->pseudoTo);
  free(msg->txt);
  free(msg->pipe);
  msg->corps = NULL;
  msg->pseudoFrom = NULL;
  msg->pseudoTo = NULL;
  msg->txt = NULL;
  msg->pipe = NULL;
  return;
}

/* Request commands */
/* goes from client to server */
char *requestHELO(char *pipe, char *pseudo){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "HELO%04d%s%04d%s",(int)strlen(pseudo),pseudo,(int)strlen(pipe),pipe);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *requestBYEE(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BYEE%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *requestBCST(int id, char *msg){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BCST%04d%04d%s",id,(int)strlen(msg),msg);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *requestPRVT(int id, char *pseudo, char *msg){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "PRVT%04d%04d%s%04d%s",id,(int)strlen(pseudo),pseudo,(int)strlen(msg),msg);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *requestLIST(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "LIST%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *requestSHUT(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "SHUT%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *requestDEBG(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "DEBG%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

/* Response commands */
/* Goes from server to client */
char *responseOKOK(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "OKOK%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *responseBADD(){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BADD");
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *responseBYEE(int id){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BYEE%04d",id);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *responseBCST(char *pseudo, char *msg){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "BCST%04d%s%04d%s",(int)strlen(pseudo),pseudo,(int)strlen(msg),msg);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *responsePRVT(char *pseudo, char *msg){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "PRVT%04d%s%04d%s",(int)strlen(pseudo),pseudo,(int)strlen(msg),msg);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *responseLIST(int n, char *pseudo){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "LIST%04d%04d%s",n,(int)strlen(pseudo),pseudo);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

char *responseSHUT(char *pseudo){
  char buffer[4096]={0};
  char *cmd=NULL;
  sprintf(buffer, "SHUT%04d%s",(int)strlen(pseudo),pseudo);
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer)+5));
  sprintf(cmd, "%04d%s", (int)strlen(buffer),buffer);
  return cmd;
}

/* Reads protocol defined */
/* receives a raw buffer and parses it, returning corps of the message */
Message *readData(char *buffer){
  int size=0;
  Message *msg=NULL;
  int i=0,j=0;
  msg=(Message*)malloc(sizeof(Message));
  if (strlen(buffer)<8)
    printf("Comando lenght error!");
  else{
    for(i=0; i<4; i++){
      if(buffer[i] >= '0' && buffer[i] <= '9')
        size = (size*10)+(buffer[i]-'0');
      msg->type[i] = buffer[i+4];
    }
    msg->type[i] = '\0';
    msg->corps = (char*)malloc(sizeof(char)*size-4);
    for(j=0; j<size-4; j++,i++)
      msg->corps[j] = buffer[i+4];
    msg->corps[size-4]='\0';
    msg->size=size;
  }
  return msg;
}

int parseMessageClient(Message *msg){
  int erro = 0;
  int i=0;
  int j=0;
  int size=0;
  int id=0;
  char *pseudo;
  char *txt=NULL;
  if(strcmp(msg->type, "OKOK")==0){
    for(i=0; i<4; i++)
      if(msg->corps[i]>='0' && msg->corps[i]<='9')
        id=id*10+(msg->corps[i]-'0');
    msg->id=id;
    printf("id: %d\n", id);
  }
  else if(strcmp(msg->type, "BADD")==0){
    printf("Bad\n");    
  }
  else if(strcmp(msg->type, "BYEE")==0){
    for(i=0; i<4; i++)
      if(msg->corps[i]>='0' && msg->corps[i]<='9')
        id=id*10+(msg->corps[i]-'0');
    msg->id=id;
    printf("id: %d\n", id);
  }
  else if(strcmp(msg->type, "BCST")==0){
    for(i=0; i<4; i++)
      size = (size*10)+(msg->corps[i]-'0');
    pseudo = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      pseudo[j] = msg->corps[i];
    pseudo[size]='\0';
    size=0;
    for(j=0; j<4; i++,j++)
      size = (size*10)+(msg->corps[i]-'0');
    txt = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      txt[j] = msg->corps[i];
    txt[size]='\0';
    msg->pseudoFrom=pseudo;
    msg->txt=txt;
    printf("pseudo: %s | msg: %s\n", pseudo, txt);
  }
  else if(strcmp(msg->type, "PRVT")==0){
    for(i=0; i<4; i++)
      size = (size*10)+(msg->corps[i]-'0');
    pseudo = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      pseudo[j] = msg->corps[i];
    pseudo[size]='\0';
    size=0;
    for(j=0; j<4; i++,j++)
      size = (size*10)+(msg->corps[i]-'0');
    txt = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      txt[j] = msg->corps[i];
    txt[size]='\0';
    msg->pseudoFrom=pseudo;
    msg->txt=txt;    
    printf("pseudo: %s | msg: %s\n", pseudo, txt);
  }
  else if(strcmp(msg->type, "LIST")==0){
    for(i=0; i<4; i++)
      id = (id*10)+(msg->corps[i]-'0');
    for(i=4; i<8; i++)
      size = (size*10)+(msg->corps[i]-'0');
    pseudo = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      pseudo[j] = msg->corps[i];
    pseudo[size]='\0';
    msg->id=id;
    msg->pseudoFrom=pseudo;
    printf("id: %d | pseudo: %s\n", id, pseudo);
  }
  else if(strcmp(msg->type, "SHUT")==0){
    for(i=0; i<4; i++)
      size = (size*10)+(msg->corps[i]-'0');
    pseudo = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      pseudo[j] = msg->corps[i];
    pseudo[size]='\0';
    msg->pseudoFrom=pseudo;
    printf("pseudo: %s\n", pseudo);
  }
  else
    erro=1;
  return erro;
}

int parseMessageServeur(Message *msg){
  int erro = 0;
  int i=0;
  int j=0;
  int size=0;
  int id=0;
  char *pseudo;
  char *pipe;
  char *txt=NULL;
  if(strcmp(msg->type, "HELO")==0){
    for(i=0; i<4; i++)
      size = size*10+(msg->corps[i]-'0');
    pseudo = (char*)malloc(sizeof(char)*(size+1));
    for(j=0;j<size;j++,i++)
      pseudo[j]=msg->corps[i];
    pseudo[size]='\0';
    size=0;
    for(j=0; j<4; i++,j++)
      size = size*10+(msg->corps[i]-'0');
    pipe = (char*)malloc(sizeof(char)*(size+1));
    for(j=0;j<size;j++,i++)
      pipe[j]=msg->corps[i];
    pipe[size]='\0';
    msg->pseudoFrom=pseudo;
    msg->pipe=pipe;
    printf("pseudo: %s | pipe: %s\n", pseudo, pipe);
  }
  else if(strcmp(msg->type, "BYEE")==0){
    for(i=0; i<4; i++)
      if(msg->corps[i]>='0' && msg->corps[i]<='9')
        id=id*10+(msg->corps[i]-'0');
    printf("id: %d\n", id);
  }
  else if(strcmp(msg->type, "BCST")==0){
    for(i=0; i<4; i++)
      id = (id*10)+(msg->corps[i]-'0');
    for(; i<8; i++)
      size = (size*10)+(msg->corps[i]-'0');
    txt = (char*)malloc(sizeof(char)*(size+1));
    strcpy(txt, &msg->corps[i]);
    txt[size]='\0';
    msg->id=id;
    printf("id: %d | msg: %s\n", id, txt);
  }
  else if(strcmp(msg->type, "PRVT")==0){
    for(i=0; i<4; i++)
      id = (id*10)+(msg->corps[i]-'0');
    for(i=4; i<8; i++)
      size = (size*10)+(msg->corps[i]-'0');
    pseudo = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      pseudo[j] = msg->corps[i];
    pseudo[size]='\0';
    size=0;
    for(j=0; j<4; i++,j++)
      size = (size*10)+(msg->corps[i]-'0');
    txt = (char*)malloc(sizeof(char)*(size+1));
    for(j=0; j<size; i++,j++)
      txt[j] = msg->corps[i];
    txt[size]='\0';
    msg->id=id;
    msg->pseudoTo=pseudo;
    msg->txt=txt;
    printf("id: %d | pseudo: %s | msg: %s\n", id, pseudo, txt);
  }
  else if(strcmp(msg->type, "LIST")==0){
    for(i=0; i<4; i++)
      if(msg->corps[i]>='0' && msg->corps[i]<='9')
        id=id*10+(msg->corps[i]-'0');
    msg->id=id;
    printf("id: %d\n", id);
  }
  else if(strcmp(msg->type, "SHUT")==0){
    for(i=0; i<4; i++)
      if(msg->corps[i]>='0' && msg->corps[i]<='9')
        id=id*10+(msg->corps[i]-'0');
    msg->id=id;
    printf("id: %d\n", id);
  }
  else if(strcmp(msg->type, "DEBG")==0){
    for(i=0; i<4; i++)
      if(msg->corps[i]>='0' && msg->corps[i]<='9')
        id=id*10+(msg->corps[i]-'0');
    msg->id=id;
    printf("id: %d\n", id);
  }
  else
    erro=1;
  return erro;
}
/*
int main(){
  char *data;
  Message *msg;
  char pseudo[]="PaiNatal";
  char txt[]="Feliz Natal";
  int id=2512;
  char pipe[]="/tmp/pipecliente";

  printf("###############################################\n");
  printf("## Server Parser\n\n");
  
  data = requestHELO(pipe, pseudo);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = requestBYEE(id);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = requestBCST(id, txt);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = requestPRVT(id, pseudo, txt);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = requestLIST(id);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = requestSHUT(id);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = requestDEBG(id);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageServeur(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  printf("###############################################\n");
  printf("## Client Parser\n");

  data = responseOKOK(id);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = responseBADD();
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = responseBYEE(id);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = responseBCST(pseudo, txt);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = responsePRVT(pseudo, txt);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = responseSHUT(pseudo);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  data = responseLIST(1, pseudo);
  printf("DATA: %s\n", data);
  msg = readData(data);
  printf("SIZE: %d | TYPE: %s | DATA: %s\n",msg->size, msg->type, msg->corps);
  parseMessageClient(msg);
  clearMessage(msg);
  free(data);
  free(msg);
  printf("DONE\n\n");

  return 0;
}

*/
