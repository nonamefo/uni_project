#include "../libs/Json.hpp"

#include "../crypto/Affine Cipher.cpp"
#include "../crypto/Caesar Cipher.cpp"
#include "../crypto/Playfair Cipher.cpp"
#include "../crypto/Polybius Cipher.cpp"
#include "../crypto/Vigenere Cipher.cpp"

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

        virtual void terminal();

        void analis();

        void get_zero_choise();

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

void super_act_user::terminal(){
    std::string dir = get_curent_dir();

    std::cout << "┌──(user@user)-[" << dir << "]" << std::endl;
    std::cout << "└─$";
}

void super_act_user::get_zero_choise(){
    this->flag_crypt_or_encrypt = false;
}

void super_act_user::analis(){
    // переменная для вывода сообщений пользователю 
    std::string msg;
    // переменная для полученния текста введеным пользователем
    std::string info;

    msg = "1 - зашифровать или расшифровать данные\n"
          "2 - выход                              \n";
    send_message(msg, 2);
    this->terminal();


    
    if(!this->flag_crypt_or_encrypt){
        this->terminal();
        info = get_info();
        if(info[0] == '1'){
            this->flag_crypt_or_encrypt = !this->flag_crypt_or_encrypt;
        }
        else if(info[0] == '2'){
            return;
        }
        else {
            msg = "Данный код не прописан в программе возможно позже он будет добавлен но на данный момент отсутствует";
            send_message(msg, 2);
        }
    }
    else if(this->flag_crypt_or_encrypt){
        // Выбор алгоритма шифрования
        // TODO: вынести в enum и сделать через map (algoitm_creater)
        msg = "Выберете алгоритм шифрования:\n"
              "1 - Аффина                   \n"
              "2 - Цезарь                   \n"
              "3 - Плейфир                  \n"
              "4 - Полибиус                 \n"
              "5 - Вижинер                  \n";
        // отправка сообщения пользавателю для выбора алгоритма
        send_message(msg, 2);

        this->terminal();

        // получение информации
        info = get_info();

        switch(info[0]){
            // метод шифрования Аффины
            case '1':
            {
                // инициализация класса шифрования Аффины
                basic_cripto* affine = new affine_cipher();

                // TODO: вынести логику одинакого 

                msg = "1 - зашифровать тект\n"
                      "2 - расшифровать текст\n";

                send_message(msg, 2);
                this->terminal();

                info = get_info();

                if (info[0] == '1'){
                    // получение текста для зашифрования 
                    msg = "введите текст:";
                    send_message(msg, 2);

                    info = get_info();

                    msg = "зашифрованный текст:";
                    send_message(msg, 2);

                    msg = affine->encode(info);
                    send_message(msg + "\n", 2);
                }
                if (info[0] == '2'){
                    msg = "введите зашифрованный текст:";
                    send_message(msg, 2);

                    info = get_info();

                    msg = "Расшифрованный текст:";
                    send_message(msg, 2);

                    msg = affine->decode(info);
                    send_message(msg + "\n", 2);
                }
                //  отчистка памяти от класса 
                delete affine;
                break;
            }
            case '2':
            {
                basic_cripto* playfir = new playfair("KEYWORD");
                msg = "1 - зашифровать тект\n"
                      "2 - расшифровать текст\n";

                send_message(msg, 2);
                this->terminal();

                info = get_info();

                if (info[0] == '1'){
                    // получение текста для зашифрования 
                    msg = "введите текст:";
                    send_message(msg, 2);

                    info = get_info();

                    msg = "зашифрованный текст:";
                    send_message(msg, 2);

                    msg = playfir->encode(info);
                    send_message(msg + "\n", 2);
                }
                if (info[0] == '2'){
                    msg = "введите зашифрованный текст:";
                    send_message(msg, 2);

                    info = get_info();

                    msg = "Расшифрованный текст:";
                    send_message(msg, 2);

                    msg = playfir->decode(info);
                    send_message(msg + "\n", 2);
                }
                //  отчистка памяти от класса 

                

                delete playfir;
                break;
            }

        }
    }
    analis();
}