#include "../linux_base_user.h"


void linux_base_user::terminal(){
    std::cout << "┌──(" << "user@host" << ")-[" << get_curent_dir() << "]" << std::endl;
    std::cout << "└─$";
}