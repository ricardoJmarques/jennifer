#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void readCmd(char *buffer);

char *OKOKcmd(int id); //HELO ok response

char *BADDcmd(); //HELO nok response

char *BYEEcmd(int id);

char *BCSTcmd(int id, char *pseudo, char *msg);

char *PRVTcmd(int id, char *pseudo, char *msg);

char *LISTcmd(int n, char *pseudo);

char *SHUTcmd(int id);
