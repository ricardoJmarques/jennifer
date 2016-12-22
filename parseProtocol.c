int readCmdServer(char *cmd){
  int size=0;
  char type[4]={0};
  int i=0;
  //getting size and type of command
  while (i<4){
    size = size*10+cmd[i];
    type[i] = cmd[i+4];
    i++;
  }
  if(strcpm(type, "HELO"){ //client request connection
  }
  else if(strcpm(type, "BYEE"){ //client quits chat
  }
  else if(strcpm(type, "BCST"){ //client send public message
  }
  else if(strcpm(type, "PRVT"){ //client send private message
  }
  else if(strcpm(type, "LIST"){ //client requests user list
  }
  else if(strcpm(type, "SHUT"){ //client send shutdown command to server
  }
  return 0;
}

int readCmdClient(char *cmd){
  int size=0;
  char type[4]={0};
  int i=0;
  //getting size and type of command
  while (i<4){
    size = size*10+cmd[i];
    type[i] = cmd[i+4];
    i++;
  }
  if(strcpm(type, "OKOK"){ //server accepts connection
  }
  else if(strcpm(type, "BADD"){ //server denies request
  }
  else if(strcpm(type, "BYEE"){ //server closes client connection
  }
  else if(strcpm(type, "BCST"){ //server sends public message to client
  }
  else if(strcpm(type, "PRVT"){ //server sends private message to client
  }
  else if(strcpm(type, "LIST"){ //server sends list of users to client
  }
  else if(strcpm(type, "SHUT"){ //server sends shutdown to client
  }
  return 0;
}

char *HELOcmd(char *pipe, char *pseudo){
  char buffer[4096]={0};
  char *cmd==NULL;
  sprintf(buffer, "HELO%d%s%d%s",strlen(pseudo),pseudo,strlen(pipe),pipe
  cmd = (char*)malloc(sizeof(char)*(strlen(buffer+4));
  sprintf(cmd, "%d%s", strlen(buffer),buffer);
  return cmd;
}
