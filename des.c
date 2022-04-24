#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long sixtyFourBitKey = 69696969LL;
long long plaintext = 420420420LL;
long ciphertext;

void _debugPrint32bit(int input) {
    int buf[32];
    int bit = 0;
    for (int i = 0; i < 32; i++) {
        bit = tellBit32(input, i);
        buf[i] = bit;
    }
}

void _debugPrint64bit(long long input) {
    int buf[64];
    int bit = 0;
    for (int i = 0; i < 64; i++) {
        bit = tellBit64(input, i);
        buf[i] = bit;
    }
}

int manglerFunction(int RH) {
    
}

int getLH() {
    int leftHalf = 0;
    int value;
    for (int i = 0; i < 32; i++) {
        value = tellBit64(sixtyFourBitKey, i*2);
        setBit32(leftHalf, i*2, value);
    }
}

int getRH() {
    int rightHalf = 0;
    int value;
    for (int i = 1; i < 32; i++) {
        value = tellBit64(sixtyFourBitKey, i*2);
        setBit32(rightHalf, i*2, value);
    }
}

unsigned long long DESRound() {
    int LH = getLH();
    int RH = getRH();
    int newRH = manglerFunction(RH);
}


long long setBit64(long long input, int index, int value) {
    long long newInt = 0;
    if (value == 0) {
        int mask = -1 - (1 << (index));
        newInt = input & mask;
    } else {
        int mask = (9223372036854775807LL >> (index));
        newInt = input | mask;
    }
    return newInt;
}

int setBit32(int input, int index, int value) {
    int newInt = 0;
    if (value == 0) {
        int mask = -1 - (1 << (index));
        newInt = input & mask;
    } else {
        int mask = (2147483648 >> (index));
        newInt = input | mask;
    }
    return newInt;
}

int tellBit32(int input, int index) {
    int mask = 1 << index;
    int test = mask & input;
    return test;
}

int tellBit64(long long input, int index) {
    long long mask = 1 << index;
    int test = mask & input;
    return test;
}

int main() {
    DESRound();
}