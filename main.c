// #include "pinconf/pinconf.h"
#include <pinlog/pinlog.h>
#include <pinmem/pinmem.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "include/http.h"
#include "include/httpdelegate.h"
#include "include/httpparser.h"
#include "include/server.h"

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

int main() {
  int return_out = 0;
  /*
  Conf conf;
  parse("main.pconf", &conf);
  ConfKV* http_dir = search(&conf, "http_dir");
  ConfKV* port = search(&conf, "port");
  ConfKV* https = search(&conf, "https");
  ConfKV* notfound_fallback = search(&conf, "404_fallback");
  ConfKV* connections_to_queue = search(&conf, "n_cncts_to_queue");
  if (!http_dir)
  {
      pinlog(ERROR, "http_dir is not defined in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  } if (!port)
  {
      pinlog(ERROR, "port is not defined in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  } if (!https)
  {
      pinlog(ERROR, "https is not defined in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  } if (!notfound_fallback)
  {
      pinlog(ERROR, "404_fallback is not defined in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  }
  if (!connections_to_queue)
  {
      pinlog(ERROR, "n_cncts_to_queue is not defined in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  }
  if (http_dir->v_type != TEXT)
  {
      pinlog(ERROR, "http_dir type is not text in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  } if (port->v_type != INT)
  {
      pinlog(ERROR, "port type is not int in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  } if (https->v_type != BOOL)
  {
      pinlog(ERROR, "https type is not bool in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  } if (notfound_fallback->v_type != TEXT)
  {
      pinlog(ERROR, "404_fallback type is not text in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  }
  if (connections_to_queue->v_type != INT)
  {
      pinlog(ERROR, "n_cncts_to_queue type is not int in the '%s'", conf.filename);
      return_out = EXIT_FAILURE;
      goto CLEANUP;
  }*/

  /* SOCKET CODE */
  // Socket init
  int ns;
  int opt = 1;
  server_t* srv = init_socket(opt, 8080);
  if (!srv) {
    return_out = EXIT_FAILURE;
    goto CLEANUP;
  }

  const int connections_to_queue = 2;
  if (listen(srv->server_fd, connections_to_queue) < 0) {
    pinlog(ERROR, "Listen: %s", strerror(errno));
    close(srv->server_fd);
    return_out = EXIT_FAILURE;
    goto CLEANUP;
  }
  if ((ns = accept(srv->server_fd, (struct sockaddr*)&srv->addr, (socklen_t*)&srv->addrlen)) < 0) {
    pinlog(ERROR, "Listen: %s", strerror(errno));
    close(srv->server_fd);
    return_out = EXIT_FAILURE;
    goto CLEANUP;
  }
  char* buff = (char*)pmalloc(2048);
  int r = read(ns, buff, 2047);
  buff[r] = '\0';
  HTTP http_struct;
  int s = http_parse(&http_struct, buff, r);
  pinlog(INFO, "Method: %s\n", http_struct.method);
  pinlog(INFO, "Path: %s\n", http_struct.path.s);
  if (s != 0) pinlog(ERROR, "Error occured while parsing the HTTP header");
  // printf("%s\n", buff);
  pinlog(INFO, "Host: %s", http_struct.host.s);
  pinlog(INFO, "User-Agent: %s", http_struct.user_agent.s);

  printf("================================\n");
  char* ok = "OK";
  char* ct = "text/html";
  char* cl = NULL;

  char* payload = "<html><body><h1>Hello</h1><body></html>";
  // Count digits
  int digits = snprintf(NULL, 0, "%ld", strlen(payload));
  cl = (char*)pmalloc((digits * sizeof(char)) + 1);
  memset(cl, '\0', (digits * sizeof(char)) + 1);
  snprintf(cl, digits + 1, "%ld", strlen(payload));
  pinlog(INFO, "Content Length: '%s' '%s'", cl, payload);
  HTTP nh = {
      .method = GET,
      .status = 200,
      .status_reason = {.s = "OK", .size = 3         },
      .content_type = {.s = ct,   .size = strlen(ct)},
      .content_length = {.s = cl,   .size = strlen(cl)}
  };
  char* buffer = create_http_string(&nh, http_versions[0]);
  if (buffer == NULL) pinlog(ERROR, "Failed to make http sring");

  buffer = prealloc(buffer, strlen(buffer) + strlen(payload) + 10);
  sprintf(buffer, "%s\r\n%s", buffer, payload);
  pinlog(INFO, "Buffer: '%s'", buffer);
  send(ns, buffer, strlen(buffer), 0);
  close(ns);
  close(srv->server_fd);
CLEANUP:
  /*
  for (int i = 0; i < conf.columns; i++)
  {
      pfree(conf.values[i]->k);
      pfree(conf.values[i]->v);
      pfree(conf.values[i]);
  }
  pfree(conf.values);
  */
  pfree(buffer);
  pfree(srv);
  pfree(http_struct.path.s);
  pfree(nh.content_length.s);
  http_struct.path.s = NULL;
  http_struct.path.size = 0;
  pfree(http_struct.host.s);
  http_struct.host.s = NULL;
  http_struct.host.size = 0;
  pfree(http_struct.user_agent.s);
  http_struct.user_agent.s = NULL;
  http_struct.user_agent.size = 0;
  pfree(buff);

  return return_out;
}
