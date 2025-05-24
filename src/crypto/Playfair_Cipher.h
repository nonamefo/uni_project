#ifndef PLAYFAIR_CIPHER_H
#define PLAYFAIR_CIPHER_H

#include <vector>
#include <string>
#include "../project_libs.h"
#include "basic_cripto.h"
#include "key_storage.h"

class playfair_cipher : public basic_cripto {
private:
    std::string key;
    std::vector<std::vector<char>> matrix;
    std::string full_alphabet;
    int matrix_size;
    
    void generateMatrix();
    std::pair<int, int> findPosition(char c);

public:
    playfair_cipher(std::string key_value = "");
    std::string get_key() const override;
    bool set_key(const std::string& key) override;
    std::string encode(const std::string message) override;
    std::string decode(const std::string message) override;
    std::pair<std::string, std::string> encode_with_key(const std::string message) override;
    std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key_str) override;
};

#endif