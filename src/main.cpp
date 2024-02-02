
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <variant>

#include <defines.h>
#include "Token.h"

/*
class IToken
{
    public:
      
      virtual void foo() = 0;
};


template<typename T> class Token : public IToken
{
public:
    
    virtual void foo() { std::cout << "A" << std::endl; }
    
    Token(T data) : data(data) {}
    std::string type;
    T data;
};
*/
/*
class Tokena
{
public:
   
    TokenType type;
    std::variant<std::string, int, float, bool> data;
};
*/


/*template<typename T> 
T getData(std::unique_ptr<Token<T>> token) 
{
    return token.data;
};
*/  


int main() 
{
   std::cout << "Hello world" << std::endl;
        

    std::unique_ptr<Token> t_ = std::make_unique<Token>();
    t_->type = TokenType::DATA_BOOL;
    t_->data = false;

    std::unique_ptr<Token> t_2 = std::make_unique<Token>();
    t_2->type = TokenType::DATA_INT;
    t_2->data = 12;

    std::unique_ptr<Token> t_3 = std::make_unique<Token>();
    t_3->type = TokenType::DATA_STR;
    t_3->data = std::string{"sss"};
    
    std::vector<std::unique_ptr<Token>> toks;
    toks.emplace_back(std::move(t_));
    toks.emplace_back(std::move(t_2));
    toks.emplace_back(std::move(t_3));


    std::cout << std::get<bool>(toks[0]->data) << std::endl;
    std::cout << std::get<int>(toks[1]->data) << std::endl;
    std::cout << std::get<std::string>(toks[2]->data) << std::endl;

}
