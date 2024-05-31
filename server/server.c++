


#if not defined(_USEFUL_STUFF_)
    #include "../libraries/network_control.h"
#endif


void print_line_across_window(char c);


void do_the_thing(char* host, char* port);



void list_adapters();



int main(int len, char** args) {

    if (len == 1) {
        std::cerr << "Usage : " << args[0] << " <hostname> <None | port>" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int index;
    for (index = 1; index < len; index = index + 1) {
        if (useful_functions::same_string(args[index], (char *) "list addresses") || useful_functions::same_string(args[index], (char *) "la")) {
            list_adapters();;
        }
        if (useful_functions::same_string(args[1], (char *) "do the thing") || useful_functions::same_string(args[1], (char *) "dtt")) {
            if (len <= index + 1) {
                std::cerr << "Usage " << args[index] << " <dtt> <port = Defaults to 8080, but you can pass in a parameter>" << std::endl;
                continue;
            }
            char* the_port = (char*) "8080";
            if (index + 2 < len && useful_functions::all_nums(args[index + 2])) {
                the_port = args[index + 2];
            }
            do_the_thing(args[index + 1], the_port);
        }
        print_line_across_window('*');
    }

    return 0;
}


void print_line_across_window(char c) {
    int index, len = useful_functions::get_terminal_window_width();
    for (index = 0; index < len; index = index + 1) {
        std::cout << c;
    }
    std::cout << std::endl;
}


void list_adapters() {
    unsigned long index;
    std::map<std::string, std::map<std::string, std::vector<std::string> > > system_adapters = manage_network::get_local_machine_adapters();
    // std::cout << "system_adapters have a size of " << system_adapters.size() << std::endl;

    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator adapt_name = system_adapters.begin(); adapt_name != system_adapters.end(); adapt_name++) {
        
        // std::cout << "Adapter \"" << adapt_name->first << "\":" << std::endl;
        std::printf("Adapter \"%s\"'s:\n", adapt_name->first.c_str());
        for (std::map<std::string, std::vector<std::string> >::const_iterator ip_addresses = adapt_name->second.begin(); ip_addresses != adapt_name->second.end(); ip_addresses++) {
            std::cout << "\t" << ip_addresses->first << "\tAddresses: ";
            for (index = 0; index < ip_addresses->second.size(); index = index + 1) {
                std::cout << ip_addresses->second[index] << ((index + 1 == ip_addresses->second.size()) ? "" : ", ");
            }
            std::printf("\n");
        }
        std::printf("\n");
    }
}



void do_the_thing(char* host, char* port){
    std::cerr << "Accepted parameters hostname <" << host << "> and port <" << port << "> But do_the_thing is still under construction" << std::endl;
}