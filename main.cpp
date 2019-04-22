#include <iostream>
#include "utils.hpp"

using namespace std;

/*
int f(int x){
    int res = 1;
    int n = 2;
    for (int i = 0; i < n; ++i){
        res *= x;
    }
    return res;
}
*/

int main() {
    printRules();
    predProccess();
    string request;
    cout << ">> ";
    while (getline(cin, request)) {
        if (request == "calculate") {
            int x;
            cout << "Введите число: ";
            cin >> x;
            cout << execute(x) << '\n';
            continue;
        }
        if (request == "current_pow") {
            getCurrentPow();
            continue;
        }
        if (request == "modify") {
            int n;
            cout << "Введите степень: ";
            cin >> n;
            modify(n);
            getCurrentPow();
            continue;
        }
        if (request == "help") {
            printRules();
            cout << ">> ";
            continue;
        }
        if (request == "exit") {
            freeUpMemory();
            break;
        }
        cout << ">> ";
    }
    freeUpMemory();
    return 0;
}