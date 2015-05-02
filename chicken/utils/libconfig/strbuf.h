#ifndef __libconfig_strbuf_h
#define __libconfig_strbuf_h

#include <string.h>
#include <sys/types.h>

typedef struct
{
  char *string;
  size_t length;
  size_t capacity;
} strbuf_t;

char *strbuf_release(strbuf_t *buf);

void strbuf_append(strbuf_t *buf, const char *text);

#endif /* __libconfig_strbuf_h */
