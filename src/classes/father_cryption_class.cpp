#include <string>
#include <vector>
#include <iostream>
#include <windows.h>


//класс созднан как образец функций которые должны быть воплащены в программе шифрование 
class super_encryption{
    virtual void caesar(char* message);
    virtual void affine(char* message);
    virtual void vigener(char* message);
    virtual void playfair(char* message);
    virtual void polybius(char* message);
};

//класс созднан как образец функций которые должны быть воплащены в программе расшифровки
class super_decryption{
    virtual void caesar(char* message);
    virtual void affine(char* message);
    virtual void vigener(char* message);
    virtual void playfair(char* message);
    virtual void polybius(char* message);
};

// класс поведения пользователя что на что он способен и т.д.
class super_act_user{
    public:
        super_act_user();
        super_act_user(
            std::string login,
            std::string username,
            std::string password
        );
        std::vector<std::string> encpyte(std::string patern);
    private:
        std::string login;
        std::string username;
        std::string password;

        std::string crypt_patern;
};
super_act_user::super_act_user(std::string get_login, std::string get_username, std::string get_password){
    // Установка кодовой страницы ввода и вывода

    std::string message =   "#################################################################\n"
                            "#                                                               #\n"
                            "#         Шифровальщик данных                                   #\n"
                            "#                                                               #\n"
                            "#                                                               #\n"
                            "#                                   P.S.                        #\n"
                            "#                                     Над проектом работали:    #\n"
                            "#                                         nonamefo  MstShuher   #\n"
                            "#################################################################\n"
                            "                                                                 \n"
                            "     1 -- зашифровать сообщение                                  \n"
                            "     2 -- расшифровать сообщение                                 \n"
                            "                                                                 \n"
                            "user>";

    std::string command;

    std::cout << message;

    std::cin >> command;

    login = get_login;
    username = get_username;
    password = get_password;
};