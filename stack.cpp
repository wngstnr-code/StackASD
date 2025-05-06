#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

struct Node {
    string data;
    Node* left;
    Node* right;
    
    Node(string val) : data(val), left(nullptr), right(nullptr) {}
};

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string infixToPostfix(const string& infix) {
    stack<char> opStack;
    string postfix;
    
    for (size_t i = 0; i < infix.size(); ++i) {
        char c = infix[i];
        
        if (c == ' ') continue;
        
        if (isdigit(c)) {
            while (i < infix.size() && isdigit(infix[i])) {
                postfix += infix[i++];
            }
            postfix += ' ';
            i--; 
        }
        else if (c == '(') {
            opStack.push(c);
        }
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                postfix += ' ';
                opStack.pop();
            }
            opStack.pop(); 
        }
        else if (isOperator(string(1, c))) {
            while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(c)) {
                postfix += opStack.top();
                postfix += ' ';
                opStack.pop();
            }
            opStack.push(c);
        }
    }
    
    while (!opStack.empty()) {
        postfix += opStack.top();
        postfix += ' ';
        opStack.pop();
    }
    
    return postfix;
}

Node* constructExpressionTree(const string& postfix) {
    stack<Node*> nodeStack;
    string token;
    
    for (size_t i = 0; i < postfix.size(); ++i) {
        char c = postfix[i];
        
        if (c == ' ') continue;
        
        token.clear();
        while (i < postfix.size() && postfix[i] != ' ') {
            token += postfix[i++];
        }
        
        if (token.empty()) continue;
        
        if (isOperator(token)) {
            Node* newNode = new Node(token);
            newNode->right = nodeStack.top();
            nodeStack.pop();
            newNode->left = nodeStack.top();
            nodeStack.pop();
            nodeStack.push(newNode);
        } else {
            nodeStack.push(new Node(token));
        }
    }
    
    return nodeStack.top();
}

void postorderTraversal(Node* root) {
    if (root) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        cout << root->data << " ";
    }
}

int evaluateExpressionTree(Node* root) {
    if (!root) return 0;
    
    if (!root->left && !root->right) {
        return stoi(root->data);
    }
    
    int leftVal = evaluateExpressionTree(root->left);
    int rightVal = evaluateExpressionTree(root->right);
    
    if (root->data == "+") return leftVal + rightVal;
    if (root->data == "-") return leftVal - rightVal;
    if (root->data == "*") return leftVal * rightVal;
    if (root->data == "/") return leftVal / rightVal;
    
    throw invalid_argument("Operator tidak valid");
}

int evaluatePostfix(const string& postfix) {
    stack<int> operandStack;
    string token;
    
    for (size_t i = 0; i < postfix.size(); ++i) {
        char c = postfix[i];
        
        if (c == ' ') continue;
        
        token.clear();
        while (i < postfix.size() && postfix[i] != ' ') {
            token += postfix[i++];
        }
        
        if (token.empty()) continue;
        
        if (isOperator(token)) {
            if (operandStack.size() < 2) {
                throw invalid_argument("Ekspresi postfix tidak valid");
            }
            
            int operand2 = operandStack.top(); operandStack.pop();
            int operand1 = operandStack.top(); operandStack.pop();
            
            int result;
            if (token == "+") result = operand1 + operand2;
            else if (token == "-") result = operand1 - operand2;
            else if (token == "*") result = operand1 * operand2;
            else if (token == "/") result = operand1 / operand2;
            else throw invalid_argument("Operator tidak valid");
            
            operandStack.push(result);
        } else {
            operandStack.push(stoi(token));
        }
    }
    
    if (operandStack.size() != 1) {
        throw invalid_argument("Ekspresi postfix tidak valid");
    }
    
    return operandStack.top();
}

int main() {
    string infixExpr = "(5 + 3) * (6 - 2)";
    
    string postfixExpr = infixToPostfix(infixExpr);
    cout << "Ekspresi Infix: " << infixExpr << endl;
    cout << "Ekspresi Postfix: " << postfixExpr << endl;
    
    Node* root = constructExpressionTree(postfixExpr);
    
    cout << "Postorder Traversal: ";
    postorderTraversal(root);
    cout << endl;
    
    int treeResult = evaluateExpressionTree(root);
    cout << "Hasil evaluasi (expression tree): " << treeResult << endl;
    
    int stackResult = evaluatePostfix(postfixExpr);
    cout << "Hasil evaluasi (postfix stack): " << stackResult << endl;
    
    return 0;
}