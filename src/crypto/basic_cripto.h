<<<<<<< HEAD
#ifndef BASIC_CRIPTO_H // Уникальное имя
=======
#ifndef BASIC_CRIPTO_H 
>>>>>>> e554c1fb06d8f389e8c170c7c8cbb30fa77544e0
#define BASIC_CRIPTO_H

class basic_cripto{
    public:
    virtual std::string decode(const std::string message) = 0;
    virtual std::string encode(const std::string message) = 0;

    virtual std::pair<std::string, std::string> encode_with_key(const std::string message) = 0;
    virtual std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key) = 0;
};


#endif // BASIC_CRIPTO_H
