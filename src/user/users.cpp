#include "user_basic.cpp"
#include "../libs/Json.hpp"

#include "../project_libs.hpp"


class act_user : public super_act_user {
    public:
        act_user() : super_act_user() {}
        std::string get_info();
        void analis();
        void crypt();
};


void act_user::analis(){
    std::string msg;
    std::string info = get_info();
    bool flag = 1;

    if(info[0] == '1'){
        this->flag_crypt_or_encrypt = true;
        this->crypt();
    }   else if (info[0] == '2')    {
        this->flag_crypt_or_encrypt = false;
    }   else if (info[0] == '3'){
        return;
    }   else    {
        std::cerr << setTextColor("[status: ERROR] message:flag is out of range", "31");
        flag = !flag;
    }
    msg ;
    send_message(msg, flag);
    this->analis();
}

std::string act_user::get_info(){
    std::string info;
    std::cin >> info;
    return info;
}

void act_user::crypt(){
    json js = json();
    js.Parse("list_algorims.json");
    js.cout_json();
}