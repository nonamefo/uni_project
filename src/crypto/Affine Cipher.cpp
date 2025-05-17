#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"

class affine_cipher : public basic_cripto {
private:
    int a; // Коэффициент
    int b; // Смещение
    int modSize; // Размер алфавита для модуля
    std::string alphabet; // Используемый алфавит

    // Функция для нахождения модульного обратного числа
    int modInverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1) {
                return x;
            }
        }
        return 1; // Если обратного не существует
    }

    // Получение индекса символа в алфавите
    int getIndex(char c) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()) {
            return std::distance(alphabet.begin(), it);
        }
        return -1;
    }

public:
    affine_cipher(int a_value = -1, int b_value = -1) {
        // Алфавит включает английские и русские буквы, а также основные символы
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                   "0123456789 ,.;:!?-_\"'()[]{}";
        
        modSize = alphabet.size();

        // Генерируем случайный ключ, если не предоставлен
        if (a_value == -1 || b_value == -1) {
            std::random_device rd;
            std::mt19937 gen(rd());
            
            // a должно быть взаимно простым с размером алфавита
            std::vector<int> validA;
            for (int i = 2; i < modSize; i++) {
                if (std::gcd(i, modSize) == 1) {
                    validA.push_back(i);
                }
            }
            
            std::uniform_int_distribution<> a_dis(0, validA.size() - 1);
            std::uniform_int_distribution<> b_dis(1, modSize - 1);
            
            a = validA[a_dis(gen)];
            b = b_dis(gen);
        } else {
            // Проверяем, что a взаимно просто с modSize
            if (std::gcd(a_value, modSize) != 1) {
                // Если нет, используем значение по умолчанию
                a = 5;
            } else {
                a = a_value;
            }
            b = b_value % modSize;
        }
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        std::string key = "a=" + std::to_string(a) + ",b=" + std::to_string(b);
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Шифруем символ по формуле аффинного шифра: E(x) = (ax + b) mod m
                int newIdx = (a * idx + b) % modSize;
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
            size_t aPos = keyPart.find("a=");
            size_t bPos = keyPart.find(",b=");
            
            if (aPos != std::string::npos && bPos != std::string::npos) {
                a = std::stoi(keyPart.substr(aPos + 2, bPos - aPos - 2));
                b = std::stoi(keyPart.substr(bPos + 3));
            }
        }
        
        std::string decrypted;
        std::string key = "a=" + std::to_string(a) + ",b=" + std::to_string(b);
        int aInv = modInverse(a, modSize);
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Дешифруем символ по формуле: D(y) = a^(-1) * (y - b) mod m
                int newIdx = (aInv * ((idx - b) % modSize + modSize)) % modSize;
                decrypted += alphabet[newIdx];
            } else {
                // Символы, которых нет в алфавите, оставляем без изменений
                decrypted += c;
            }
        }
        
        return decrypted + "\nKEY:" + key;
    }
};

#endif // AFFINE_CIPHER_H





//#include "basic_cripto.h"
//class affine_cipher : public basic_cripto {
  //  private:
  //      int a, b;
  //      int modInverse(int a, int n) {
 //           for (int x = 1; x < n; ++x) {
   //             if ((a * x) % n == 1)
     //               return x;
       //     }
         //   return 1;
       // }
    
   // public:
     //   affine_cipher(int aKey = 5, int bKey = 7)
       //     : a(aKey), b(bKey) {
         //   if (std::gcd(a, 26) != 1) {
           //     throw std::runtime_error("Папвметр 'a' должен быть 26.");
           // }
    //    }
    
      //  std::string encode(const std::string message) override {
     //       std::string encrypted = message;
       //     for (char& c : encrypted) {
         //       if (isalpha(c)) {
           //         char offset = isupper(c) ? 'A' : 'a';
             //       c = (a * (c - offset) + b) % 26 + offset;
               // }
        //    }
          //  return encrypted;
       // }
    
        // std::string decode(const std::string encryptedMessage) override {
        //    std::string decrypted = encryptedMessage;
          //  int aInv = modInverse(a, 26);
         //   for (char& c : decrypted) {
           //     if (isalpha(c)) {
             //       char offset = isupper(c) ? 'A' : 'a';
               //     c = (aInv * ((c - offset - b + 26) % 26)) % 26 + offset;
             //   }
          //  }
         //   return decrypted;
       // }
    
      //  int getA() const {
        //    return a;
        // }
    
 //       int getB() const {
   //         return b;
       
     //   }
 //   }; 

// int main(){
   // affine_cipher a = affine_cipher();
  //  std::cout << a.encode("asfg   sfdsd");
// }
