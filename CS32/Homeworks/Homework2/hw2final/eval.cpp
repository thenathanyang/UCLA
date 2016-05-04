// eval.cpp			Problem 5

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

const int Operand = 0;
const int Operator = 1;
const int openParen = 2;
const int closeParen = 3;
const int Space = 4;

bool isOperator(char op);
int charType(char input);
int precedence(char op);
bool isInfixValid(string infix);
void infixToPostfix(const string& infix, string& postfix);
bool calcPostfix(const string& postfix, const Map& values, int& result);

// Evaluates an integer arithmetic expression
// Precondition: infix is an infix integer arithmetic
//   expression consisting of single lower case letter operands,
//   parentheses, and the operators +, -, *, /, with embedded blanks
//   allowed for readability. 
// Postcondition: If infix is a syntactically valid infix integer
//   expression whose only operands are single lower case letters
//   (whether or not they appear in the values map), then postfix is
//   set to the postfix form of the expression; otherwise postfix may
//   or may not be changed, result is unchanged, and the function
//   returns 1.  If infix is syntactically valid but contains at
//   least one lower case letter operand that does not appear in the
//   values map, then result is unchanged and the function returns 2.
//   If infix is syntactically valid and all its lower case operand
//   letters appear in the values map, then if evaluating the
//   expression (using for each letter in the expression the value in
//   the map that corresponds to it) attempts to divide by zero, then
//   result is unchanged and the function returns 3; otherwise,
//   result is set to the value of the expression and the function
//   returns 0.

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	if (!isInfixValid(infix))
		return 1;

	infixToPostfix(infix, postfix);

	for (int k = 0; k < infix.size(); k++) {
		if (islower(infix[k])) {
			if (! values.contains(infix[k]))
				return 2;
		}
	} 

	if (! calcPostfix(postfix, values, result))
		return 3;

	return 0;
}

bool isInfixValid(string infix)
{
	if (infix.size() == 0)
		return false;

	int paren = 0;

	for (string::iterator x = infix.begin(); x != infix.end(); ++x)
	{
		if (*x != '(' && *x != ')' && *x != ' ')
			continue;
		paren += (*x == '(') ? 1 : ((*x == ')') ? -1 : 0);
		infix.erase(x--);
	}

	int k = 0;
	bool returnValue = true;
	if (infix.size() % 2 == 0)
		return false;
	if (infix.size() % 2 == 1)
	{
		while (k < infix.size() - 1)
		{
			if (! islower(infix[k]) || ! isOperator(infix[k+1]))
				return false;
			k = k+2;
		}
		if (! islower(infix.back()))
			return false;
	}
	return (paren == 0);
}

void infixToPostfix(const string& infix, string& postfix)
{
	postfix = "";
	stack<char> operatorStack;
	
	for (int k = 0; k < infix.size(); k++)
	{
		switch (charType(infix[k]))
		{
			case Operand:
				postfix += infix[k];
				break;
			case openParen:
				operatorStack.push(infix[k]);
				break;
			case closeParen:
				while (charType(operatorStack.top()) != openParen)
				{
					postfix += operatorStack.top();
					operatorStack.pop();
				}
				operatorStack.pop();
				break;
			case Operator:
				while(! operatorStack.empty() && charType(operatorStack.top()) != openParen && precedence(infix[k]) <= precedence(operatorStack.top()))
				{
					postfix += operatorStack.top();
					operatorStack.pop();
				}
				operatorStack.push(infix[k]);
				break;
		}

	}
	while (! operatorStack.empty())
	{
		postfix += operatorStack.top();
		operatorStack.pop();
	}
}

bool isOperator(char op)
{
	if (op == '+' || op == '-' || op == '*' || op == '/')
		return true;
	return false;
}

int charType(char input)
{
	if (isOperator(input))
		return Operator;
	if (input == '(')
		return openParen;
	if (input == ')')
		return closeParen;
	if (input == ' ')
		return Space;
	return Operand;
}

int precedence(char op)
{
	int returnValue = 0;
	if (op == '*' || op == '/')
		returnValue = 1;
	return returnValue;
}

bool calcPostfix(const string& postfix, const Map& values, int& result)
{	
	stack<int> operandStack;
	int valFromMap = 0;
	for (int k = 0; k < postfix.size(); k++)
	{
		if (charType(postfix[k]) == Operand)
		{
			values.get(postfix[k], valFromMap);
			operandStack.push(valFromMap);
		}	
		else	// ch is a binary operator
		{
			int operand2 = operandStack.top();
			operandStack.pop();
			int operand1 = operandStack.top();
			operandStack.pop();

			int tempResult = 0;

			if (postfix[k] == '+')
				tempResult = operand1 + operand2;
			else if (postfix[k] == '-')
				tempResult = operand1 - operand2;
			else if (postfix[k] == '*')
				tempResult = operand1 * operand2;
			else if (postfix[k] == '/')
			{
				if (operand2 == 0)
					return false;
				tempResult = operand1 / operand2;
			}
			operandStack.push(tempResult);
		}
	}
	result = operandStack.top();
	return true;
}

	// int main()
	// {
	//     char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	//     int  vals[] = {  3,  -9,   6,   2,   4,   1  };
	//     Map m;
	//     for (int k = 0; vars[k] != '#'; k++)
	// 	m.insert(vars[k], vals[k]);
	//     string pf;
	//     int answer;
	//     assert(evaluate("a+ e", m, pf, answer) == 0  &&
	//                             pf == "ae+"  &&  answer == -6);
	//     answer = 999;
	//     assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
	//       // unary operators not allowed:
	//     assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
	//     assert(evaluate("a*b", m, pf, answer) == 2  &&
	//                             pf == "ab*"  &&  answer == 999);
	//     assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
	//                             pf == "yoau-*+"  &&  answer == -1);
	//     answer = 999;
	//     assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
 //                                    pf == "oyy-/"  &&  answer == 999);
	//     assert(evaluate(" a  ", m, pf, answer) == 0  &&
	//                             pf == "a"  &&  answer == 3);
	//     assert(evaluate("((a))", m, pf, answer) == 0  &&
	//                             pf == "a"  &&  answer == 3);
	//     cout << "Passed all tests" << endl;
	// }
