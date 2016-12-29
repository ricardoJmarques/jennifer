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

void clearMessage(Message *msg);

/* Request commands */
/* goes from client to server */

char *requestHELO(char *pipe, char *pseudo);

char *requestBYEE(int id);

char *requestBCST(int id, char *msg);

char *requestPRVT(int id, char *pseudo, char *msg);

char *requestLIST(int id);

char *requestSHUT(int id);

char *requestDEBG(int id);

/* Response commands */
/* Goes from server to client */

char *responseOKOK(int id);

char *responseBADD();

char *responseBYEE(int id);

char *responseBCST(char *pseudo, char *msg);

char *responsePRVT(char *pseudo, char *msg);

char *responseLIST(int n, char *pseudo);

char *responseSHUT(char *pseudo);

/* Reads protocol defined */
/* receives a raw buffer and parses it, returning corps of the message */

Message *readData(char *buffer);

int parseMessageClient(Message *msg);

int parseMessageServeur(Message *msg);
