/**
 * @file pa18.cpp
 * @author Daniel O'Neill
 * @date 11/13/2024
 * @section 1003
 *
 * @note I pledge my word of honor that I have abided by the
 * CSN Academic Integrity Policy while completing this assignment.
 * @class Vector
 * @brief this program takes in information and puts it in a prefix expression
 * and does the operations for the math.
 */

/// Include statmenst
#include <cmath>
#include <cctype>

#include "LList.hpp"

/**
* @brief this is a derived class of the LList class, LList is protected and the
* Stack class can put in and access the information at the top of the list.
* There is no traversing the list.
*/
template <class T>
class Stack : protected LList<T> {
public:
    /**
    * @brief this function returns the value at the top of the stack and checks
    * to make sure it is not pulling from an empty list
    */
    T& top() {
        if (empty())
        {
            throw std::underflow_error("Empty stack");
        }
        return this->front(); }
    /**
    * @brief this function checks to see if the list is empty
    */
    bool empty() const {return LList<T>::empty(); }
    /**
    * @brief this function inputs information before the current position in the
    * llist
    * @param value is the input that will go on top of the stack
    */
    void push(const T& value) { LList<T>::push_front(value); }
    /**
    * @brief this function removes the top of the llist
    */
    void pop() {
        if (empty())
        {
            throw std::underflow_error("Empty stack");
        } else {
        this->pop_front();
        }
    }
};

std::string input();
void format();
void post(std::string& equation);
int precidence(char value);
void output(Stack<std::string>& exp);
void order(Stack<std::string>& exp);



int main(){
    Stack<std::string> equations {};
    size_t num_of_equations {};
    std::string answer;

    std::cin >> num_of_equations;
    std::cin.ignore(2);
    for (size_t i = 0; i < num_of_equations; ++i)
    {
        answer = input();
        post(answer);
        equations.push(answer);
    }
    order(equations);
    for (size_t i = 0; i < num_of_equations; ++i)
    {
        output(equations);
        if (i < num_of_equations - 1){
            std::cout << std::endl;
        }
    }


    return EXIT_SUCCESS;
}

/**
* @brief this function inputs from the user and puts it into a string returning
* value
*/
std::string input(){
    std::string value = "";
    std::string add;
    while (getline(std::cin, add) && add != ""){
        value += add;
    }

    return value;
}

/**
* @brief this function outputs one answer at a time and removes the line after
* it is done
* @param exp is a copy of the stack to put out to the terminal
*/
void output(Stack<std::string>& exp){
        std::cout << exp.top() << std::endl;
        exp.pop();
}

/**
* @brief this function takes the string passed to it and organizes it into the
* correct format
* @param equation is passed by reference in order to change it
*/
void post(std::string& equation){
    Stack<char> exp {};
    std::string fix;
    for (size_t i = 0; i < equation.size(); ++i)
    {
        if (isdigit(equation[i]))
        {
            fix += equation[i];
        }
        else if (equation[i] == '(')
        {
            exp.push(equation[i]);
        }
        else if (equation[i] == ')')
        {
            while (!exp.empty() && exp.top() != '(')
            {
                fix += exp.top();
                exp.pop();
            }
            exp.pop();
        } else {
            while (!exp.empty() && precidence(equation[i]) <=
                                              precidence(exp.top()))
            {
                fix += exp.top();
                exp.pop();
            }
            exp.push(equation[i]);
        }
    }
    while (!exp.empty())
    {
        fix += exp.top();
        exp.pop();
    }
    equation = fix;
}

/**
* @brief this function sorts the presidence of the symbols acording to pemdas
* @param value is the symbol being passed in
*/
int precidence(char value)
{
    int mult;
    if (value == '*' || value == '/')
    {
        mult = 2;
    }
    else if (value == '+' || value == '-'){
        mult = 1;
    } else {
        mult = 0;
    }
    return mult;
}

/**
* @brief this function flips the current order of the Stack.
* @param exp is a copy of the stack to put out to the terminal
*/
void order(Stack<std::string>& exp){
    Stack<std::string> reverse = exp;
    while (!reverse.empty())
    {
        exp.push(reverse.top());
        reverse.pop();
    }
}
