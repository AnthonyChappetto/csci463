#include <iostream>
#include <iomanip>
#include <cstdint> //Standard integar representation

using std::cout;
using std::endl;
using std::setw;

class test {
    public:
        int32_t  c[2];
};

int main() {
    char    c;
    short   s;
    int     i;
    cout << "sizeof(c): " << sizeof(c) << endl;
    cout << "sizeof(s): " << sizeof(s) << endl;
    cout << "sizeof(i): " << sizeof(i) << endl;
}