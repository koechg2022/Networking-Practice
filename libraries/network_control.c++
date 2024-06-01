




#if not defined(_INCLUDED_LIBS)
    #include "included.h"
#endif


#define _USEFUL_STUFF_


namespace useful_functions {

    /*
    * @brief Check if the char passed in is a capital letter or not.
    */
    bool is_caps(const char c);


    /*
    * @brief Check if the char passed in is a lower case letter or not.
    */
    bool is_lower(const char c);

    bool is_letter(const char c);

    bool is_number (const char c);

    char to_caps(const char c);

    char to_lower(const char c);

    bool same_char(const char a, const char b, bool ignore_case = true);

    bool same_string(const char* first, const char* second, bool ignore_case = true);

    unsigned long string_length(const char* the_string);

    bool all_nums(const char* the_string);

    std::string get_current_time();

    std::string ws2string(const std::wstring& wstr);

    int get_terminal_window_width();


    bool is_caps(const char c) {
        return c >= 'A' && c<= 'Z';
    }


    bool is_lower(const char c) {
        return c >= 'a' && c <= 'z';
    }


    bool is_letter(const char c) {
        return is_caps(c) || is_lower(c);
    }

    bool is_number (const char c) {
        return c >= '0' && c <= '9';
    }

    char to_caps(const char c) {
        return (is_lower(c)) ? (c - ('a' - 'A')) : c;
    }

    char to_lower(const char c) {
        return (is_caps(c)) ? (c + ('a' - 'A')) : c;
    }

    bool same_char(const char a, const char b, bool ignore_case) {
        return (ignore_case) ? to_caps(a) == to_caps(b) : a == b;
    }

    bool same_string(const char* first, const char* second, bool ignore_case) {
        unsigned long index;
        for (index = 0; first[index] != '0' && second[index] != '\0'; index = index + 1) {
            if (!same_char(first[index], second[index], ignore_case)) {
                return false;
            }
        }
        return (first[index] == '\0' && second[index] == '\0');
    }

    unsigned long string_length(const char* the_string) {
        unsigned long the_answer;
        for (the_answer = 0; the_string[the_answer] != '\0'; the_answer = the_answer + 1);
        return the_answer;
    }
    
    bool all_nums(const char* the_string) {
        unsigned long index;
        for (index = 0; the_string[index] != '\0'; index = index + 1) {
            if (!is_number(the_string[index])) {
                return false;
            }
        }
        return true;
    }

    std::string get_current_time() {
        time_t this_time;
        time(&this_time);
        std::string the_answer;
        #if defined(crap_os)
            char time_buff[basic_buffer_size];
            if (ctime_s(time_buff, basic_buffer_size, &this_time)) {
                the_answer = std::string(time_buff);
            }
            else {
                std::fprintf(stderr, "Error retrieving current time: %d\n", get_socket_errno());
                std::exit(EXIT_FAILURE);
            }
        #else
            the_answer = std::string(ctime(&this_time));
        #endif

        return the_answer.substr(0, the_answer.length() - 1);
    }

    std::string ws2string(const std::wstring& wstr) {
        std::string the_answer;
        the_answer.reserve(wstr.size());

        for (wchar_t wc : wstr) {
            if (wc <= 0x7f) {
                the_answer.push_back(static_cast<char>(wc));
            }
            else if (wc <= 0x7ff) {
                the_answer.push_back(0xc0 | (wc >> 6));
                the_answer.push_back(0x80 | (wc & 0x3f));
            }
            else if (wc <= 0xffff) {
                the_answer.push_back(0xe0 | (wc >> 12));
                the_answer.push_back(0x80 | ((wc >> 6) & 0x3f));
                the_answer.push_back(0x80 | (wc & 0x3f));
            }
        #if defined(crap_os)
            else {
        #else
            else if (wc <= 0x10ffff) {
        #endif
                the_answer.push_back(0xf0 | (wc >> 18));
                the_answer.push_back(0x80 | ((wc >> 12) & 0x3f));
                the_answer.push_back(0x80 | ((wc >> 6) & 0x3f));
                the_answer.push_back(0x80 | (wc & 0x3f));
            }
        }

        return the_answer;
    }


    int get_terminal_window_width() {
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




namespace manage_network {


    namespace network_errors {

        class memory_error {
            private:
                char* msg;

            public:

                memory_error(char* msg) {
                    this->msg = msg;
                }

                ~memory_error() throw() {
                    return;
                }

                char* what() {
                    return this->msg;
                }
        };



        class network_error {
            private:
                char* msg;

            public:

                network_error(char* msg) {
                    this->msg = msg;
                }

                ~network_error() throw() {
                    return;
                }

                char* what() {
                    return this->msg;
                }
        };



        class not_yet_implemented {
            private:
                char* msg;

            public:

                not_yet_implemented(char* msg) {
                    this->msg = msg;
                }

                ~not_yet_implemented() throw() {
                    return;
                }

                char* what() {
                    return this->msg;
                }
        };


    }


    namespace {
        bool initialized = false, will_throw = true;
        const std::string ip4_const = "IP Version 4";
        const std::string ip6_const = "IP Version 6";


        void initialize_network() {
            #if defined(crap_os)
                if (!initialized) {
                    WSADATA d;
                    if (WSAStartup(MAKEWORD(2, 2), &d)) {
                        initialized = false;
                        if (will_throw) {
                            throw network_errors::network_error((char *) "Failed to initialize the network for this crappy Operating System.");
                        }
                    }
                    initialized = true;
                }
            #endif
        }


        void clean_up() {
            #if defined(crap_os)
                if (initialized) {
                    WSACleanup();
                }

            #endif
        }


        inline bool is_initialized() {
            #if defined(crap_os)
                return initialized;
            #else
                return true;
            #endif
        }

    }

    // const std::map<std::string, std::map<std::string, std::vector<std::string> > > get_local_machine_adapters();

    inline void initialize_network_for_crap_os() {
        initialize_network();
    }

    inline bool network_is_initialized() {
        return initialized;
    }

    inline void uninitialize_for_crap_os() {
        clean_up();
    }

    const std::map<std::string, std::map<std::string, std::vector<std::string> > > get_local_machine_adapters() {
        std::map<std::string, std::map<std::string, std::vector<std::string> > > the_answer;
        bool was_initialized = false;
        if (!is_initialized()) {
            initialize_network_for_crap_os();
        }
        else {
            was_initialized = true;
        }

        std::string adapt_name, ip_ver, ip_addr;
        char addr_buff[basic_buffer_size];

        // Get the adapter information on each system
        adapter_type the_adapters;
        #if defined(unix_os)
            // adapter_type the_adapters;
            if (getifaddrs(&the_adapters)) {
                if (!was_initialized) {
                    uninitialize_for_crap_os();
                }
                if (will_throw) {
                    throw network_errors::network_error((char *) "Could not retrieve machine's network adapter information");
                }
                std::exit(EXIT_FAILURE);
            }

            // getifaddrs should return 0 on success, so to get here, we have the adapter information, ideally
        #else
            

            the_adapters = NULL;

            while (!the_adapters) {
                DWORD size = 20000;
                the_adapters = (adapter_type) malloc(size);

                if (!the_adapters) {
                    if (!was_initialized) {
                        uninitialize_for_crap_os();
                    }
                    if (will_throw) {
                        throw network_errors::memory_error((char *) "Could not retrieve necessary memory for the adapters. God windows sucks and I can't believe I have to accomopdate this Crap OS.\n");
                    }
                    std::fprintf(stderr, "Failed to get the necessary memory for the adapters. God windows sucks and I can't believe I have to accomopdate this Crap OS.\n");
                    std::exit(EXIT_FAILURE);
                }

                int adapt_resp = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, the_adapters, &size);

                if (adapt_resp == ERROR_BUFFER_OVERFLOW) {
                    std::printf("Your crappy windows machine needs %ld bytes to give me the adapters of interest. I'm gonna try again to get those bytes.\n", size);
                    std::free(the_adapters);
                    the_adapters = NULL;
                }

                else if (adapt_resp == ERROR_SUCCESS) {
                    break;
                }

                else {
                    std::free(the_adapters);
                    if (!was_initialized) {
                        uninitialize_for_crap_os();
                    }
                    if (will_throw) {
                        throw network_errors::network_error((char *) "Failed to retrieve this machine's adapters. I'm gonna go now");
                    }
                    std::fprintf(stderr, "Ya, your crappy machine is windows. I don't like spending more time here than I have to, so I'm gonna leave. Here's the error number I encountered %d\n", get_socket_errno());
                    std::exit(EXIT_FAILURE);
                }
            }

        #endif

        adapter_type this_adapter;
        address_type this_address;
        for (this_adapter = the_adapters; this_adapter; this_adapter = get_next_adapter(this_adapter)) {
            for (this_address = get_address(this_adapter); this_address; this_address = get_next_address(this_address)) {
                if (get_address_family(this_address) == AF_INET || get_address_family(this_address) == AF_INET6) {
                    adapt_name = get_adapter_name(this_adapter);
                    ip_ver = (get_address_family(this_address) == AF_INET) ? ip4_const : ip6_const;
                    memset(addr_buff, 0, basic_buffer_size);
                    get_name_info(this_address, addr_buff, basic_buffer_size);
                    ip_addr = std::string(addr_buff);

                    if (the_answer.find(adapt_name) == the_answer.end()) {
                        std::vector<std::string> new_list;
                        new_list.push_back(ip_addr);
                        std::map<std::string, std::vector<std::string> > new_map;
                        new_map.insert(std::make_pair(ip_ver, new_list));
                        the_answer.insert(std::make_pair(adapt_name, new_map));
                        continue;
                    }

                    if (the_answer[adapt_name].find(ip_ver) == the_answer[adapt_name].end()) {
                        std::vector<std::string> new_list;
                        new_list.push_back(ip_addr);
                        the_answer[adapt_name].insert(std::make_pair(ip_ver, new_list));
                        continue;
                    }

                    the_answer[adapt_name][ip_ver].push_back(ip_addr);
                }
                break;
            }
        }
        free_adapters(the_adapters);

        if (!was_initialized) {
            uninitialize_for_crap_os();
        }

        return the_answer;
    }


    void single_server_run(const char* port_no, int listen_count = 10) {
        bool was_init = false;
        if (is_initialized()) {
            was_init = true;
        }

        struct addrinfo* this_machine, init_vals;
        memset(&init_vals, 0, sizeof(init_vals));
        init_vals.ai_family = AF_UNSPEC;
        init_vals.ai_socktype = SOCK_STREAM;
        init_vals.ai_flags = AI_PASSIVE;


        if (getaddrinfo(0, port_no, &init_vals, &this_machine)) {
            if (!was_init) {
                uninitialize_for_crap_os();
            }
            if (will_throw) {
                throw network_errors::network_error((char *) ("Erorr retrieving adapter information for binding adapter address. Error (" + std::to_string(get_socket_errno()) + ") " + "\t(" + std::string(get_socket_errno_string(get_socket_errno())) + ")").c_str());
            }
            std::fprintf(stderr, "Error retrieving adapter information for a binding adapter address. Error %d, (%s)\n", get_socket_errno(), get_socket_errno_string(get_socket_errno()));
            std::exit(EXIT_FAILURE);
        }

        sock_type listening_socket = socket(this_machine->ai_family, this_machine->ai_socktype, this_machine->ai_protocol);
        if (!valid_socket(listening_socket)) {
            if (!was_init) {
                uninitialize_for_crap_os();
            }
            if (will_throw) {
                throw network_errors::network_error((char *) ("Erorr creating listening socket. Error (" + std::to_string(get_socket_errno()) + ") " + "\t(" + std::string(get_socket_errno_string(get_socket_errno())) + ")").c_str());
            }
            std::fprintf(stderr, "Error creating listening socket. Error %d, (%s)\n", get_socket_errno(), get_socket_errno_string(get_socket_errno()));
            std::exit(EXIT_FAILURE);
        }

        if (bind(listening_socket, this_machine->ai_addr, this_machine->ai_addrlen)) {
            if (!was_init) {
                uninitialize_for_crap_os();
            }
            if (will_throw) {
                throw network_errors::network_error((char *) ("Error error binding the listening socket. Error " + std::to_string(get_socket_errno()) + ", (" + std::string(get_socket_errno_string(get_socket_errno())) + ")").c_str());
            }
            std::fprintf(stderr, "Error binding the listening socket. Error %d, (%s)\n", get_socket_errno(), get_socket_errno_string(get_socket_errno()));
            std::exit(EXIT_FAILURE);
        }

        // socket is bound.

        if (listen(listening_socket, listen_count)) {
            if (!was_init) {
                uninitialize_for_crap_os();
            }
            if (will_throw) {
                throw network_errors::network_error((char *) ("Error setting the listening socket to start listening. Error " + std::to_string(get_socket_errno()) + ", (" + std::string(get_socket_errno_string(get_socket_errno())) + ")").c_str());
            }
            std::fprintf(stderr, "Error setting the listening socket to start listening. Error %d, (%s)\n", get_socket_errno(), get_socket_errno_string(get_socket_errno()));
            std::exit(EXIT_FAILURE);
        }

        struct sockaddr_storage client_info;
        socklen_t client_size = sizeof(client_info);

        sock_type client_socket = accept(listening_socket, (struct sockaddr*) &client_info, &client_size);

        if (!valid_socket(client_socket)) {
            if (!was_init) {
                uninitialize_for_crap_os();
            }
            if (will_throw) {
                throw network_errors::network_error((char *) ("Error setting the new client's socket. Error " + std::to_string(get_socket_errno()) + ", (" + get_socket_errno_string(get_socket_errno()) + ")").c_str());
            }
            std::fprintf(stderr, "Error setting the new client's socket. Error %d, (%s)\n", get_socket_errno(), get_socket_errno_string(get_socket_errno()));
            std::exit(EXIT_FAILURE);
        }

        char address_buff[basic_buffer_size];
        char service_buff[basic_buffer_size];
        getnameinfo((struct sockaddr*) &client_info, client_size, address_buff, basic_buffer_size, service_buff, basic_buffer_size, NI_MAXHOST | NI_MAXSERV);

        std::printf("Connection from %s\n\tService %s\n", address_buff, service_buff);

        char request[kilo_byte];
        int bytes_received, bytes_sent;
        std::string from_client = "";
        unsigned long received_total = 0, total_sent;
        received_total = recv(client_socket, request, kilo_byte, 0);
        from_client = std::string(request, received_total);
        std::printf("Received %lu bytes from the client: \"%s\"\n", received_total, from_client.c_str());


        // Sending response....
        const char* response_head = "HTTP/1.1 200 OK\r\n" "Connection: close\r\n" "Content-Type: text/plain\r\n\r\n" "Local time is: ";
        total_sent = send(client_socket, response_head, useful_functions::string_length(response_head), 0);
        std::string to_send = useful_functions::get_current_time() + "\n\n\n";
        total_sent = total_sent + send(client_socket, to_send.c_str(), to_send.length(), 0);

        std::printf("Opening file to read data to the client.\n");
        std::FILE* html_file = std::fopen("files/quelques-femmes.html", "r");
        if (!html_file) {
            // do something else
            to_send = "Could not open quelques-femmes.html file to send it's data over";
            total_sent = total_sent + send(client_socket, to_send.c_str(), to_send.length(), 0);
        }
        else {
            while ((bytes_sent = fread(request, sizeof(char), kilo_byte, html_file)) > 0) {
                total_sent = total_sent + send(client_socket, request, bytes_sent, 0);
            }
            std::fclose(html_file);
        }

        std::printf("Sent a total of %lu bytes...\n", total_sent);
        close_socket(client_socket);
        close_socket(listening_socket);
        if (!was_init) {
            uninitialize_for_crap_os();
        }
    }


};