// #include "basic_cripto.h"

class affine_cipher : public basic_cripto {
    private:
        int a, b;
        int modInverse(int a, int n) {
            for (int x = 1; x < n; ++x) {
                if ((a * x) % n == 1)
                    return x;
            }
            return 1;
        }
    
    public:
        affine_cipher(int aKey = 5, int bKey = 7)
            : a(aKey), b(bKey) {
            if (std::gcd(a, 26) != 1) {
                throw std::runtime_error("Папвметр 'a' должен быть 26.");
            }
        }
    
        std::string encode(const std::string& message) override {
            std::string encrypted = message;
            for (char& c : encrypted) {
                if (isalpha(c)) {
                    char offset = isupper(c) ? 'A' : 'a';
                    c = (a * (c - offset) + b) % 26 + offset;
                }
            }
            return encrypted;
        }
    
        std::string decode(const std::string& encryptedMessage) override {
            std::string decrypted = encryptedMessage;
            int aInv = modInverse(a, 26);
            for (char& c : decrypted) {
                if (isalpha(c)) {
                    char offset = isupper(c) ? 'A' : 'a';
                    c = (aInv * ((c - offset - b + 26) % 26)) % 26 + offset;
                }
            }
            return decrypted;
        }
    
        int getA() const {
            return a;
        }
    
        int getB() const {
            return b;
       
        }
    }; 