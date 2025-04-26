#include "user_basic.cpp"
#include "../project_libs.hpp"
#include "../crypto/basic_cripto.h"
#include "../crypto/Affine Cipher.cpp"
#include "../crypto/Caesar Cipher.cpp"




class act_user : public super_act_user {
    public:
        act_user() : super_act_user() {analis(js);}
        std::string get_info();
        void crypt();
        void analis(json js);
        
        
};


void act_user::analis(json js){
    std::string dir = get_curent_dir();
    std::string msg, text;
    char message;
    std::string info = get_info();
    bool flag = 1;
    auto key = js.get_keys();

    basic_cripto* affine = new affine_cipher();
    basic_cripto* caesar = new caesar_cipher();

    switch(info[0]){
        // affine алогритм 
        case '1': {
            std::cout << "1 - зашифровать данные" << std::endl << "2 - расшифрование данные" << std::endl;


            std::cout << "┌──(user@user)-[" << dir << "]" << std::endl;
            std::cout << "└─$";
            
            std::cin >> message;


            if(message == '1') {  
                std::cout << "Введите текст для шифрования: ";  
                std::cin.ignore(); 

                std::getline(std::cin, text);  

                std::string encryptedText = affine->encode(text);  

                msg = "Зашифрованный текст: " + encryptedText;  


            } else if(message == '2') {  
                std::cout << "Введите текст для расшифрования: ";  
                std::cin.ignore();  
                std::getline(std::cin, text);  
                std::string decryptedText = affine->decode(text);  
                msg = "Расшифрованный текст: " + decryptedText; 

                std::cout << "┌──(user@user)-[" << dir << "]" << std::endl << "└─$";
 
            } else {  
                msg = "Неверный ввод. Пожалуйста, введите 1 или 2.";  
            }  
            std::cout << msg <<std::endl;
            std::cout << "┌──(user@user)-[" << dir << "]" << std::endl << "└─$";

            break;
        }
        // caesar алогритм
        case '2': {
            std::cout << "1 - зашифровать данные" << std::endl << "2 - расшифрование данные" << std::endl;


            std::cout << "┌──(user@user)-[" << dir << "]" << std::endl;
            std::cout << "└─$";
            
            std::cin >> message;


            if(message == '1') {  
                std::cout << "Введите текст для шифрования: ";  
                std::cin.ignore(); 

                std::getline(std::cin, text);  

                std::string encryptedText = caesar->encode(text);  

                msg = "Зашифрованный текст: " + encryptedText;  


            } else if(message == '2') {  
                std::cout << "Введите текст для расшифрования: ";  
                std::cin.ignore();  
                std::getline(std::cin, text);  
                std::string decryptedText = caesar->decode(text);  
                msg = "Расшифрованный текст: " + decryptedText; 

                std::cout << "┌──(user@user)-[" << dir << "]" << std::endl << "└─$";
 
            } else {  
                msg = "Неверный ввод. Пожалуйста, введите 1 или 2.";  
            }  
            std::cout << msg <<std::endl;
            std::cout << "┌──(user@user)-[" << dir << "]" << std::endl << "└─$";

            break;
        }   
        // если не совпадает с 1,2,3 то ошибка     
        default: {
            std::cerr << setTextColor("[status: ERROR] message:flag is out of range", "31");
            flag = !flag;
        }
    }
    
    if(flag == 0){
        std::cout << "Выход из программы." << std::endl;
        exit(0);
    }
    
    this->analis(js);
}

std::string act_user::get_info(){
    std::string info;
    std::cin >> info;
    return info;
}

void act_user::crypt(){
    json js = json();
    js.Parse("list_algorims.json");
    js.cout_json();
}