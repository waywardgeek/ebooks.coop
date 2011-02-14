CFLAGS=-g -DDD_DEBUG
LIBS=-l ddutil-dbg

all: mediaserver mediaclient

mediaserver: mediaserver.c medatabase.c medatabase.h sha256.c sha256.h cgiutil.c cgiutil.h comserver.c comserver.h comclient.h
	gcc $(CFLAGS) mediaserver.c medatabase.c sha256.c cgiutil.c comserver.c $(LIBS) -o mediaserver

medatabase.c medatabase.h: Media.dd
	datadraw Media.dd

mediaclient: mediaclient.c comclient.c comclient.h cgiutil.c cgiutil.h
	gcc $(CFLAGS) mediaclient.c cgiutil.c comclient.c $(LIBS) -o mediaclient
