#ifndef REGISTERFACTORY_H
#define REGISTERFACTORY_H

class Register;

/**
 * @brief 使用简单工厂模式创建不同平台下的Register.
 */
class RegisterFactory {
public:
    static Register& getRegister();
};

#endif // REGISTERFACTORY_H
 
