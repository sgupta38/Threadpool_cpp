#include <iostream>
#include "threadpool.h"
using namespace std;

int main() {
    int rv = 0;
    threadpool tp(2);

    for (int i = 0; i < 20; ++i) {
        tp.add_to_queue([] { cout << "Dispatcher 1" << endl; });
        tp.add_to_queue([] { cout << "Dispatcher 2" << endl; });
        tp.add_to_queue([] { cout << "Dispatcher 3" << endl; });
    }
    return rv;
}