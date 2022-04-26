#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long sixtyFourBitKey = 69696969LL;
long long plaintext = 999999999999999LL;
long ciphertext;

unsigned long long DESRound();
int manglerFunction(int);
long long expansionFunction(int);
int getLH();
int getRH();
long long setBit64(long long, int, int);
int setBit32(int, int, int);
int tellBit32(int, int);
int tellBit64(long long, int);
void _debugPrint32bit(int);
void _debugPrint64bit(long long);

////////////////////////////////////////////////////////////////
// Debug Functions

void _debugPrint32bit(int input) {
    int bit;
    printf("%d:\t", input);
    for (int i = 31; i >= 0; i--) {
        bit = tellBit32(input, i);
        printf("%d", bit);
    }
    printf("\n");
}

void _debugPrint64bit(long long input) {
    int bit;
    printf("%lld:\t", input);
    for (int i = 63; i >= 0; i--) {
        bit = tellBit64(input, i);
        printf("%d", bit);
    }
    printf("\n");
}
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Bit Manipulation Functions

long long setBit64(long long input, int index, int value) {
    long long newInt = 0LL;
    if (value == 0) {
        long long mask = -1LL - (1LL << (index));
        newInt = input & mask;
    } else {
        long long mask = (1LL << (index));
        newInt = input | mask;
    }
    return newInt;
}

int setBit32(int input, int index, int value) {
    int newInt = 0;
    if (value == 0) {
        int mask = (-1) - (1 << (index));
        newInt = input & mask;
    } else {
        int mask = (1 << (index));
        newInt = input | mask;
    }
    return newInt;
}

int tellBit32(int input, int index) {
    int mask = 1 << index;
    int test = mask & input;
    if (test != 0) {
        return 1;
    } else {
        return 0;
    }
}

int tellBit64(long long input, int index) {
    long long mask = 1LL << index;
    long long test = mask & input;
    if (test != 0) {
        return 1;
    } else {
        return 0;
    }
}
////////////////////////////////////////////////////////////////

int manglerFunction(int RH) {
    long long expandedRH = expansionFunction(RH);
    return expandedRH;
}

long long expansionFunction(int RH) {
    long long retValue = 0;
    int index32 = 0;
    int bit = tellBit32(RH, 31);
    retValue = setBit64(retValue, 0, bit);
    index32++;
    for (int i = 0; i < 4; i++) {
        bit = tellBit32(RH, i);
        retValue = setBit64(retValue, (i+1), bit);
        index32++;
    }

    bit = tellBit32(RH, 3);
    retValue = setBit64(retValue, 6, bit);
    index32++;
    int index64 = 5;

    for (int j = 0; j < 6; j++) {
        bit = tellBit32(RH, index32);
        retValue = setBit64(retValue, index64, bit);
        index64 += 2;
        for (int i = 0; i < 4; i++) {
            bit = tellBit32(RH, index32);
            retValue = setBit64(retValue, index64, bit);
            index32++;
            index64++;
        }
        bit = tellBit32(RH, (index32-1));
        retValue = setBit64(retValue, (index64+1), bit);
    }

    bit = tellBit32(RH, index32);
    retValue = setBit64(retValue, index64, bit);
    index64 += 2;

    for (int i = 0; i < 4; i++) {
        bit = tellBit32(RH, index32);
        retValue = setBit64(retValue, index64, bit);
        index32++;
        index64++;
    }
    index64++;
    bit = tellBit32(RH, 0);
    retValue = setBit64(retValue, index64, bit);
    return retValue;
}

long long XOR48(long long RH, long long key) {
    
}

int getLH() {
    int leftHalf = 0;
    int value;
    for (int i = 32; i < 64; i++) {
        value = tellBit64(plaintext, i);
        leftHalf = setBit32(leftHalf, (i-32), value);
    }
    return leftHalf;
}

int getRH() {
    int rightHalf = 0;
    int value;
    for (int i = 0; i < 32; i++) {
        value = tellBit64(plaintext, i);
        rightHalf = setBit32(rightHalf, i, value);
    }
    return rightHalf;
}

unsigned long long DESRound() {
    long long ciphertext = 0LL;
    int LH = getLH();
    _debugPrint32bit(LH);
    int RH = getRH();
    _debugPrint32bit(RH);
    int newRH = manglerFunction(RH);
    return ciphertext;
}

int main() {
    _debugPrint64bit(plaintext);
    DESRound();
}