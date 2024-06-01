

#if not defined(_INCLUDED_LIBS)
    #include "included.h"
#endif


#define _USEFUL_STUFF_




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

<<<<<<< HEAD
        int get_adapter_information(filling_adapter the_adapters) {
            int the_answer;
            #if defined(unix_os)
                the_answer = getifaddrs(the_adapters);
            #else
                DWORD size = 20000;
                do {
                    
                    the_adapters = (PIP_ADAPTER_ADDRESSES) malloc(size);
                    if (!the_adapters) {
                        std::fprintf(stderr, "This stupid windows machine won't share %ld bytes.\n", size);
                        return -1;
                    }

                    int adapt_resp = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, the_adapters, &size);

                    if (adapt_resp == ERROR_BUFFER_OVERFLOW) {
                        std::fprintf(stderr, "Gonna try again. This stupid windows adapter interface wants %ld bytes. Let's see if we can accomopdate it.\n", size);
                        std::free(the_adapters);
                        the_adapters = NULL;
                    }

                    else if (adapt_resp == ERROR_SUCCESS) {
                        the_answer = 0;
                        break;
                    }

                    else {
                        std::fprintf(stderr, "Error while retrieving adapter information:\t(Error number %d):\n", get_socket_errno());
                        std::free(the_adapters);
                        the_answer = -1;
                        the_adapters = NULL;
                        break;
                    }
                } while (!the_adapters);
            #endif
            return the_answer;
        }

=======
>>>>>>> 296c10cf1ec27c0e37c977d9d7601616b773975f
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
        #if defined(unix_os)
            adapter_type adapters;
            if (getifaddrs(&adapters)) {
                if (!was_initialized) {
                    uninitialize_for_crap_os();
                }
                if (will_throw) {
                    throw network_errors::network_error((char *) "Could not retrieve machine's network adapter information");
                }
                std::exit(EXIT_FAILURE);
            }

            // getifaddrs should return 0 on success, so to get here, we have the adapter information, ideally
            adapter_type this_adapter;
            address_type this_address;
            for (this_adapter = adapters; this_adapter; this_adapter = get_next_adapter(this_adapter)) {
                for (this_address = this_adapter; this_address; this_address = get_next_address(this_address)) {
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
            free_adapters(adapters);
        #else
            
            adapter_type the_adapters;
            DWORD size = 20000;

            the_adapters = NULL;

            while (!the_adapters) {
                the_adapters = (adapter_type) malloc(size);

                if (!the_adapters) {
                    if (was_initialized) {
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
                    if (was_initialized) {
                        uninitialize_for_crap_os();
                    }
                    if (will_throw) {
                        throw network_errors::network_error((char *) "Failed to retrieve this machine's adapters. I'm gonna go now");
                    }
                    std::fprintf(stderr, "Ya, your crappy machine is windows. I don't like spending more time here than I have to, so I'm gonna leave. Here's the error number I encountered.\n", get_socket_errno());
                    std::exit(EXIT_FAILURE);
                }
            }

            adapter_type this_adapter;
            address_type this_address;
            for (this_adapter = the_adapters; this_adapter; get_next_adapter(this_adapter)) {
                for (this_address = this_adapter->FirstUnicastAddress; this_address; get_next_address(this_address)) {
                    if (get_address_family(this_address) == AF_INET || get_address_family(this_address) == AF_INET6) {
                        adapt_name = get_adapter_name(this_adapter);
                        ip_ver = (get_address_family(this_address) == AF_INET) ? ip_4const : ip_6const;
                        memset(addr_buff, 0, basic_buffer_size);
                        get_name_info(this_address, addr_buff, basic_buffer_size);
                        // getnameinfo(this_address->Address.lpSockaddr, this_address->Address.lpSockaddrLength, addr_buff, basic_buffer_size, 0, 0, NI_NUMERICHOST);
                        ip_ver = std::string(addr_buff);

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
                }
            }
            // free(the_adapters);
            free_adapters(the_adapters);

        #endif

        if (!was_initialized) {
            uninitialize_for_crap_os();
        }

        return the_answer;
    }


};