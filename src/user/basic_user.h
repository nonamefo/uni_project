#ifndef BASIC_USER
#define BASIC_USER

#include "../project_libs.h"

class basic_user{
    public:
        void analis();
        std::string get_info();
        virtual void terminal();
        void send_message(std::string, char);
        void give_choise(std::string);
        char choise;
        std::string get_curent_dir();
};


#endif