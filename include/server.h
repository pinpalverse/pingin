#include <netinet/in.h>
#include <pinlog/pinlog.h>
#include <pinmem/pinmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  int server_fd;
  struct sockaddr_in addr;
  socklen_t addrlen;
} server_t;

server_t* init_socket(int opt, int port) {
  int server_fd;
  struct sockaddr_in addr;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    pinlog(ERROR, "Socket failed: %s", strerror(errno));
    return NULL;
  }
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    pinlog(ERROR, "Setsockopt failed: %s", strerror(errno));
    close(server_fd);
    return NULL;
  }
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    // pinlog(ERROR, "Binding failed: %s", strerror(errno));
    close(server_fd);
    return NULL;
  }
  server_t* srv = (server_t*)pmalloc(sizeof(server_fd));
  srv->server_fd = server_fd;
  srv->addr = addr;
  srv->addrlen = sizeof(addr);
  return srv;
}
