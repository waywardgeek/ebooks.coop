/*--------------------------------------------------------------------------------------------------
  The client communicates with the server using:

      void coSendMessage(char *format, ...);
      char *coReadMessage(void);

  Typically, a message will be just like a command that would normally be issued through a console
  interface to the server.  It's typically one line, and newline terminated.  The server will
  respond to each message sent, even if no response is expected.  In this case, the response
  message will be empty.

  The client does so with:

      char *coStartClient(char *fileSocketPath, char *sessionId);
      void coStopClient(void);

  The server is expected to send an initial hello message of some sort, which is returned by
  coStartClient.

  The client may also want to receive messages from the server asyncronously.  This can be used
  to enable communication between clients in a bus-style, for example.  Typically, the client will
  have a listening thread devoted to this.  Commands are:

      char *coStartListener(char *fileSocketPath, char *sessionId);
      void coStopListener(void);
      char *coWaitForMessage(void);
      void coSendResponse(char *format, ...);

  A client can be a pure listener, without needing to call coStartClient.  If a client is both,
  it needs to use the same sessionId in coStartClient and coStartListener.

--------------------------------------------------------------------------------------------------*/

/* Largest string that can be created with coSendMessage. */
#define CO_MAX_MESSAGE_LENGTH 4096
char *coStartClient(char *fileSocketPath, char *sessionId);
void coStopClient(void);
void coSendMessage(char *format, ...);
/* Note that a pointer to a static buffer is returned, which will be over-written by the next
   call to coReadMessage.  It ok to modify the return value in-place. */
char *coReadMessage(void);

/* Listener functions */
char *coStartListener(char *fileSocketPath, char *sessionId);
void coStopListener(void);
char *coWaitForMessage(void);
void coSendResponse(char *format, ...);
