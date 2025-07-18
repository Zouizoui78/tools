#ifndef NETINCLUDES_HPP
#define NETINCLUDES_HPP

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

#elif defined(__linux__)
#include <arpa/inet.h>
#include <netdb.h>

#endif

#endif // NETINCLUDES_HPP
