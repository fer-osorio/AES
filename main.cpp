#include<iostream>
#include"KeyExpansion.hpp"

int main(int argc, char *argv[]) {
    byte key[] = {0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B,
    0x80, 0x90, 0x79, 0xE5, 0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};
    byte w[216]; // -Key schedule.
    KeyExpansion(key, 6, w);
    for(int i = 0; i < 216; i++) {
        printf("%X, ", w[i]);
        if((i + 1 & 3) == 0)
            std::cout << '\n';
    }
    std::cout << '\n';
    return 0;
}
