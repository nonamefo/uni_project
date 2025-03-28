class caesar_cipher : public basic_cripto {
    private:
        int shift; // Параметр сдвига
    public:
        caesar_cipher(int s = std::rand() % 26 + 1) : shift(s) {} // генерацией случайного сдвига
    
        std::string encode(const std::string& message) override {
            std::string encrypted = message;
            for (char& c : encrypted) {
                if (isalpha(c)) {
                    char offset = isupper(c) ? 'A' : 'a';
                    c = (c - offset + shift) % 26 + offset;
                }
            }
            return encrypted;
        }
    
        std::string decode(const std::string& encryptedMessage) override {
            std::string decrypted = encryptedMessage;
            for (char& c : decrypted) {
                if (isalpha(c)) {
                    char offset = isupper(c) ? 'A' : 'a';
                    c = (c - offset - shift + 26) % 26 + offset;
                }
            }
            return decrypted;
        }
    
        int getShift() const {
            return shift;
        }
    };