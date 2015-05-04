CC=cc
LWS_OBJ=alloc.o base64-decode.o handshake.o output.o client.o header.o parsers.o client-handshake.o pollfd.o client-parser.o server.o context.o server-handshake.o libwebsockets.o service.o extension.o lws-plat-unix.o sha-1.o extension-deflate-frame.o ssl.o extension-deflate-stream.o

CFLAGS=-g

COMPILE.cc = $(CC) -c $(CFLAGS)

all:	$(LWS_OBJ)

alloc.o:	alloc.c
base64-decode.o:	base64-decode.c
handshake.o:	handshake.c
output.o:	output.c
client.o:	client.c
header.o:	header.c
parsers.o:	parsers.c
client-handshake.o:	client-handshake.c
pollfd.o:	pollfd.c
client-parser.o:	client-parser.c
server.o:	server.c
context.o:	context.c
server-handshake.o:	server-handshake.c
libwebsockets.o:	libwebsockets.c
service.o:	service.c
extension.o:	extension.c
lws-plat-unix.o:	lws-plat-unix.c
sha-1.o:	sha-1.c
extension-deflate-frame.o:	extension-deflate-frame.c
ssl.o:	ssl.c
extension-deflate-stream.o:	extension-deflate-stream.c




clean:
	rm -rf *.o
