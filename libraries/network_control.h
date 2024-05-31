

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

        int get_adapter_information(filling_adapter the_adapters) {
            int the_answer;
            #if defined(unix_os)
                the_answer = getifaddrs(the_adapters);
            #else
                DWORD size = 20000;
                do {
                    
                    the_adapters = (filling_adapter) malloc(size);
                    if (!the_adapters) {
                        std::fprintf(stderr, "This stupid windows machine won't share %ld bytes.\n", size);
                        return -1;
                    }

                    int adapt_resp = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, the_adapters, &size);

                    if (adapt_resp == ERROR_BUFFER_OVERFLOW) {
                        std::fprintf(stderr, "Gonna try again. This stupid windows adapter interface wants %ld bytes. Let's see if we can accomopdate it.\n", size);
                        std::free(the_adapters);
                    }

                    else if (adapt_resp == ERROR_SUCCESS) {
                        the_answer = 0;
                        break;
                    }

                    else {
                        std::fprintf(stderr, "Error while retrieving adapter information:\t(Error number %d):\n", get_socket_errno());
                        std::free(the_adapters);
                        the_answer = -1;
                        break;
                    }
                } while (!the_adapters);
            #endif
            return the_answer;
        }

    }

    const std::map<std::string, std::map<std::string, std::vector<std::string> > > get_local_machine_adapters();

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


        adapter the_adapters;

        if (get_adapter_information(pass_adapters_to_filling_info(the_adapters)) == -1) {
            std::fprintf(stderr, "Failed to retrieve adapter information for this machine. Error %d\n", get_socket_errno());
            if (will_throw) {
                if (!was_initialized) {
                    uninitialize_for_crap_os();
                }
                throw network_errors::network_error((char *) "An error has occured. Could not retrieve adapter information. Network error %d\n");
            }
            else {
                if (!was_initialized) {
                    uninitialize_for_crap_os();
                }
            }
            std::exit(EXIT_FAILURE);
        }

        char address_string[basic_buffer_size];
        std::string adapter_name, ip_version, ip_address;

        for (adapter this_adapter = the_adapters; this_adapter; this_adapter = get_next_adapter(this_adapter)) {
            adapter_name = get_adapter_name(this_adapter);
            for (address this_address = get_address(this_adapter); this_address; this_address = get_next_address(this_address)) {
                if (get_ip_version(this_address) == AF_INET || get_ip_version(this_address) == AF_INET6) {
                    ip_version = (get_ip_version(this_address) == AF_INET) ? ip4_const : ip6_const;
                    fill_getnameinfo(this_address, address_string, basic_buffer_size, NI_NUMERICHOST);
                    ip_address = std::string(address_string);

                    if (the_answer.find(adapter_name) == the_answer.end()) {
                        std::map<std::string, std::vector<std::string> > new_dict;
                        std::vector<std::string> new_list;
                        new_list.push_back(ip_address);
                        new_dict.insert(std::make_pair(ip_version, new_list));
                        the_answer.insert(std::make_pair(adapter_name, new_dict));
                        continue;
                    }

                    if (the_answer[adapter_name].find(ip_version) == the_answer[adapter_name].end()) {
                        // the_answer has adapter_name, but adapter_name does not have ip_version
                        std::vector<std::string> new_list;
                        new_list.push_back(ip_address);
                        the_answer[adapter_name].insert(std::make_pair(ip_version, new_list));
                        continue;
                    }

                    the_answer[adapter_name][ip_version].push_back(ip_address);

                }
            }
        }
        free_adapters(the_adapters);

        if (!was_initialized) {
            uninitialize_for_crap_os();
        }

        return the_answer;
    }


};