

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)

    #if not defined(crap_os)
        #define crap_os
    #endif

    #if not defined(_WIN32_WINNT)
        #define _WIN32_WINNT 0x600
    #endif

    // find the flags for each of these libraries
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>

    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")


    #include <windows.h>

    #define adapter_type PIP_ADAPTER_ADDRESSES
    #define address_type PIP_ADAPTER_UNICAST_ADDRESS

    #define valid_socket(sock_no) (sock_no != INVALID_SOCKET)
    #define close_socket(sock_no) (closesocket(sock_no))
    #define get_socket_errno() (WSAGetLastError())
    #define get_socket_errno_string(error_num) ((char *) "An error has occured and I'm a stupid windows sysem so I can't really tell you much about it...")

    #define get_next_adapter(this_adapter) (this_adapter->Next)
    #define get_address(this_adapter) (this_adapter->FirstUnicastAddress)
    #define get_adapter_name(this_adapter) (useful_functions::ws2string(this_adapter->FriendlyName))
    #define get_next_address(this_address) (this_address->Next)
    #define get_address_family(this_address) (this_address->Address.lpSockaddr->sa_family)
    #define get_name_info(this_address, buff, buff_size) (getnameinfo(this_address->Address.lpSockaddr, this_address->Address.iSockaddrLength, buff, buff_size, 0, 0, NI_NUMERICHOST))
    #define free_adapters(the_adapters) (free(the_adapters));

    #define sock_type SOCKET
    #define invalid_socket INVLAID_SOCKET



#else

    #if not defined(unix_os)
        #define unix_os
    #endif

    #if defined(__APPLE__) || defined(__APPLE_CC__)
        #define mac_os
    #else
        #define linux_os
    #endif

    
    // for getting network adapters
    #if not defined(_IFADDRS_H)
        #include <ifaddrs.h>
    #endif

    #if not defined(_SYS_TYPES_H)
        #include <sys/types.h>
    #endif


    #if not defined(_SYS_SOCKET_H)
        #include <sys/socket.h>
    #endif


    #if not defined(_NETINET_IN_H)
        #include <netinet/in.h>
    #endif

    #if not defined(_ARPA_INET_H)
        #include <arpa/inet.h>
    #endif

    #if not defined(_NETDB_H)
        #include <netdb.h>
    #endif


    #if not defined(_UNISTD_H)
        #include <unistd.h>
    #endif

    #if not defined(_ERRNO_H)
        #include <errno.h>
    #endif

    #if not defined(_SYS_IOCTL_H)
        #include <sys/ioctl.h>
    #endif

    

    #define adapter_type struct ifaddrs*
    #define address_type struct ifaddrs*

    #define valid_socket(sock_no) (sock_no >= 0)
    #define close_socket(sock_no) (close(sock_no))
    #define get_socket_errno() (errno)
    #define get_socket_errno_string(error_num) (gai_strerror(error_num))

    #define get_next_adapter(this_adapter) (this_adapter->ifa_next)
    #define get_address(this_adapter) (this_adapter)
    #define get_adapter_name(this_adapter) (std::string(this_adapter->ifa_name))
    #define get_next_address(this_address) (NULL)
    #define get_address_family(this_address) (this_address->ifa_addr->sa_family)
    #define get_name_info(this_address, buff, buff_size) (getnameinfo(this_address->ifa_addr, (get_address_family(this_address) == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), buff, buff_size, 0, 0, NI_NUMERICHOST))
    #define free_adapters(the_adapters) (freeifaddrs(the_adapters))

    #define sock_type int
    #define invalid_socket -1

#endif






#if not defined(__EXCEPTION__)
    #include <exception>
#endif




#if not defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif




#if not defined(_GLIBCXX_MAP)
    #include <map>
#endif




#if not defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif





#if not defined(_TIME_H)
    #include <time.h>
#endif






#if defined(crap_os) && not defined(_INC_STRING) || defined(unix_os) && not defined(_STRING_H)
    #include <string.h>
#endif

#if defined(crap_os) && not defined(_STRING_)
    #include <string>
#endif







#define _INCLUDED_LIBS
#define basic_buffer_size 100
#define kilo_byte 1024

