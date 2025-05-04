#include "../project_libs.hpp"


// класс поведения пользователя что на что он способен и т.д.
class super_act_user{
    public:
        super_act_user();
        super_act_user(std::string);
        
        std::string get_curent_dir();
        std::string setTextColor(const std::string& text, const std::string& colorCode);
        void send_message(std::string msg, char status);
        int get_info();

        void linux_base_terminal();

        bool flag_crypt_or_encrypt = false; 

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

    this->send_message(message, 1);
};


super_act_user::super_act_user(std::string message){
    this->send_message(message, 1);
}
void super_act_user::send_message(std::string msg, char status){
    if (status == 1){
        std::cout << msg << std::endl;
    }
    else if (status == 2){
        std::cout << msg;
    }
     else    {
        std::cout << setTextColor(msg, "31") <<std::endl;
    }    
} 

std::string super_act_user::get_curent_dir() {
    try {
        std::filesystem::path path = std::filesystem::current_path(); // Явное указание
        return path.string();
    } catch (const std::filesystem::filesystem_error& e) { // Явное указание
        std::cerr << "Error getting working directory: " << e.what() << std::endl;
        return "";
    }
}


std::string super_act_user::setTextColor(const std::string& text, const std::string& colorCode) {
    return "\033[" + colorCode + "m" + text + "\033[0m"; // \033[0m сбрасывает цвет
}

void super_act_user::linux_base_terminal(){
    std::string dir = get_curent_dir();

    std::cout << "┌──(user@user)-[" << dir << "]" << std::endl;
    std::cout << "└─$";
}