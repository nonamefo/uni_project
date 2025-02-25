#include "../project_libs.h"

//класс созднан как образец функций которые должны быть воплащены в программе шифрование 
class super_encryption{
    virtual void сaesar(char* message);
    virtual void affine(char* message);
    virtual void vigener(char* message);
    virtual void playfair(char* message);
    virtual void polybius(char* message);
};

//класс созднан как образец функций которые должны быть воплащены в программе расшифровки
class super_decryption{
    virtual void сaesar(char* message);
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
    private:
        std::string login;
        std::string username;
        std::string password;
};
super_act_user::super_act_user(std::string get_login, std::string get_username, std::string get_password){
    login = get_login;
    username = get_username;
    password = get_password;
};