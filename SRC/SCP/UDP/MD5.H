#ifndef MD5_H
#define MD5_H

typedef unsigned int word32;
	/* this used to get done in header files that used gnu autoconf,
	 * but to avoid lifting non-public-domain code, I have extracted
	 * the relevant line. Therefore, if ints aren't 32 bits on your
	 * machine, you should fix this typedef.
	 */
typedef word32 uint32;

struct MD5Context {
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf,
	       unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void MD5Transform(uint32 buf[4], const unsigned char in[64],
			struct MD5Context *ctx);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

void debugStatus(char *m, struct MD5Context *ctx);
void dumpBytes(unsigned char *b, int len);

#endif /* !MD5_H */
