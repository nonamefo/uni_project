#include "user/basic_user.h" 
#include "user/linux_base_user.h"
#include "user/win_base_user.h" 




argagreagegr

int main() {

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
    // TODO
    // add win
    #ifdef __linux__
        basic_user* user = new linux_base_user();
    #elif _WIN32
        basic_user* user = new win_base_user();
    #else 
        basic_user* user = new basic_user();
    #endif
    user->analis();

    return 0;
}
