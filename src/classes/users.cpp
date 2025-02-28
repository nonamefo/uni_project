#include "./father_cryption_class.cpp"
// #include "../libs/Json.hpp"

class act_user : public super_act_user {
    public:
      act_user(std::string login, std::string username, std::string password) : super_act_user(login, username, password) {}
};