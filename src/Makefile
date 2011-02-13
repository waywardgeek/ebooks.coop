CFLAGS=-g -DDD_DEBUG
LIBS=-l ddutil-dbg

all: sharealot sharealot_client

sharealot: main.c sadatabase.c sadatabase.h sha256.c sha256.h cgiutil.c cgiutil.h comserver.c comserver.h comclient.h
	gcc $(CFLAGS) main.c sadatabase.c sha256.c cgiutil.c comserver.c $(LIBS) -o sharealot

sadatabase.c sadatabase.h: Share.dd
	datadraw Share.dd

sharealot_client: sharealot_client.c comclient.c comclient.h cgiutil.c cgiutil.h
	gcc $(CFLAGS) sharealot_client.c cgiutil.c comclient.c $(LIBS) -o sharealot_client
