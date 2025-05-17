#include "user/users.cpp"

// int main(){
    
//     #ifdef _WIN32
//     SetConsoleCP(CP_UTF8);
//     SetConsoleOutputCP(CP_UTF8);
//     #endif

//     std::string hello_msg = "#################################################################\n"
//                             "#                                                               #\n"
//                             "#         Шифровальщик данных                                   #\n"
//                             "#                                                               #\n"
//                             "#                                                               #\n"
//                             "#                                   P.S.                        #\n"
//                             "#                                     Над проектом работали:    #\n"
//                             "#                                         nonamefo  MstShuher   #\n"
//                             "#################################################################\n"
//                             "                                                                 \n";
//     std::string get_login, get_password, get_username;

//     act_user user = act_user(hello_msg);
//     user.analis();
// }


int main() {
    playfair cipher("KEYWORD");
    std::string message = "HELLO WORLD";
    std::string encrypted = cipher.encode(message);
    std::string decrypted = cipher.decode(encrypted);
    std::cout << "Original: " << message << std::endl;
    std::cout << "Encrypted: " << encrypted << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;
    std::cout << "Key: " << cipher.getKey() << std::endl;
    std::cout << "Matrix: " << cipher.getMatrica() << std::endl;
}