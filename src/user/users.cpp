#include "user_basic.cpp"
#include "../libs/Json.hpp"

#include "../crypto/Affine Cipher.cpp"
#include "../crypto/Caesar Cipher.cpp"
#include "../crypto/Playfair Cipher.cpp"
#include "../crypto/Polybius Cipher.cpp"
#include "../crypto/Vigenere Cipher.cpp"

#include "../project_libs.hpp"


class act_user : public super_act_user {
    public:
        act_user() : super_act_user() {}
        act_user(std::string message) : super_act_user(message) {}
        std::string get_info();
        void analis();
};

void act_user::analis(){
    // переменная для вывода сообщений пользователю 
    std::string msg;
    // переменная для полученния текста введеным пользователем
    std::string info;

    msg = "1 - зашифровать или расшифровать данные\n"
          "2 - выход                              \n";
    send_message(msg, 2);
    this->linux_base_terminal();


    
    if(!this->flag_crypt_or_encrypt){
        this->linux_base_terminal();
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
        msg = "Выберете алгоритм шифрования:\n"
              "1 - Аффина                   \n"
              "2 - Цезарь                   \n"
              "3 - Плейфир                  \n"
              "4 - Полибиус                 \n"
              "5 - Вижинер                  \n";
        // отправка сообщения пользавателю для выбора алгоритма
        send_message(msg, 2);

        this->linux_base_terminal();

        // получение информации
        info = get_info();

        switch(info[0]){
            // метод шифрования Аффины
            case '1':
            {
                // инициализация класса шифрования Аффины
                basic_cripto* affine = new affine_cipher();

                msg = "1 - зашифровать тект\n"
                      "2 - расшифровать текст\n";

                send_message(msg, 2);
                this->linux_base_terminal();

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
                this->linux_base_terminal();

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

std::string act_user::get_info(){
    std::string info;
    std::cin >> info;
    return info;
}
