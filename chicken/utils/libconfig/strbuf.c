#include "strbuf.h"

#include <string.h>
#include <stdlib.h>

#define STRING_BLOCK_SIZE 64

/* ------------------------------------------------------------------------- */

char *strbuf_release(strbuf_t *buf)
{
  char *r = buf->string;
  memset(buf, 0, sizeof(strbuf_t));
  return(r);
}

/* ------------------------------------------------------------------------- */

void strbuf_append(strbuf_t *buf, const char *text)
{
  static const size_t mask = ~(STRING_BLOCK_SIZE - 1);
  size_t len = strlen(text);
  size_t newlen = buf->length + len + 1; /* add 1 for NUL */

  if(newlen > buf->capacity)
  {
    buf->capacity = (newlen + (STRING_BLOCK_SIZE - 1)) & mask;
    buf->string = (char *)realloc(buf->string, buf->capacity);
  }

  strcpy(buf->string + buf->length, text);
  buf->length += len;
}

/* ------------------------------------------------------------------------- */
/* eof */
