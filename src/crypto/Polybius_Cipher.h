#ifndef POLYBIUS_CIPHER_H
#define POLYBIUS_CIPHER_H

#include <vector>
#include <string>
#include "../project_libs.h"
#include "basic_cripto.h"
#include "key_storage.h"

class polybius_cipher : public basic_cripto {
private:
    std::string full_alphabet;
    std::vector<std::vector<char>> square;
    std::string keyword;
    int square_size;
    
    void generateSquare();

public:
    polybius_cipher(const std::string& key = "");
    std::string get_key() const override;
    bool set_key(const std::string& key) override;
    std::string encode(const std::string message) override;
    std::string decode(const std::string message) override;
    std::pair<std::string, std::string> encode_with_key(const std::string message) override;
    std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key_str) override;
};

#endif