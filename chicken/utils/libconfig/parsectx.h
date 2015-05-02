#ifndef __libconfig_parsectx_h
#define __libconfig_parsectx_h

#include "libconfig.h"
#include "strbuf.h"

struct parse_context
{
  config_t *config;
  config_setting_t *parent;
  config_setting_t *setting;
  char *name;
  strbuf_t string;
};

#define parsectx_init(C)                        \
  memset((C), 0, sizeof(struct parse_context))
#define parsectx_cleanup(C)                             \
  free((void *)(strbuf_release(&((C)->string))))

#define parsectx_append_string(C, S)            \
  strbuf_append(&((C)->string), (S))
#define parsectx_take_string(C)                 \
  strbuf_release(&((C)->string))

#endif /* __libconfig_parsectx_h */
