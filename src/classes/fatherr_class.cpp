#include "../project_libs.h"

//класс созднан как образец функций которые должны быть воплащены в программе
class super_encryption{
    virtual void сaesar(char* message);
    virtual void affine(char* message);
    virtual void vigener(char* message);
    virtual void playfair(char* message);
    virtual void polybius(char* message);


};