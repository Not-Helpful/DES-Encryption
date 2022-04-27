#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long sixtyFourBitKey = 69696969LL;
long long plaintext = 999999999999999LL;

long long DESRounds(long long);
int manglerFunction(int);
long long expansionFunction(int);
int getLH(long long);
int getRH(long long);
long long setBit64(long long, int, int);
int setBit32(int, int, int);
int tellBit32(int, int);
int tellBit64(long long, int);
void _debugPrint32bit(int);
void _debugPrint64bit(long long);
long long XOR48(long long, long long);
int sBox1(long long, int);
int sBox2(long long, int);
int sBox3(long long, int);
int sBox4(long long, int);
int sBox5(long long, int);
int sBox6(long long, int);
int sBox7(long long, int);
int sBox8(long long, int);
long long reconstructText(int, int);


// Something about the function is not working, but I cant find what specifically is broken.
// I tried debugging many of the functions and narrowed the problem down to the mangler function.
// I believe the Sbox functions are the actual problem, but I am out of time. Sorry :(


////////////////////////////////////////////////////////////////
// S Boxes

// S Box Values come from: https://www.techiedelight.com/des-implementation-c/

int S1[4][16] =
{
        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
        0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
        4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
};
 
int S2[4][16] =
{
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
};
 
int S3[4][16] =
{
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
};
 
int S4[4][16] =
{
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
};

int S5[4][16] =
{
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
};
 
int S6[4][16] =
{
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
};
 
int S7[4][16]=
{
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
};

int S8[4][16]=
{
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};
////////////////////////////////////////////////////////////////

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
    long long exandedRHxorKey = XOR48(exandedRHxorKey, sixtyFourBitKey);
    int sboxedNum = 0;
    sboxedNum = sBox1(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox2(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox3(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox4(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox5(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox6(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox7(exandedRHxorKey, sboxedNum);
    sboxedNum = sBox8(exandedRHxorKey, sboxedNum);
    return sboxedNum;
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
    long long retValue = 0LL;
    int RHbit = 0;
    int keyBit = 0;
    for (int i = 0; i < 48; i++) {
        RHbit = tellBit64(RH, i);
        keyBit = tellBit64(key, i);
        int xor = RHbit ^ keyBit;
        if (xor == 0) {
            retValue = setBit64(retValue, i, 0);
        } else {
            retValue = setBit64(retValue, i, 1);
        }
    }
    return retValue;
}

int sBox1(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 0);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 5);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1; i < 5; i++) {
        bit = tellBit64(expandedRHxorKey, i);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S1[row][column];
    for (int i = 0, iRH = 0; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox2(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 6);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey,11);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 7; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S2[row][column];
    for (int i = 0, iRH = 4; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox3(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 12);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 17);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 13; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S3[row][column];
    for (int i = 0, iRH = 8; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox4(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 18);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 23);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 19; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S4[row][column];
    for (int i = 0, iRH = 12; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox5(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 24);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 29);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 25; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S5[row][column];
    for (int i = 0, iRH = 16; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox6(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 30);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 35);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 31; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S6[row][column];
    for (int i = 0, iRH = 20; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox7(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 36);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 41);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 37; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S7[row][column];
    for (int i = 0, iRH = 24; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

int sBox8(long long expandedRHxorKey, int newRH) {
    int row = 0;
    int bit = tellBit64(expandedRHxorKey, 42);
    row = setBit32(row, 0, bit);

    bit = tellBit64(expandedRHxorKey, 47);
    row = setBit32(row, 0, bit);

    int column = 0;
    for (int i = 1, iKey = 43; i < 5; i++, iKey++) {
        bit = tellBit64(expandedRHxorKey, iKey);
        column = setBit32(column, i, bit);
    }

    int sboxNum = S8[row][column];
    for (int i = 0, iRH = 28; i < 4; i++, iRH++) {
        bit = tellBit32(sboxNum, i);
        newRH = setBit32(newRH, iRH, bit);
    }
    return newRH;
}

long long reconstructText(int rightHalf, int leftHalf) {
    long long text = 0LL;
    int bit = 0;
    for (int i = 0; i < 32; i++) {
        bit = tellBit32(rightHalf, i);
        text = setBit64(text, i, bit);
    }
    
    for (int i = 32; i < 64; i++) {
        bit = tellBit32(leftHalf, (i-32));
        text = setBit64(text, i, bit);
    }
    return text;
}

int getLH(long long text) {
    int leftHalf = 0;
    int bit;
    for (int i = 32; i < 64; i++) {
        bit = tellBit64(text, i);
        leftHalf = setBit32(leftHalf, (i-32), bit);
    }
    return leftHalf;
}

int getRH(long long text) {
    int rightHalf = 0;
    int bit;
    for (int i = 0; i < 32; i++) {
        bit = tellBit64(text, i);
        rightHalf = setBit32(rightHalf, i, bit);
    }
    return rightHalf;
}

long long DESRounds(long long text) {
    int LH = 0;
    int RH = 0;
    int newRH = 0;
    for (int i = 0; i < 1; i++) {
        LH = getLH(text);
        RH = getRH(text);
        newRH = manglerFunction(RH);
        newRH = LH ^ RH;
        text = reconstructText(newRH, RH);
    }
    return text;
}

long long DESRoundsDecrypt(long long text) {
    int LH = 0;
    int RH = 0;
    int newRH = 0;
    for (int i = 0; i < 1; i++) {
        RH = getLH(text);
        LH = getRH(text);
        newRH = manglerFunction(RH);
        newRH = LH ^ RH;
        text = reconstructText(newRH, RH);
    }
    return text;
}

int main() {
    _debugPrint64bit(plaintext);
    long long ciphertext = DESRounds(plaintext);
    long long decrypt = DESRoundsDecrypt(ciphertext);
    _debugPrint64bit(decrypt);
}