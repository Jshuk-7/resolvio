#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

const void* get_inet_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &((struct sockaddr_in*)sa)->sin_addr;
    }

    return &((struct sockaddr_in6*)sa)->sin6_addr;
}

int32_t resolve_domain(const char* domain)
{
    struct addrinfo* hints;

    memset(hints, 0, sizeof hints);
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;

    struct addrinfo* res = NULL;

    int32_t status = getaddrinfo(domain, NULL, hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    char* ipver = NULL;
    char ipstr[INET6_ADDRSTRLEN];
    struct addrinfo* it = NULL;

    printf("resolving domain: %s ...\n", domain);

    for (it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET)
        {
            ipver = "IPv4";
        }
        else
        {
            ipver = "IPv6";
        }
        inet_ntop(it->ai_family, get_inet_addr(it->ai_addr), ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);

    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: resolvio <domain>\n");
        return 1;
    }

    return resolve_domain(argv[1]);
}