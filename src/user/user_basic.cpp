#include "../project_libs.hpp"

// класс поведения пользователя что на что он способен и т.д.
class super_act_user{
    public:
        super_act_user();
        
        std::string encpyte(std::string patern);

    private:
        std::string login;
        std::string username;
        std::string password;

        std::string crypt_patern;
};
super_act_user::super_act_user(){
    // Установка кодовой страницы ввода и вывода

    std::string message =   "#################################################################\n"
                            "#                                                               #\n"
                            "#         Шифровальщик данных                                   #\n"
                            "#                                                               #\n"
                            "#                                                               #\n"
                            "#                                   P.S.                        #\n"
                            "#                                     Над проектом работали:    #\n"
                            "#                                         nonamefo  MstShuher   #\n"
                            "#################################################################\n";

    std::cout << message;
};