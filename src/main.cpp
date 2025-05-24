#include "user/basic_user.h" 
#include "user/linux_base_user.h" 

// TODO
// #include "user/win_base_user.h"

<<<<<<< HEAD




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
    #else 
        basic_user* user = new basic_user();
    #endif
    user->analis();

    return 0;
}
=======
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


// int main() {
//     basic_cripto playfair = new playfair("KEYWORD");
//     std::string message = "HELLO WORLD";
//     std::string encrypted = cipher.encode(message);
//     std::string decrypted = cipher.decode(encrypted);
//     std::cout << "Original: " << message << std::endl;
//     std::cout << "Encrypted: " << encrypted << std::endl;
//     std::cout << "Decrypted: " << decrypted << std::endl;
//     std::cout << "Key: " << cipher.getKey() << std::endl;
//     std::cout << "Matrix: " << cipher.getMatrica() << std::endl;
// }
>>>>>>> 1a0042f (.)
