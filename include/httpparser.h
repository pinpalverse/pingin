#ifndef __PIN_HTTP_PARSER
#define __PIN_HTTP_PARSER


static char http_versions[1][4] = {"1.1"};

typedef enum {GET, POST} METHOD;

typedef struct {char *s; int size; int tracker;} PINM;

typedef struct  // Only HTTP 1.X 
{
    // Request main headers
    METHOD method;
    PINM path;
    // Response main headers
    int status;
    PINM status_reason; // OK, Not Found, Server Error...
    // Shared
    char* version;

    // General Headers
    PINM connection;
    PINM cache_control;
    PINM date;
    PINM pragma;
    PINM trailer;
    PINM transfer_encoding;
    PINM upgrade;
    PINM via;
    PINM warning;
    // Request Headers
    PINM accept;
    PINM accept_charset;
    PINM accept_encoding;
    PINM accept_language;
    PINM authorization;
    PINM expect;
    PINM from;
    PINM host;
    PINM if_match;
    PINM if_modified_since;
    PINM if_none_match;
    PINM if_range;
    PINM if_unmodified_since;
    PINM max_forwards;
    PINM proxy_authorization;
    PINM range;
    PINM referer;
    PINM te;
    PINM user_agent;
    // Response Headers
    PINM accept_ranges;
    PINM age;
    PINM etag;
    PINM location;
    PINM proxy_authenticate;
    PINM retry_after;
    PINM server;
    PINM vary;
    PINM www_authenticate;
    // Entity Headers
    PINM allow;
    PINM content_encoding;
    PINM content_language;
    PINM content_length;
    PINM content_location;
    PINM content_md5;
    PINM content_range;
    PINM content_type;
    PINM expires;
    PINM last_modified;
} HTTP;

int http_parse(HTTP* http, char* content, int content_size){
    return 0;
}

#endif
