#include "../win_base_user.h"


void win_base_user::terminal(){
    std::cout << "PS " << get_curent_dir() << ">";
}