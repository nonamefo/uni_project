#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"

class caesar_cipher : public basic_cripto {
private:
    int shift; // Сдвиг
    std::string alphabet; // Используемый алфавит

    // Получение индекса символа в алфавите
    int getIndex(char c) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()) {
            return std::distance(alphabet.begin(), it);
        }
        return -1;
    }

public:
    caesar_cipher(int shift_value = -1) {
        // Алфавит включает английские и русские буквы, а также основные символы
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                   "0123456789 ,.;:!?-_\"'()[]{}";
        
        // Генерируем случайный сдвиг, если не предоставлен
        if (shift_value == -1) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, alphabet.size() - 1);
            shift = dis(gen);
        } else {
            shift = shift_value % alphabet.size();
            if (shift < 0) shift += alphabet.size(); // Обеспечиваем положительный сдвиг
        }
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        std::string key = "shift=" + std::to_string(shift);
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Шифруем символ сдвигом
                int newIdx = (idx + shift) % alphabet.size();
                encrypted += alphabet[newIdx];
            } else {
                // Символы, которых нет в алфавите, оставляем без изменений
                encrypted += c;
            }
        }
        
        return encrypted + "\nKEY:" + key;
    }

    std::string decode(const std::string encryptedMessage) override {
        // Разделяем сообщение и ключ
        size_t keyPos = encryptedMessage.find("\nKEY:");
        std::string message = encryptedMessage;
        
        if (keyPos != std::string::npos) {
            message = encryptedMessage.substr(0, keyPos);
            std::string keyPart = encryptedMessage.substr(keyPos + 5);
            
            // Парсим ключ
            size_t shiftPos = keyPart.find("shift=");
            if (shiftPos != std::string::npos) {
                shift = std::stoi(keyPart.substr(shiftPos + 6));
            }
        }
        
        std::string decrypted;
        std::string key = "shift=" + std::to_string(shift);
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Дешифруем символ обратным сдвигом
                int newIdx = (idx - shift + alphabet.size()) % alphabet.size();
                decrypted += alphabet[newIdx];
            } else {
                // Символы, которых нет в алфавите, оставляем без изменений
                decrypted += c;
            }
        }
        
        return decrypted + "\nKEY:" + key;
    }
};

#endif // CAESAR_CIPHER_H




// #include "basic_cripto.h"


// class caesar_cipher : public basic_cripto {
  //  private:
     //   int shift; // Параметр сдвига
//    public:
 //       caesar_cipher(int s = std::rand() % 26 + 1) : shift(s) {} // генерацией случайного сдвига
    
//        std::string encode(const std::string message) override {
//            std::string encrypted = message;
// for (char& c : encrypted) {
 //               if (isalpha(c)) {
//                    char offset = isupper(c) ? 'A' : 'a';
//                   c = (c - offset + shift) % 26 + offset;
//                }
//            }
//            return encrypted;
//        }
    
//        std::string decode(const std::string encryptedMessage) override {
 //           std::string decrypted = encryptedMessage;
// for (char& c : decrypted) {
//                if (isalpha(c)) {
 //                   char offset = isupper(c) ? 'A' : 'a';
 //                   c = (c - offset - shift + 26) % 26 + offset;
//                }
////            }
//            return decrypted;
//        }
//    
//        int getShift() const {
//            return shift;
//        }
//    };
