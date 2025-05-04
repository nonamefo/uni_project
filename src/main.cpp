#include "user/users.cpp"

int main(){
    
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    std::string hello_msg = "#################################################################\n"
                            "#                                                               #\n"
                            "#         Шифровальщик данных                                   #\n"
                            "#                                                               #\n"
                            "#                                                               #\n"
                            "#                                   P.S.                        #\n"
                            "#                                     Над проектом работали:    #\n"
                            "#                                         nonamefo  MstShuher   #\n"
                            "#################################################################\n"
                            "                                                                 \n";
    std::string get_login, get_password, get_username;

    act_user user = act_user(hello_msg);
    user.analis();
}