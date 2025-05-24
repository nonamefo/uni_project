#include "basic_cripto.h"
#include <algorithm>
#include <stdexcept>
#include <string>

class playfair : public basic_cripto {
private:
    std::string key;
    std::string matrica[5][5];

    void findPosition(char c, int& row, int& col) const {
        c = toupper(c);
        if (c == 'J') c = 'I';
        
        for (row = 0; row < 5; ++row) {
            for (col = 0; col < 5; ++col) {
                if (matrica[row][col][0] == c) {
                    return;
                }
            }
        }
        throw std::runtime_error("Character not found in matrix");
    }

public:
    playfair(std::string key = "default") : key(key) {
        std::string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
        std::string usedChars;
        int index = 0;

        // Обработка ключа
        for (char c : key) {
            if (isalpha(c)) {
                char upper_c = toupper(c);
                if (upper_c == 'J') upper_c = 'I';
                
                if (usedChars.find(upper_c) == std::string::npos) {
                    matrica[index / 5][index % 5] = upper_c;
                    usedChars += upper_c;
                    index++;
                }
            }
        }

        // Заполнение матрицы
        for (char c : alphabet) {
            if (usedChars.find(c) == std::string::npos) {
                matrica[index / 5][index % 5] = c;
                usedChars += c;
                index++;
            }
        }
    }

    std::string encode(const std::string message) override {
        std::string processed;
        // Предварительная обработка
        for (char c : message) {
            if (isalpha(c)) {
                char upper_c = toupper(c);
                if (upper_c == 'J') upper_c = 'I';
                processed += upper_c;
            }
        }

        // Формирование биграмм
        std::string prepared;
        size_t i = 0;
        while (i < processed.size()) {
            if (i+1 >= processed.size()) {
                prepared += processed[i];
                prepared += 'X';
                break;
            }
            else if (processed[i] == processed[i+1]) {
                prepared += processed[i];
                prepared += 'X';
                i++;
            }
            else {
                prepared += processed[i];
                prepared += processed[i+1];
                i += 2;
            }
        }

        // Шифрование
        std::string ciphertext;
        for (i = 0; i < prepared.size(); i += 2) {
            char a = prepared[i];
            char b = prepared[i+1];
            
            int row_a, col_a, row_b, col_b;
            findPosition(a, row_a, col_a);
            findPosition(b, row_b, col_b);

            if (row_a == row_b) {
                col_a = (col_a + 1) % 5;
                col_b = (col_b + 1) % 5;
            }
            else if (col_a == col_b) {
                row_a = (row_a + 1) % 5;
                row_b = (row_b + 1) % 5;
            }
            else {
                std::swap(col_a, col_b);
            }
            
            ciphertext += matrica[row_a][col_a];
            ciphertext += matrica[row_b][col_b];
        }
        return ciphertext;
    }

    std::string decode(const std::string message) override {
        std::string processed;
        for (char c : message) {
            if (isalpha(c)) {
                processed += toupper(c);
            }
        }

        if (processed.size() % 2 != 0) {
            processed += 'X';
        }

        std::string plaintext;
        for (size_t i = 0; i < processed.size(); i += 2) {
            char a = processed[i];
            char b = processed[i+1];
            
            int row_a, col_a, row_b, col_b;
            findPosition(a, row_a, col_a);
            findPosition(b, row_b, col_b);

            if (row_a == row_b) {
                col_a = (col_a - 1 + 5) % 5;
                col_b = (col_b - 1 + 5) % 5;
            }
            else if (col_a == col_b) {
                row_a = (row_a - 1 + 5) % 5;
                row_b = (row_b - 1 + 5) % 5;
            }
            else {
                std::swap(col_a, col_b);
            }
            
            plaintext += matrica[row_a][col_a];
            plaintext += matrica[row_b][col_b];
        }

        // Удаление технических X
        std::string result;
        for (size_t i = 0; i < plaintext.size(); ++i) {
            if (i > 0 && i < plaintext.size()-1 && 
                plaintext[i] == 'X' && 
                plaintext[i-1] == plaintext[i+1]) {
                continue;
            }
            if (i == plaintext.size()-1 && plaintext[i] == 'X') {
                continue;
            }
            result += plaintext[i];
        }
        
        return result;
    }

    std::string getKey() const {
        return key;
    }

    std::string getMatrica() const {
        std::string result;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                result += matrica[i][j];
            }
        }
        return result;
    }
};