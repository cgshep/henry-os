#include "crypto_aead.h"
#include "api.h"
#include "string.h"
#include "tty.h"

int demo_ascon_aead()
{
    unsigned char n[CRYPTO_NPUBBYTES] = {0, 1, 2,  3,  4,  5,  6,  7,
	8, 9, 10, 11, 12, 13, 14, 15};
    unsigned char k[CRYPTO_KEYBYTES] = {0, 1, 2,  3,  4,  5,  6,  7,
	8, 9, 10, 11, 12, 13, 14, 15};
    unsigned char a[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    unsigned char m[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    unsigned char m1[16] = {0};
    unsigned char c[32], h[32], t[32];
    unsigned long long alen = 16;
    unsigned long long mlen = 16, m1_len=16, i;
    unsigned long long clen = CRYPTO_ABYTES;

    int res = 0;
    puts(" ** m ** \n");
    for (i = 0; i < mlen; i++){
	terminal_write_hex(m[i]);
	putch(' ');
    }
    puts("\n");

    res = crypto_aead_encrypt(c, &clen, m, mlen, a, alen, (void*)0, n, k);
    puts("ASCON res: ");
    terminal_write_int(res);
    puts("\n ** Ciphertext ** \n");
    for (i = 0; i < clen; i++){
	terminal_write_hex(c[i]);
	putch(' ');
    }
    puts("\n");

    res = crypto_aead_decrypt(m1, &m1_len, (void*)0, c, clen, a, alen, n, k);
    puts("ASCON res: ");
    terminal_write_int(res);
    puts("\n ** m1 ** \n");
    for (i = 0; i < m1_len; i++){
	terminal_write_hex(m1[i]);
	putch(' ');
    }
    puts("\n");

    return res;
}
