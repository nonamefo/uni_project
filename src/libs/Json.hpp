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
        std::vector<std::string> get_keys();
        void cout_json();
        bool f_status;
        std::string operator[](std::string index);
        
    private:
        std::vector<std::string> array_keys;
        std::map<std::string, std::string> id_crypto_dict;
};

json::json(){}
json::json(std::string f_name){
    std::ifstream in(f_name);
    if (in.is_open()) {
        this->f_status = 1;
        std::stringstream ss;
        ss << in.rdbuf(); // Считываем весь файл в stringstream
        std::string text_from_file = ss.str();


        try {
            size_t start = text_from_file.find('{') + 1;
            size_t end = text_from_file.rfind('}');
            text_from_file = text_from_file.substr(start, end - start);


            std::string key, value;
            bool in_key = true;
            std::stringstream ss(text_from_file);
            char c;

            while (ss.get(c)) {
                if (c == '"') { continue; } // Пропускаем кавычки
                if (c == ':') { in_key = false; continue; }
                if (c == ',' || c == '}') {
                    array_keys.push_back(key);
                    id_crypto_dict[key] = value;
                    key.clear();
                    value.clear();
                    in_key = true;
                    if (c == '}') break; // Выходим, если конец объекта
                    continue;
                }
                if (in_key) {
                    key += c;
                } else {
                    value += c;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            f_status = 0;
        }


    } else {
        this->f_status = 0;
        std::cerr << "Error: Could not open file " << f_name << std::endl;
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
        std::string key;
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
                key = "";
                value = "";
                flag = !flag;
            }
        }
        // завершение цыкла и добавление финальной информации
        // добавление последних элементов
        array_keys.push_back(key);
        this->id_crypto_dict[key] = value;

        key = "";
        value = "";
    }   else    {
        this->f_status = 0;
        std::cout << "Error" << std::endl; 
    }
    in.close();
}

std::vector<std::string> json::get_keys(){
    return this->array_keys;
}

std::string json::operator[](std::string index){
    return id_crypto_dict[index];
}

void json::cout_json(){
    for (std::string key : this->get_keys()){
        std::cout << this->id_crypto_dict[key];
    }
}