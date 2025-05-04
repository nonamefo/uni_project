#ifndef BASIC_CRIPTO_H // Уникальное имя, обычно имя файла заглавными буквами с подчеркиваниями
#define BASIC_CRIPTO_H

class basic_cripto{
    public:
    virtual std::string decode(const std::string message) = 0;
    virtual std::string encode(const std::string message) = 0;
};


#endif // BASIC_CRIPTO_H