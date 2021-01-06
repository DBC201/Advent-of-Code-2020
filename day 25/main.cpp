#include <iostream>

const int given_constant = 20201227;

int find_loop_size(int public_key) {
    int subject = 7;
    int value = 1;
    int loop_size = 0;
    do {
        value *= subject;
        value %= given_constant;
        loop_size++;
    } while (value != public_key);
    return loop_size;
}

unsigned long long find_encryption_key(int public_key, int loop_size) {
    unsigned long long value = 1;
    for (int i=0; i<loop_size; i++) {
        value *= public_key;
        value %= given_constant;
    }
    return value;
}

int main() {
    int key1 = 6930903;
    int key2 = 19716708;
    std::cout << find_encryption_key(key1, find_loop_size(key2)) << std::endl;
    return 0;
}
