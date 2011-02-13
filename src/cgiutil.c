/*--------------------------------------------------------------------------------------------------
  This example CGI script maintains a session ID, and writes user input to a file in /tmp/data.txt.
--------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "cgiutil.h"

typedef unsigned int uint;

static char *varBuf = NULL, *valueBuf = NULL;
static long varBufSize = 42, valueBufSize = 42;
static FILE *debugFile = NULL;

/*--------------------------------------------------------------------------------------------------
  Enable debug logging to the file.
--------------------------------------------------------------------------------------------------*/
void cgiEnableDebug(
    char *logFile)
{
    debugFile = fopen(logFile, "w");
}

/*--------------------------------------------------------------------------------------------------
  A print function that can optionally log to a log file while printing.
--------------------------------------------------------------------------------------------------*/
int cgiPrintf(
    char *format,
    ...)
{
    va_list ap;
    char buffer[CGI_MAX_STRING_LENGTH];
    int retVal;

    va_start(ap, format);
    vsnprintf(buffer, CGI_MAX_STRING_LENGTH, format, ap);
    va_end(ap);
    retVal = printf("%s", buffer);
    if(debugFile != NULL) {
        fprintf(debugFile, "%s", buffer);
        fflush(debugFile);
    }
    return retVal;
}

/*--------------------------------------------------------------------------------------------------
  Just unencode the string.  Do it in place.
--------------------------------------------------------------------------------------------------*/
char *cgiEncode(
    char *string)
{
    char *p, c;

    if(varBuf == NULL) {
        varBuf = (char *)calloc(varBufSize, sizeof(char));
        valueBuf = (char *)calloc(valueBufSize, sizeof(char));
    }
    if(strlen(string)*3 + 1 > valueBufSize) {
        valueBufSize = strlen(string) << 2;
        valueBuf = (char *)realloc(valueBuf, valueBufSize*sizeof(char));
    }
    p = valueBuf;
    while(*string != '\0') {
        c = *string++;
        if(c == ' ') {
            *p++ = '+';
        } else if(!isalnum(c) && c != '.' && c != '_') {
            *p++ = '%';
            sprintf(p, "%02x", (unsigned char)c);
            p += 2;
        } else {
            *p++ = c;
        }
    }
    *p = '\0';
    return valueBuf;
}

/*--------------------------------------------------------------------------------------------------
  Just unencode the string.  Do it in place.
--------------------------------------------------------------------------------------------------*/
char *cgiEncodeArray(
    char *array,
    int length)
{
    char *p, c;

    if(varBuf == NULL) {
        varBuf = (char *)calloc(varBufSize, sizeof(char));
        valueBuf = (char *)calloc(valueBufSize, sizeof(char));
    }
    if(length*3 + 1 > valueBufSize) {
        valueBufSize = length << 2;
        valueBuf = (char *)realloc(valueBuf, valueBufSize*sizeof(char));
    }
    p = valueBuf;
    while(length-- != 0) {
        c = *array++;
        if(c == ' ') {
            *p++ = '+';
        } else if(!isalnum(c) && c != '.' && c != '_') {
            *p++ = '%';
            sprintf(p, "%02x", (unsigned char)c);
            p += 2;
        } else {
            *p++ = c;
        }
    }
    *p = '\0';
    return valueBuf;
}

/*--------------------------------------------------------------------------------------------------
  Just unencode the string.  Do it in place.
--------------------------------------------------------------------------------------------------*/
void cgiUnencode(
    char *string)
{
    char *p = string;
    int code;

    while(*string != '\0') {
        if(*string == '+') {
            *p = ' ';
        } else if(*string == '%') {
            if(sscanf(string + 1, "%2x", &code) != 1) {
                code = '?';
            }
            *p = code;
            string += 2;
        } else {
            *p = *string;
        }
        string++;
        p++;
    }
    *p = '\0';
}

/*--------------------------------------------------------------------------------------------------
  Read the next variable name and value in the cookie string, and return a pointer to the next
  variable/value pair in the cookie string.
--------------------------------------------------------------------------------------------------*/
static char *readNextVariableValueCombo(
    char *string,
    char separator)
{
    char c;
    int varPos = 0, valuePos = 0;

    while((c = *string++) != '=' && c != '\0') {
        if(varPos + 1 == varBufSize) {
            varBufSize <<= 1;
            varBuf = (char *)realloc(varBuf, varBufSize*sizeof(char));
        }
        varBuf[varPos++] = c;
    }
    varBuf[varPos] = '\0';
    while((c = *string++) != separator && c != '\0') {
        if(valuePos + 1 == valueBufSize) {
            valueBufSize <<= 1;
            valueBuf = (char *)realloc(valueBuf, valueBufSize*sizeof(char));
        }
        valueBuf[valuePos++] = c;
    }
    valueBuf[valuePos] = '\0';
    while(*string == ' ') {
        string++;
    }
    cgiUnencode(varBuf);
    cgiUnencode(valueBuf);
    return string;
}

/*--------------------------------------------------------------------------------------------------
  Read a cookie variable.  Return NULL if not set.
--------------------------------------------------------------------------------------------------*/
char *cgiReadCookie(
    char *varName)
{
    char *cookies = getenv("HTTP_COOKIE");
    char *retVal;

    if(varBuf == NULL) {
        varBuf = (char *)calloc(varBufSize, sizeof(char));
        valueBuf = (char *)calloc(valueBufSize, sizeof(char));
    }
    if(cookies == NULL) {
        return NULL;
    }
    while(*cookies != '\0') {
        cookies = readNextVariableValueCombo(cookies, ';');
        if(!strcmp(varName, varBuf)) {
            retVal = (char *)calloc(strlen(valueBuf) + 1, sizeof(char));
            strcpy(retVal, valueBuf);
            return retVal;
        }
    }
    return NULL;
}

/*--------------------------------------------------------------------------------------------------
  Generate a random session ID.
--------------------------------------------------------------------------------------------------*/
char *cgiGenerateRandomID(
    unsigned int length)
{
    FILE *randFile = fopen("/dev/urandom", "r");
    static char randChars[CGI_MAX_RANDOM_ID + 1];
    char *string = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";
    int i;

    if(length > CGI_MAX_RANDOM_ID) {
        printf("Random ID too long.\n");
        exit(1);
    }
    for(i = 0; i < length; i++) {
        randChars[i] = string[getc(randFile) & 0x3f];
    }
    randChars[length] = '\0';
    fclose(randFile);
    return randChars;
}

/*--------------------------------------------------------------------------------------------------
  Read input.
--------------------------------------------------------------------------------------------------*/
char *cgiReadInput(void)
{
    char *lenstr = getenv("CONTENT_LENGTH");
    char *input;
    long len;

    if(lenstr == NULL) {
        cgiPrintf("CONTENT_LENGTH not set");
        return NULL;
    }
    if(sscanf(lenstr,"%ld",&len) != 1 || len > CGI_INPUT_MAXLEN) {
        cgiPrintf("Invalid input length");
        return NULL;
    }
    input = (char *)calloc(len + 1, sizeof(char));
    return fgets(input, len + 1, stdin);
}

/*--------------------------------------------------------------------------------------------------
  Read an input variable from the input string.
--------------------------------------------------------------------------------------------------*/
char *cgiReadInputVar(
    char *input,
    char *varName)
{
    char *retVal;

    if(varBuf == NULL) {
        varBuf = (char *)calloc(varBufSize, sizeof(char));
        valueBuf = (char *)calloc(valueBufSize, sizeof(char));
    }
    while(*input != '\0') {
        input = readNextVariableValueCombo(input, '&');
        if(!strcmp(varName, varBuf)) {
            retVal = (char *)calloc(strlen(valueBuf) + 1, sizeof(char));
            strcpy(retVal, valueBuf);
            return retVal;
        }
    }
    return NULL;
}

/*--------------------------------------------------------------------------------------------------
  Read the sessionId cookie, and if not set, set it.  Return the sessionId.
--------------------------------------------------------------------------------------------------*/
char *cgiReadSessionId(void)
{
    char *session = cgiReadCookie("sessionId");

    if(session == NULL) {
        session = cgiGenerateRandomID(20);
        cgiPrintf("Set-Cookie: sessionId=%s\n", session);
    }
    return session;
}
/*--------------------------------------------------------------------------------------------------
  Find out how many args are used in a template.
--------------------------------------------------------------------------------------------------*/
static uint countArgs(
    char *temp)
{
    char c;
    uint maxArg = 0, xArg;

    while (*temp) {
        c = *temp++;
        if (c == '%') {
            c = *temp;
            if (c == 'l' || c == 'u' || c == 'c') {
                temp++;
                c = *temp;
            }
            if(isdigit(c)) {
                temp++;
                xArg = c - '0';
                if (xArg >= maxArg) {
                    maxArg = xArg + 1;
                }
            } else if (c == '%') {
                temp++;
            }
        }
    }
    return maxArg;
}

/*--------------------------------------------------------------------------------------------------
  This code manages a simple string buffer.
--------------------------------------------------------------------------------------------------*/
static char *cgiStringBuffer;
static uint cgiStringBufferSize;
static uint cgiStringBufferPosition;

/*--------------------------------------------------------------------------------------------------
  Append a string to the string buffer.
--------------------------------------------------------------------------------------------------*/
static void appendString(
    char *string)
{
    uint length = strlen(string);

    if(cgiStringBufferPosition + length + 1 >= cgiStringBufferSize) {
        cgiStringBufferSize = ((cgiStringBufferPosition + length + 1)*3) >> 1;
        cgiStringBuffer = (char *)realloc(cgiStringBuffer, cgiStringBufferSize*sizeof(char));
    }
    strcpy(cgiStringBuffer + cgiStringBufferPosition, string);
    cgiStringBufferPosition += length;
}

/*--------------------------------------------------------------------------------------------------
  Append a character to the string buffer.
--------------------------------------------------------------------------------------------------*/
static void appendChar(
    char c)
{
    char string[2];

    string[0] = c;
    string[1] = '\0';
    appendString(string);
}

/*--------------------------------------------------------------------------------------------------
  Write a template to a string.
--------------------------------------------------------------------------------------------------*/
static void wrtemp(
   char *temp,
   va_list ap)
{
    uint sArg = countArgs(temp), xArg;
    char *(args[10]);
    char *string, *arg;
    char c;
    int lowerCase = 0, upperCase = 0, caps = 0;

    if(cgiStringBuffer == NULL) {
        cgiStringBufferSize = 42;
        cgiStringBuffer = (char *)calloc(cgiStringBufferSize, sizeof(char));
    }
    cgiStringBufferPosition = 0;
    cgiStringBuffer[0] = '\0';
    for (xArg = 0; xArg < sArg; xArg++) {
        args[xArg] = va_arg(ap, char *);
    }
    string = temp;
    while (*string) {
        c = *string++;
        if (c == '%') {
            c = *string;
            if(c == 'l') {
                lowerCase = 1;
                c = *++string;
            } else if(c == 'u') {
                upperCase = 1;
                c = *++string;
            } else if(c == 'c') {
                caps = 1;
                c = *++string;
            }
            if(isdigit(c)) {
                string++;
                xArg = c - '0';
                if(xArg >= sArg) {
                    printf("wrtemp: not enough args");
                    exit(1);
                }
                if (*args[xArg]) {
                    if(lowerCase) {
                        appendChar((char)tolower(*(args[xArg])));
                        appendString((args[xArg]) + 1);
                        lowerCase = 0;
                    } else if(upperCase) {
                        appendChar((char)toupper(*(args[xArg])));
                        appendString((args[xArg]) + 1);
                        upperCase = 0;
                    } else if(caps) {
                        arg = args[xArg];
                        while(*arg) {
                            appendChar((char)toupper(*arg));
                            arg++;
                        }
                        caps = 0;
                    } else {
                        appendString(args[xArg]);
                    }
                }
            } else if (c == '%') {
                string++;
                appendChar('%');
            }
        } else {
            appendChar(c);
        }
    }
}

/*--------------------------------------------------------------------------------------------------
  Write a template to a string.
--------------------------------------------------------------------------------------------------*/
char *cgiPrintTemplate(
    char *temp,
    ...)
{
    va_list ap;

    va_start(ap, temp);
    wrtemp(temp, ap);
    va_end(ap);
    return cgiStringBuffer;
}

/*--------------------------------------------------------------------------------------------------
  Read a file and return it as a string.
--------------------------------------------------------------------------------------------------*/
char *cgiReadFile(
    char *fileName)
{
    uint bufferSize = 42;
    char *buffer = (char *)calloc(bufferSize, sizeof(char));
    uint bufferPos = 0;
    FILE *file = fopen(fileName, "r");
    int c;

    if(file == NULL) {
        printf("Unable to open file %s\n", fileName);
        exit(1);
    }
    while(1) {
        if(bufferPos == bufferSize) {
            bufferSize += bufferSize >> 1;
            buffer = (char *)realloc(buffer, bufferSize*sizeof(char));
        }
        c = getc(file);
        if(c == EOF) {
            buffer[bufferPos] = '\0';
            return buffer;
        }
        buffer[bufferPos++] = c;
    }
    return NULL; /* Dummy return */
}
