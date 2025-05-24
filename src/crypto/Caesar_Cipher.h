#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <string>
#include "../project_libs.h"
#include "basic_cripto.h"
#include "key_storage.h"

class caesar_cipher : public basic_cripto {
private:
    int shift;
    std::string alphabet;
    int getIndex(char c);

public:
    caesar_cipher(int shift_value = -1);
    std::string get_key() const override;
    bool set_key(const std::string& key) override;
    std::string encode(const std::string message) override;
    std::string decode(const std::string message) override;
    std::pair<std::string, std::string> encode_with_key(const std::string message) override;
    std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key) override;
};

#endif