/*--------------------------------------------------------------------------------------------------
  This is just a test client, which let's you directly type to your console application.
-------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgiutil.h"
#include "comclient.h"

int main(
    int argc,
    char **argv)
{
    char line[CO_MAX_MESSAGE_LENGTH];
    char *response;
    char *sessionID = cgiGenerateRandomID(20);

    if(argc != 1) {
        printf("Usage: %s\n", argv[0]);
        return 1;
    }
    response = coStartClient("/tmp/sharealot", sessionID);
    printf("%s", response);
    while(fgets(line, CO_MAX_MESSAGE_LENGTH, stdin) && strcmp(line, "quit")) {
        coSendMessage("%s", line);
        response = coReadMessage();
        printf("%s", response);
    }
    coStopClient();
    return 0;
}

