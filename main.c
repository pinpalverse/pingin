#include "pinconf/pinconf.h"
#include "pinlog/pinlog.h"
#include <asm-generic/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef EXIT_FAILURE
    #define EXIT_FAILURE 1
#endif

int main()
{
    int return_out = 0;
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
    }
    // Socket init
    int server_fd, ns;
    struct sockaddr_in addr;
    int opt = 1;
    socklen_t addrlen = sizeof(addr);
    char *payload = "<html><body><h1>Hello</h1><body></html>";
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        pinlog(ERROR, "Socket failed: %s", strerror(errno));
        return_out = EXIT_FAILURE;
        goto CLEANUP;
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        pinlog(ERROR, "Setsockopt failed: %s", strerror(errno));
        close(server_fd);
        return_out = EXIT_FAILURE;
        goto CLEANUP;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(port->v));
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        pinlog(ERROR, "Binding failed: %s", strerror(errno));
        close(server_fd);
        return_out = EXIT_FAILURE;
        goto CLEANUP;
    }
    if (listen(server_fd, atoi(connections_to_queue->v)) < 0)
    {
        pinlog(ERROR, "Listen: %s", strerror(errno));
        close(server_fd);
        return_out = EXIT_FAILURE;
        goto CLEANUP;
    }
    if ((ns = accept(server_fd, (struct sockaddr *)&addr, &addrlen)) < 0)
    {
        pinlog(ERROR, "Listen: %s", strerror(errno));
        close(server_fd);
        return_out = EXIT_FAILURE;
        goto CLEANUP;
    }
    send(ns, payload, strlen(payload), 0);
    close(ns);
    close(server_fd);
CLEANUP:
    for (int i = 0; i < conf.columns; i++)
    {
        free(conf.values[i]->k);
        free(conf.values[i]->v);
        free(conf.values[i]);
    }
    return return_out;
}