#ifndef __PIN_HTTP_PARSER
#define __PIN_HTTP_PARSER

#include <pinmem/pinmem.h>
#include <pinlog/pinlog.h>
#include <string.h>
static char http_versions[1][4] = {"1.1"};

typedef enum {GET, POST} METHOD;

typedef struct {char *s; size_t size;} PINHPM;

typedef struct  // Only HTTP 1.X 
{
 // Request main headers
    METHOD method;
    PINHPM path;
    // Response main headers
    int status;
    PINHPM status_reason; // OK, Not Found, Server Error...
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

int http_parse(HTTP* http, char* content, int content_size){
    int line = 0;
    char* tk = strtok(content,"\n");
    while(tk != NULL){
        if(tk[0] == 13){goto NEXT;}
        if(line == 0){
            char method[10];
            char path[2048];
            char version[100];
            sscanf(tk, "%s %s %s",method,path,version);
            if(strcmp(method, "GET") == 0) http->method = GET;
            else if(strcmp(method, "POST") == 0) http->method = POST;
            else pinlog(WARN, "HTTP method '%s' unsupported.", method);
            http->path.size = strlen(path);
            http->path.s = (char*)pmalloc(http->path.size);
            strncpy(http->path.s, path, http->path.size);
        }else{
            size_t key_size;
            size_t value_size;
            char* c = strchr(tk, ':');
            key_size = c - tk;
            value_size = strlen(&c[2]); // index is two to skip ':' and whitespace
            if(strncasecmp("host", tk, key_size-1) == 0){
              http->host.size = value_size;
              http->host.s = (char*)pmalloc(value_size*sizeof(char));
              strncpy(http->host.s, &c[2], value_size);
            }else if (strncasecmp("user-agent", tk, key_size-1) == 0){
              http->user_agent.size = value_size;
              http->user_agent.s = (char*)pmalloc(value_size*sizeof(char));
              strncpy(http->user_agent.s, &c[2], value_size);
            
            }

            pinlog(INFO, "%s\n\tKey Size: %d\n\tValue Size: %d",tk, key_size, value_size);
            
        }

NEXT:

        tk = strtok(NULL, "\n");
        line++;
    }
    
    return 0;
}

#endif
