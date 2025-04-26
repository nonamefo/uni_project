#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

class json{
    public:
        json();
        json(std::string f_name);
        void Parse(std::string f_name);
        std::vector<char> get_keys();
        void cout_json();
        bool f_status;
        std::string operator[](char index);
        
    private:
        std::vector<char> array_keys;
        std::map<char, std::string> id_crypto_dict;
};

json::json(){}
json::json(std::string f_name){
    // открытие файла
    std::ifstream in(f_name);
    // проверка открытия
    if(in.is_open()){
        // отладочкая информация
        this->f_status = 1;
        // считывание информации из файла и добавление всего в переменную
        std::string text_from_file, line;
        while (in >> line)
        {
            text_from_file +=line;
        }
                
        // удаление лишней информации
        text_from_file.erase(text_from_file.find('{'), 1);
        text_from_file.erase(text_from_file.find('}'), 1);

        std::regex re("\"");
        text_from_file = std::regex_replace(text_from_file, re, "");

        std::regex rep(",");
        text_from_file = std::regex_replace(text_from_file, rep, " ");

        char text_splited[text_from_file.length()];

        strcpy(text_splited, text_from_file.c_str());

        // создание словоря со всеми id и названием
        // переменные которые используються для создания словаря
        char key;
        std::string value;
        bool flag = 1;

        for(int i = 0; i < text_from_file.length(); i++){
            // проверка на пробел
            if (text_splited[i] != ' '){
                // проверка на изменение значения с ключа на значение 
                if (text_splited[i] == ':'){
                    flag = !flag;
                    continue;
                }
                if (flag){
                    key+=text_splited[i];
                }   else    {
                    value+=text_splited[i];
                }
            } else {
                // добавление данных в словарь
                array_keys.push_back(key);
                this->id_crypto_dict[key] = value;
                // отладочная информация
                // std::cout << id_crypto_dict[key]<<std::endl;


                // обнуление пере новым цыклом
                key = '0';
                value = "";
                flag = !flag;
            }
        }
        // завершение цыкла и добавление финальной информации
        // добавление последних элементов
        array_keys.push_back(key);
        this->id_crypto_dict[key] = value;

        key = '0';
        value = "";
    }   else    {
        this->f_status = 0;
        std::cout << "Error" << std::endl; 
    }
    in.close();
}

void json::Parse(std::string f_name){
    // открытие файла
    std::ifstream in(f_name);
    // проверка открытия
    if(in.is_open()){
        // отладочкая информация
        this->f_status = 1;
        // считывание информации из файла и добавление всего в переменную
        std::string text_from_file, line;
        while (in >> line)
        {
            text_from_file +=line;
        }
                
        // удаление лишней информации
        text_from_file.erase(text_from_file.find('{'), 1);
        text_from_file.erase(text_from_file.find('}'), 1);

        std::regex re("\"");
        text_from_file = std::regex_replace(text_from_file, re, "");

        std::regex rep(",");
        text_from_file = std::regex_replace(text_from_file, rep, " ");

        char text_splited[text_from_file.length()];

        strcpy(text_splited, text_from_file.c_str());

        // создание словоря со всеми id и названием
        // переменные которые используються для создания словаря
        char key;
        std::string value;
        bool flag = 1;

        for(int i = 0; i < text_from_file.length(); i++){
            // проверка на пробел
            if (text_splited[i] != ' '){
                // проверка на изменение значения с ключа на значение 
                if (text_splited[i] == ':'){
                    flag = !flag;
                    continue;
                }
                if (flag){
                    key+=text_splited[i];
                }   else    {
                    value+=text_splited[i];
                }
            } else {
                // добавление данных в словарь
                array_keys.push_back(key);
                this->id_crypto_dict[key] = value;
                // отладочная информация
                // std::cout << id_crypto_dict[key]<<std::endl;


                // обнуление пере новым цыклом
                key = '0';
                value = "";
                flag = !flag;
            }
        }
        // завершение цыкла и добавление финальной информации
        // добавление последних элементов
        array_keys.push_back(key);
        this->id_crypto_dict[key] = value;

        key = '0';
        value = "";
    }   else    {
        this->f_status = 0;
        std::cout << "Error" << std::endl; 
    }
    in.close();
}

std::vector<char> json::get_keys(){
    return this->array_keys;
}

std::string json::operator[](char index){
    return id_crypto_dict[index];
}

void json::cout_json(){
    for(char key : this->array_keys){
        std::cout << key << this->id_crypto_dict[key] << std::endl;
    }
}