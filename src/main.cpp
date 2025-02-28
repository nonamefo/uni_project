#include <locale.h>

#include "./classes/users.cpp"

int main(){
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::string get_login, get_password, get_username;

    act_user user = act_user(get_login, get_username, get_password);
}