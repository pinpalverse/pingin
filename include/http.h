#ifndef __PIN_HTTP
#define __PIN_HTTP

#include <string.h>

typedef enum { GET,
               POST } METHOD;

typedef struct {
  char* s;
  size_t size;
} PINHPM;

typedef struct  // Only HTTP 1.X
{
  // Request main headers
  METHOD method;
  PINHPM path;
  // Response main headers
  int status;
  PINHPM status_reason;  // OK, Not Found, Server Error...
  // Shared
  char* version;

  // General Headers
  PINHPM connection;
  PINHPM cache_control;
  PINHPM date;
  PINHPM pragma;
  PINHPM trailer;
  PINHPM transfer_encoding;
  PINHPM upgrade;
  PINHPM via;
  PINHPM warning;
  // Request Headers
  PINHPM accept;
  PINHPM accept_charset;
  PINHPM accept_encoding;
  PINHPM accept_language;
  PINHPM authorization;
  PINHPM expect;
  PINHPM from;
  PINHPM host;
  PINHPM if_match;
  PINHPM if_modified_since;
  PINHPM if_none_match;
  PINHPM if_range;
  PINHPM if_unmodified_since;
  PINHPM max_forwards;
  PINHPM proxy_authorization;
  PINHPM range;
  PINHPM referer;
  PINHPM te;
  PINHPM user_agent;
  // Response Headers
  PINHPM accept_ranges;
  PINHPM age;
  PINHPM etag;
  PINHPM location;
  PINHPM proxy_authenticate;
  PINHPM retry_after;
  PINHPM server;
  PINHPM vary;
  PINHPM www_authenticate;
  // Entity Headers
  PINHPM allow;
  PINHPM content_encoding;
  PINHPM content_language;
  PINHPM content_length;
  PINHPM content_location;
  PINHPM content_md5;
  PINHPM content_range;
  PINHPM content_type;
  PINHPM expires;
  PINHPM last_modified;
} HTTP;

#endif
