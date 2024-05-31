

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


    #define valid_socket(sock_no) (sock_no != INVALID_SOCKET)
    #define close_socket(sock_no) (closesocket(sock_no))
    #define get_socket_errno() (WSAGetLastError())

    // TODO : COMPLETE ME
    // Needs to be complete.
    #define get_adapter_name(this_adapter) ()
    #define get_address(an_adapter) ()
    #define get_next_address(this_address) ()
    #define get_ip_version(this_address) ()
    #define fill_getnameinfo(this_address, address_buffer, buffer_size, flag) ()
    #define free_adapters(the_adapters)(free(the_adapters))


    typedef SOCKET sock;
    typedef PIP_ADAPTER_UNICAST_ADDRESS address;
    typedef PIP_ADAPTER_ADDRESSES adapter;
    typedef PIP_ADAPTER_ADDRESSES filling_adapter;



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

    

    #define valid_socket(sock_no) (sock_no >= 0)
    #define close_socket(sock_no) (close(sock_no))
    #define get_socket_errno() (errno)



    #define get_adapter_name(this_adapter) (std::string(this_adapter->ifa_name))
    #define get_address(an_adapter) (an_adapter)
    #define get_next_address(this_address) (NULL)
    #define get_ip_version(this_address) (this_address->ifa_addr->sa_family)
    #define fill_getnameinfo(this_address, address_buffer, buffer_size, flag) (getnameinfo(this_address->ifa_addr, (this_address->ifa_addr->sa_family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), address_buffer, buffer_size, 0, 0, flag))
    #define free_adapters(the_adapters)(freeifaddrs(the_adapters))


    typedef int sock;
    typedef struct ifaddrs* address;
    typedef struct ifaddrs* adapter;
    typedef struct ifaddrs** filling_adapter;

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







#define _INCLUDED_LIBS
#define basic_buffer_size 100
#define kilo_byte 1024





namespace useful_functions {

    inline bool is_caps(const char c) {
        return c >= 'A' && c<= 'Z';
    }


    inline bool is_lower(const char c) {
        return c >= 'a' && c <= 'z';
    }

    inline bool is_letter(const char c) {
        return is_caps(c) || is_lower(c);
    }

    inline bool is_number (const char c) {
        return c >= '0' && c <= '9';
    }

    inline char to_caps(const char c) {
        return (is_lower(c)) ? (c - ('a' - 'A')) : c;
    }

    inline char to_lower(const char c) {
        return (is_caps(c)) ? (c + ('a' - 'A')) : c;
    }

    inline bool same_char(const char a, const char b, bool ignore_case = true) {
        return (ignore_case) ? to_caps(a) == to_caps(b) : a == b;
    }

    inline bool same_string(const char* first, const char* second, bool ignore_case = true) {
        unsigned long index;
        for (index = 0; first[index] != '0' && second[index] != '\0'; index = index + 1) {
            if (!same_char(first[index], second[index], ignore_case)) {
                return false;
            }
        }
        return (first[index] == '\0' && second[index] == '\0');
    }

    inline unsigned long string_length(const char* the_string) {
        unsigned long the_answer;
        for (the_answer = 0; the_string[the_answer] != '\0'; the_answer = the_answer + 1);
        return the_answer;
    }
    
    inline bool all_nums(const char* the_string) {
        unsigned long index;
        for (index = 0; the_string[index] != '\0'; index = index + 1) {
            if (!is_number(the_string[index])) {
                return false;
            }
        }
        return true;
    }

    inline std::string get_current_time() {
        time_t this_time;
        time(&this_time);
        std::string the_answer = std::string(ctime(&this_time));
        return the_answer.substr(0, the_answer.length() - 1);
    }

    #if defined(crap_os)
        std::string ws2string(const std::string& the_string) {
            int len, string_len = (int) the_string.length() + 1;
            len = WideCharToMultiByte(CP_ACP, 0, the_string.c_str(), string_len, 0, 0, 0, 0);
            std::string the_answer(string_len, '\0');
            WideCharToMultiByte(CP_ACP, 0, the_string.c_str(), string_len, &the_answer[0], len, 0, 0);
            return the_answer;
        }
    #endif


    inline int get_terminal_window_width() {
        int the_answer;
        #if defined (crap_os)
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            // int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            // int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            // std::cout << "Terminal width: " << columns << " columns\n";
            // std::cout << "Terminal height: " << rows << " rows\n";
            the_answer = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        #else
            winsize terminal_size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
            // int columns = terminal_size.ws_col;
            // int rows = terminal_size.ws_row;
            // std::cout << "Terminal width: " << columns << " columns\n";
            // std::cout << "Terminal height: " << rows << " rows\n";
            the_answer = terminal_size.ws_col;
        #endif
        return the_answer;
    }




}