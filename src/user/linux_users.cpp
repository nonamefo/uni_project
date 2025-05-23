#include "user_basic.cpp"


class linux_act_user : public super_act_user {
    public:
        act_user() : super_act_user() {}
        act_user(std::string message) : super_act_user(message) {}
        
        void analis();
};



void linux_act_user::terminal(){
    std::string dir = get_curent_dir();

    std::cout << "┌──(user@user)-[" << dir << "]" << std::endl << "└─$";
}