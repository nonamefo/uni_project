#include "../project_libs.hpp"
#include "../libs/Json.hpp"


// класс поведения пользователя что на что он способен и т.д.
class super_act_user{
    public:
        super_act_user();
        json js;
        std::string get_curent_dir();
        std::string setTextColor(const std::string& text, const std::string& colorCode);
        void send_message(std::string msg, int status);
        int get_info();
        bool get_element(char ids);

        bool flag_crypt_or_encrypt; 

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
                            "#################################################################\n"
                            "                                                                 \n"
                            "                                                                 \n"
                            "         1 - Affine шифрование                                   \n"
                            "         2 - Цезарь шифр                                         \n"
                            "         3 - вЫхОд                                               \n";

    this->send_message(message, 1);
};

void super_act_user::send_message(std::string msg, int status){
    js = json("./list_algoritms.json");
    if (status){
        std::cout << msg << std::endl;
    }   else    {
        std::cout << setTextColor(msg, "31") <<std::endl;
    }
    std::string dir = get_curent_dir();

    std::cout << "┌──(user@user)-[" << dir << "]" << std::endl;
    std::cout << "└─$";
    
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
    return "\033[" + colorCode + "m" + text + "\033[0m\n"; // \033[0m сбрасывает цвет
}
