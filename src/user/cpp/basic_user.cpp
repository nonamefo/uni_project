#include "../../crypto/Affine_Cipher.h"
#include "../../crypto/Caesar_Cipher.h"
#include "../../crypto/Playfair_Cipher.h"
#include "../../crypto/Polybius_Cipher.h"
#include "../../crypto/Vigenere_Cipher.h"

#include "../basic_user.h"


void basic_user::analis(){
    std::string msg;
    
    while(true){
        msg = "1 - зашифровать или расшифровать текст\n"
              "2 - выход";
        give_choise(msg);
        if (choise == '1'){
            msg = 
                "1 - аффиной\n"
                "2 - цезерь\n"
                "3 - плейфер\n"
                "4 - плобиус\n"
                "5 - вижинер\n";

            give_choise(msg);

            msg = 
                "1 - зашифровать\n"
                "2 - расшифровать";

            switch(choise){
                case '1': {
                    basic_cripto* affine = new affine_cipher();

                    decode_encode(affine, msg);

                    delete affine;
                    break;
                }
                case '2': {
                    basic_cripto* caesar = new caesar_cipher(7);

                    decode_encode(caesar, msg);
                    
                    delete caesar;
                    break;
                }
                case '3': {
                    basic_cripto* playfair = new playfair_cipher("KEYWORD");

                    decode_encode(playfair, msg);

                    delete playfair;
                    break;
                }
                case '4': {
                    basic_cripto* polybius = new polybius_cipher("KEYWORD");

                    decode_encode(polybius, msg);

                    delete polybius;
                    break;
                }
                case '5': {
                    basic_cripto* vigenere = new vigenere_cipher("KEYWORD");

                    decode_encode(vigenere, msg);

                    delete vigenere;
                    break;
                }
                default:
                    choise_out_of_range();
            }
        }
        else if(choise == '2'){
            return;
        }
        else{
            choise_out_of_range();
        }
    }
};


void basic_user::choise_out_of_range(){
    send_message("\nДанного значения нету в выборе\n", 2);
}


void basic_user::decode_encode(basic_cripto* crypto, std::string msg = ""){
    give_choise(msg);
    
    if (choise == '1'){
        msg = "введите текст для зашифрования: ";
        send_message(msg, 1);

        std::cin >> msg;
        std::cout << crypto->encode(msg) << std::endl;
    }
    else if (choise == '2'){
        msg = "введите текст для расшифровки: ";
        send_message(msg, 1);
        
        std::cin >> msg;
        std::cout << crypto->decode(msg) << std::endl;
    }
    else{
        choise_out_of_range();
    }
};


std::string basic_user::get_info(){
    std::string msg;
    std::cin >> msg;
    return msg;
};

std::string basic_user::get_curent_dir() {
    try {
        std::filesystem::path path = std::filesystem::current_path(); // Явное указание
        return path.string();
    } catch (const std::filesystem::filesystem_error& e) { // Явное указание
        std::cerr << "Error getting working directory: " << e.what() << std::endl;
        return "";
    }
}

void basic_user::terminal(){
    std::cout << "[none]>";
}

void basic_user::give_choise(std::string msg ){
        send_message(msg, 2);
        terminal();
        this->choise = get_info()[0];
}

void basic_user::send_message(std::string msg, char choise){
    if(choise == 1){
        std::cout << msg;
    }
    else if (choise == 2){
        std::cout << msg << std::endl;
    }
}