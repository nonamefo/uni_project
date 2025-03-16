#include "user/users.cpp"

int main(){
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif
    std::string get_login, get_password, get_username;

    act_user user = act_user();
    user.analis();
}