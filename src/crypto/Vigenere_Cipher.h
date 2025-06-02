// Файл: Vigenere_Cipher.h
#ifndef VIGENERE_CIPHER_H
#define VIGENERE_CIPHER_H

#include <string>
#include "../project_libs.h"
#include "basic_cripto.h"
#include "key_storage.h"

class vigenere_cipher : public basic_cripto {
private:
    std::string keyword;
    
    
    bool isInAlphabet(char c) const;
    int getIndex(char c) const;
    std::string generateRandomKeyword(int length);

public:
    explicit vigenere_cipher(const std::string& key = "");
    std::string get_key() const override;
    bool set_key(const std::string& key) override;
    std::string encode(const std::string message) override;
    std::string decode(const std::string message) override;
    std::pair<std::string, std::string> encode_with_key(const std::string message) override;
    std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key) override;
};

#endif