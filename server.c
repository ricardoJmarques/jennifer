#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //needed for read() & write()
#include <stdio.h>  //needed for printf() scanf()

#define serverFIFOPATH "/tmp/sobusrv" /*Named Pipe for server listening*/
#define tamCMD 4096

int main(){	
  int fd, loopT, i;
  char buffer[tamCMD];
  //cmdSessao cmd;
  loopT = 1;

  if ((mkfifo(serverFIFOPATH, 0666)) == 0) {
     printf("server started with ID: %d\n", getpid());
  }
  else {
    printf("Error opening FIFO!");
    loopT = 0;
  }
  
  while(loopT){
    fd = open(serverFIFOPATH, O_RDONLY);
    while (read(fd, buffer, tamCMD) > 0) {
      
      //lePedido(buffer, &cmd);
    }
    for (i=0; i<tamCMD; i++) buffer[i] = '\0'; /*clear buffer*/
    close(fd);
  }
  unlink(serverFIFOPATH);
  return 0;
}

