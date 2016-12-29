#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void readCmd(char *buffer);

char *HELOcmd(char *pipe, char *pseudo);

char *BYEEcmd(int id);

char *BCSTcmd(int id, char *msg);

char *PRVTcmd(int id, char *pseudo, char *msg);

char *LISTcmd(int id);

char *SHUTcmd(int id);

char *DEBGcmd();
