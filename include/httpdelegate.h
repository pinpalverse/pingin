#ifndef __PIN_HTTP_DELEGATE
#define __PIN_HTTP_DELEGATE

#include <pinlog/pinlog.h>
#include <pinmem/pinmem.h>
#include <stdio.h>

#include "http.h"

char* create_http_string(HTTP* http_header, char* http_version) {
  // So far only HTTP
  // The if statements were AI-generated cause there are a lot
  // TODO: Fix the size bounds, cause the size isn't checked for a reallocation
  //        The first attempt is merely for prototyping
  int count = 0;
  int initial_size = 100;
  int current_size = initial_size;
  char* buffer = (char*)pmalloc(initial_size);

  count = snprintf(buffer, initial_size, "HTTP/%s %d %s\r\n", http_version, http_header->status, http_header->status_reason.s);

  // General Headers
  if (http_header->connection.s != NULL && http_header->connection.size > 0) {
    // add Connection header
  }
  if (http_header->cache_control.s != NULL && http_header->cache_control.size > 0) {
    // add Cache-Control header
  }
  if (http_header->date.s != NULL && http_header->date.size > 0) {
    // add Date header
  }
  if (http_header->pragma.s != NULL && http_header->pragma.size > 0) {
    // add Pragma header
  }
  if (http_header->trailer.s != NULL && http_header->trailer.size > 0) {
    // add Trailer header
  }
  if (http_header->transfer_encoding.s != NULL && http_header->transfer_encoding.size > 0) {
    // add Transfer-Encoding header
  }
  if (http_header->upgrade.s != NULL && http_header->upgrade.size > 0) {
    // add Upgrade header
  }
  if (http_header->via.s != NULL && http_header->via.size > 0) {
    // add Via header
  }
  if (http_header->warning.s != NULL && http_header->warning.size > 0) {
    // add Warning header
  }

  // Response Headers
  if (http_header->accept_ranges.s != NULL && http_header->accept_ranges.size > 0) {
    // add Accept-Ranges header
  }
  if (http_header->age.s != NULL && http_header->age.size > 0) {
    // add Age header
  }
  if (http_header->etag.s != NULL && http_header->etag.size > 0) {
    // add ETag header
  }
  if (http_header->location.s != NULL && http_header->location.size > 0) {
    // add Location header
  }
  if (http_header->proxy_authenticate.s != NULL && http_header->proxy_authenticate.size > 0) {
    // add Proxy-Authenticate header
  }
  if (http_header->retry_after.s != NULL && http_header->retry_after.size > 0) {
    // add Retry-After header
  }
  if (http_header->server.s != NULL && http_header->server.size > 0) {
    // add Server header
  }
  if (http_header->vary.s != NULL && http_header->vary.size > 0) {
    // add Vary header
  }
  if (http_header->www_authenticate.s != NULL && http_header->www_authenticate.size > 0) {
    // add WWW-Authenticate header
  }

  // Entity Headers
  if (http_header->allow.s != NULL && http_header->allow.size > 0) {
    // add Allow header
  }
  if (http_header->content_encoding.s != NULL && http_header->content_encoding.size > 0) {
    // add Content-Encoding header
  }
  if (http_header->content_language.s != NULL && http_header->content_language.size > 0) {
    // add Content-Language header
  }
  if (http_header->content_length.s != NULL && http_header->content_length.size > 0) {
    // Content-Length: ...
    if (count >= current_size) {
      buffer = (char*)prealloc(buffer, count + initial_size);
      if (!buffer) {
        pinlog(ERROR, "Cannot reallocate new memory space");
      }
      current_size = count + initial_size;
    }
    count = sprintf(buffer, "%sContent-Length: %s\r\n", buffer, http_header->content_length.s);
  }
  if (http_header->content_location.s != NULL && http_header->content_location.size > 0) {
    // add Content-Location header
  }
  if (http_header->content_md5.s != NULL && http_header->content_md5.size > 0) {
    // add Content-MD5 header
  }
  if (http_header->content_range.s != NULL && http_header->content_range.size > 0) {
    // add Content-Range header
  }
  if (http_header->content_type.s != NULL && http_header->content_type.size > 0) {
    // Content-Type: ...
    if (count >= current_size) {
      buffer = (char*)prealloc(buffer, count + initial_size);
      if (!buffer) {
        pinlog(ERROR, "Cannot reallocate new memory space");
      }
      current_size = count + initial_size;
    }
    count = sprintf(buffer, "%sContent-Type: %s\r\n", buffer, http_header->content_type.s);
  }
  if (http_header->expires.s != NULL && http_header->expires.size > 0) {
    // add Expires header
  }
  if (http_header->last_modified.s != NULL && http_header->last_modified.size > 0) {
    // add Last-Modified header
  }

  return buffer;
}

// HTTP create_basic_response(METHOD method, int status) {
//   return {.method=GET};
// }

PINHPM create_http_pinhpm() {}

#endif
