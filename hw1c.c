//Ian Hunter
//Austin-sec02
//2-17-2016

#include <stdint.h>
#include <stdio.h>

/*
 * This function should take in 64-bits of plaintext and a 128 bit key.
 * It should encrypt the text in place.  Note that text[0] is the left 32
 * bits of the plaintext and text[1] is the right 32 bits of plaintext.
 * Also, note that the key is stored in 4 32-bit blocks.
 */
void encrypt(uint32_t* text, uint32_t* key) {
    /* YOUR CODE HERE */
    uint32_t delta = 0x9e3779b9;
    uint32_t sum = 0;
    uint32_t L = text[0];
    uint32_t R = text[1];
    int i = 0; 
    for(i;i<32;i++)
    {
        sum+= delta;
        L += ((R<<4) + key[0]) ^ (R+sum) ^ ((R>>5) + key[1]);
        R += ((L<<4) + key[2]) ^ (L+sum) ^ ((L>>5) + key[3]);
        
    }
    text[0] = L;
    text[1] = R;
    
}

/*
 * This function should take in 64-bits of ciphertext and a 128 bit key.
 * It should decrypt the text in place.
 */
void decrypt(uint32_t* text, uint32_t* key) {
    /* YOUR CODE HERE */
    uint32_t delta = 0x9e3779b9;
    uint32_t sum = 0;
    uint32_t L = text[0];
    uint32_t R = text[1];
    
    sum = delta << 5;
    
    int i = 0; 
    for(i;i<32;i++)
    {
        R -= (L<<4) + key[2] ^ L+sum ^ (L>>5) + key[3];
        L -= (R<<4) + key[0] ^ (R+sum) ^ (R>>5) + key[1];
        sum -=delta;
    }
    text[0] = L;
    text[1] = R;
}

/*
 * Utility function to print your results
 */
void print_arr (uint32_t* arr, int size) {
    int i;
    for (i=0; i<size; i++) {
        printf("%02X ", arr[i]);
    }
    printf("\n");
}

/*
 * This function encrypts several blocks with TEA, using electronic codebook mode (ECB).
 */
void ecb_mode_test() {
    int i;
    uint32_t key[] = {0x7e7a9cb4, 0x89a201f3, 0x14d6fed4, 0xbe21afea};

    uint32_t text[] = { 0x9e4587e3, 0xe49383a2,
                        0xe1432902, 0xbe790210,
                        0xa39ffe11, 0x93333276,
                        0x21247348, 0x85956106,
                        0x169f5ea6, 0xdb8a7137 };

    printf("(ECB) plaintext = ");
    print_arr(text, 10);

    for (i=0; i<10; i+=2) {
        encrypt(text+i, key);
    }

    printf("(ECB) ciphertext= ");
    print_arr(text, 10);

    for (i=0; i<10; i+=2) {
        decrypt(text+i, key);
    }

    printf("(ECB) plaintext = ");
    print_arr(text, 10);
}

/*
 * This function (when you make your changes) should encrypt several blocks using TEA,
 * but in cipher block chaining (CBC) mode.
 *
 * USE THE SAME ENCRYPT AND DECRYPT FUNCTIONS AS ECB MODE
 */
void cbc_mode_test() {
    int i;
    uint32_t key[] = {0x7e7a9cb4, 0x89a201f3, 0x14d6fed4, 0xbe21afea};

    uint32_t text[] = { 0x9e4587e3, 0xe49383a2,
                        0xe1432902, 0xbe790210,
                        0xa39ffe11, 0x93333276,
                        0x21247348, 0x85956106,
                        0x169f5ea6, 0xdb8a7137 };

    /* XOR (the ^ operator in C) these values with the first two blocks of your plaintext */
    uint32_t iv[] = { 0x74f78c88, 0x8d8844a1 };
    

    printf("(CBC) plaintext = ");
    print_arr(text, 10);

    
    /*************************** YOUR CODE HERE ***************************************/
    uint32_t left = 0;
    uint32_t right = 0;
    //save the plain text for encrypting
     for (i=0; i<10; i+=2) 
     {
         if(i==0) 
         {
             left = iv[0] ^ text[0];
             right = iv[1] ^ text[1];
             text[0] = left;
             text[1] = right;
            encrypt(text+i,key);
            left = text[0];
            right = text[1];
         }
         else
         {
             left = left ^ text[i];
             right = right ^ text[i+1];
            encrypt(text+i, key);
            left = text[i];
            right = text[i+1];
         }
    }
    
    printf("(CBC) ciphertext= ");
    print_arr(text, 10);

    
    /*************************** YOUR CODE HERE ***************************************/
    uint16_t cipherL = 0;
    uint16_t cipherR = 0;
    uint16_t cipherL2 = 0;
    uint16_t cipherR2 = 0;
    for (i=0; i<10; i+=2) 
     {
         if(i==0) 
         {
             cipherL = text[0];
             cipherR = text[1];
             decrypt(text+i,key);
             text[0] = iv[0] ^ text[0];
             text[1] = iv[1] ^ text[1];
             
         }
         else
         {
             cipherL2 = text[i];
             cipherR2 = text[i+1];
             decrypt(text+i,key);
             text[i] = cipherL ^ text[i];
             text[i+1] = cipherR ^ text[i+1];
             cipherL = cipherL2;
             cipherR = cipherR2;
         }
    }
    
    printf("(CBC) plaintext = ");
    print_arr(text, 10);
}

/*
 * This function (when you make your changes) should encrypt several blocks using TEA,
 * but in counter (CTR) mode.
 *
 * USE THE SAME ENCRYPT FUNCTION AS ECB MODE.
 *
 * You will not need a decrypt function
 */
void ctr_mode_test() {
    int i;
    uint32_t key[] = {0x7e7a9cb4, 0x89a201f3, 0x14d6fed4, 0xbe21afea};

    uint32_t text[] = { 0x9e4587e3, 0xe49383a2,
                        0xe1432902, 0xbe790210,
                        0xa39ffe11, 0x93333276,
                        0x21247348, 0x85956106,
                        0x169f5ea6, 0xdb8a7137 };

    uint32_t iv_left = 0x74f78c88;
    uint32_t iv_right = 0x8d8844a1;
    uint32_t iv[2];

    printf("(CTR) plaintext = ");
    print_arr(text, 10);

    /*************************** YOUR CODE HERE ***************************************/
    uint64_t p = 0;
    uint64_t c = 0;
    
    iv[0] = iv_left;
    iv[1] = iv_right;
    for (i=0; i<=10; i+=2) {
        
        p = text[i];
        encrypt(iv+i, key);
        text[i] = p ^ text[i];
    }

    printf("(CTR) ciphertext= ");
    print_arr(text, 10);

    /*************************** YOUR CODE HERE ***************************************/
    
    for (i=0; i<=10; i+=2) {
        
        c = text[i];
        encrypt(iv+i, key);
        text[i] = c ^ text[i];
    }
    
    printf("(CTR) plaintext = ");
    print_arr(text, 10);
}


int main(int argc, const char* argv[]) {
    printf("\nECB mode test\n");
    ecb_mode_test();

    printf("\nCBC mode test\n");
    cbc_mode_test();

    printf("\nCTR mode test\n");
    ctr_mode_test();

    return 0;
}