#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H

#include <string>
#include "../project_libs.h"
#include "basic_cripto.h"
#include "key_storage.h"

class affine_cipher : public basic_cripto {
private:
    int a;
    int b;
    int modSize;
    std::string alphabet;
    int modInverse(int a, int m);
    int getIndex(char c);

public:
    affine_cipher(int a_value = -1, int b_value = -1);
    std::string get_key() const override;
    bool set_key(const std::string& key) override;
    std::string encode(const std::string message) override;
    std::string decode(const std::string message) override;
    std::pair<std::string, std::string> encode_with_key(const std::string message) override;
    std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key) override;
};

#endif