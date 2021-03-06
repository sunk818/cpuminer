// Not-Copyrighted 2019-2020 The BiblePay Developers 
// The Gospel is available to all humans on the planet, come and be saved under Jesus Christ!
// Distributed under the MIT/X11 software license, see the accompanying

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "miner.h"


#include "md5.h"
#include "aes.h"


static unsigned char *b[31102];
static double iVerseFactor = .4745708; //Verses available divided by bits per octet
// (Last verse:) Verse #31101 : The grace of our Lord Jesus Christ be with you all. Amen.";
extern void sha256d_80_swap_bbp(uint32_t *hash, const uint32_t *data);
extern bool fDebug;

void initkjv_dict(unsigned char *b[31102]);

void initkjv()
{
	initkjv_dict(b);
}

// prints string as hex
void phex(uint8_t* str, uint8_t len)
{
    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}

int ATD(int iAscii)
{
	int iOut = 0;
	switch(iAscii)
	{
		case 48:
			iOut = 0;
			break;
		case 49:
			iOut = 1;
			break;
		case 50:
			iOut=2;
			break;
		case 51:
			iOut=3;
			break;
		case 52:
			iOut=4;
			break;
		case 53:
			iOut=5;
			break;
		case 54:
			iOut=6;
			break;
		case 55:
			iOut=7;
			break;
		case 56:
			iOut=8;
			break;
		case 57:
			iOut=9;
			break;
		case 97:
			iOut=10;
			break;
		case 98:
			iOut=11;
			break;
		case 99:
			iOut=12;
			break;
		case 100:
			iOut=13;
			break;
		case 101:
			iOut=14;
			break;
		case 102:
			iOut=15;
			break;
	}
	return iOut;
}

int HTD(char *sOctet)
{
	int number = (int)strtol(sOctet, NULL, 0);
	return number;
}


char *BibleMD5(char *str, int length)
{

    mbedtls_md5_context ctx;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    mbedtls_md5_init( &ctx );
    mbedtls_md5_starts_ret( &ctx );
	mbedtls_md5_update_ret( &ctx, str, length );
    mbedtls_md5_finish_ret( &ctx, digest );
    mbedtls_md5_free( &ctx );

    for (int n = 0; n < 16; ++n) 
	{
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }
	if (fDebug)
		printf("\nSource %s MD5 %s\n", str, out);

    return out;
}


unsigned char *ExtractSubstring(unsigned char *myBigString, int iStartPos, int iExtractionLength)
{
	char substringer[iExtractionLength];
	memcpy( substringer, &myBigString[iStartPos], iExtractionLength );
}

unsigned char *GVFromHash(unsigned char *myHash) 
{
  unsigned char *myOctets[8];
  unsigned char *suffix = "\r\n";
  unsigned char *prefix = "0x";
  int iOctetNumber = 0; 
  for (int i = 0; i < 32; i = i + 4)
  {
	  char *sOctet = calloc(7, 1);
	  memcpy(sOctet, prefix, 2);
 	  memcpy(sOctet + 2, myHash + i, 4);
	  int iVerse = HTD(sOctet) * iVerseFactor;
	  free(sOctet);

	  myOctets[iOctetNumber] = calloc(strlen(b[iVerse]) + 3, 1);
	  memcpy(myOctets[iOctetNumber], b[iVerse], strlen(b[iVerse]));
	  memcpy(myOctets[iOctetNumber] + strlen(b[iVerse]), suffix, 2);
	  iOctetNumber++; 	 
  }
  int iSize = 0;
  for (int i1 = 0; i1 <= 7; i1++)
  {
	  iSize += strlen(myOctets[i1]);
  }

  unsigned char *sVerses = calloc(iSize + 1, 1);
  int iOffset = 0;
  for (int i2 = 0; i2 <= 7; i2++)
  {
	  memcpy(sVerses + iOffset, myOctets[i2], strlen(myOctets[i2]));
	  iOffset += strlen(myOctets[i2]);
  }

  for (int i3 = 0; i3 < 8; i3++)
  {
	  free(myOctets[i3]);
  }
  return sVerses; 
}

// BASE 64 AREA
static const unsigned char pr2six[256] =
{
    /* ASCII table */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode_len(const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);

    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    return nbytesdecoded + 1;
}

int Base64decode(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;

    while (nprbytes > 4) {
    *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    bufin += 4;
    nprbytes -= 4;
    }

    if (nprbytes > 1) {
    *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
    *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
    *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len(int len)
{
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode(char *encoded, const char *string, int len)
{
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
    *p++ = basis_64[(string[i] >> 2) & 0x3F];
    *p++ = basis_64[((string[i] & 0x3) << 4) |
                    ((int) (string[i + 1] & 0xF0) >> 4)];
    *p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                    ((int) (string[i + 2] & 0xC0) >> 6)];
    *p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
    *p++ = basis_64[(string[i] >> 2) & 0x3F];
    if (i == (len - 1)) {
        *p++ = basis_64[((string[i] & 0x3) << 4)];
        *p++ = '=';
    }
    else {
        *p++ = basis_64[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
    }
    *p++ = '=';
    }

    *p++ = '\0';

	int iResult = p - encoded;
	return iResult;
}

// END OF BASE 64 AREA

static void ReverseUint256(uint8_t toSwap[], uint8_t swapped[])
{
	for (int i = 0; i < 32; i++)
	{
		int iSource = 31 - i;
		swapped[i] = toSwap[iSource];
	}
}

static void ReverseUint256b(uint8_t toSwap[], uint8_t swapped[])
{
	for (int i = 0; i < 32; i++)
	{
		int iSource = 31 - i;
		swapped[i] = toSwap[iSource];
	}
}



static void longDivision(uint8_t number[], int divisor) 
{ 
	// Its not too trivial to divide a uint256, but we do it here in 8 bit chunks - as a result, the division is a very long process
	uint8_t ans[32] = {0};
	int answer_index = 0;
    // Find prefix of number that is larger than the divisor. 
    int idx = 0;
	int WIDTH = 32;
    int temp = number[idx];
    while (temp < divisor) 
	{
       temp = temp * 256 + (number[++idx]); 
	}
      
    // Repeatedly divide divisor with temp. After every division, update temp to include one more digit. 
    while (WIDTH > idx) 
    { 
        // Store result in answer i.e. temp / divisor 
		ans[answer_index] = (temp/divisor);
		answer_index++;
        // Take next digit of number 
        temp = (temp % divisor) * 256 + number[++idx]; 
    } 
      
	// Zero pad the answer into the vector
	for (int i = 0; i < 32; i++)
	{
		number[i] = 0;
	}
	int iZeroes = 32 - answer_index;
	memcpy(number + iZeroes, ans, answer_index);
} 

static void SerializeHash(unsigned char *sMD5, uint8_t hash[])
{
	// The Bitcoin SerializeHash function prefixes the data to be hashed with the length first:
	unsigned char *sha = calloc(3 + strlen(sMD5), 1);
	unsigned char *space = " ";
	memcpy(sha, space, 1);
	memcpy(sha + 1, sMD5, strlen(sMD5));
	uint8_t hashb[32] = {0x0};
	sha256d(hashb, sha, strlen(sha));
	// Reverse the hash to be represented in uint256
	ReverseUint256b(hashb, hash);
	free(sha);

	if (fDebug)
	{
		unsigned char *genhash = malloc(65);
		bin2hex(genhash, (unsigned char *)hash, 32);
		free(genhash);
		printf("Sha256Hash: %s\n", genhash);
	}
}

static void Multiply(uint8_t Backward[], uint32_t Factor)
{
	// Multiplies a uint256 * uint32_t
	// Start with a reversed uint256
	// RAndrews:  I ported this straight out of bitcoin - Thanks Bitcoin!
	uint8_t Source[32] = {0};
	ReverseUint256(Backward, Source);
    uint64_t carry = 0;
	int WIDTH = 32;
    for (int i = 0; i < WIDTH; i++) 
	{
        uint64_t n = carry + (uint64_t)Factor * Source[i];
		Source[i] = n & 0xff;
        carry = n >> 8;
    }
	// Return the forward representation
	ReverseUint256(Source, Backward);
}


 // The various BibleHash functions for BiblePay - writen July 2017
 // The purpose: Decrease the liklihood of porting the hash to an ASIC, and spread the gospel to the cryptocommunity
 // Given an input hash provide a bible hash based on the chained verses found in the bits of the input hash.
 // Encryption step: Decrease the possibility of porting to a GPU
 // The primary reasons we created biblehash: Allow commodity PCs to mine, encourage full node participation, decrease liklihood of porting to ASIC, discourage bot-nets and mining monopolies 
 // The Bible also helps lead IT professionals and crypto-enthusiasts to the Gospel of Jesus Christ
 // In the wallet we provide prayer pages, a bible browser, chained verses in getblock, and future Christian Economy features.
void BibleHashV2(uint8_t uHash[], bool fLate)
{
	if (fDebug)
	{
		phex(uHash, 32);
	}

  	// 1. Start with a source uint256 sha256 hash from the block == uint8_t[32].
	// 2. BibleEncrypt(vchPlaintext, vchCiphertext);
 	uint8_t uPOBH[48] = { 0x0 };
	Encrypt_AES_CBC_II(uHash, uPOBH);
	if (fDebug)
	{
		printf("Encrypt_AES_CBC_II ");
		phex(uPOBH, 48);
	}


	// 3.  Encode uPOBH->Base64
	char *sBase64 = malloc(96);
	Base64encode(sBase64, uPOBH, 48);
	if (fDebug)
		printf("BASE 64 POBH %s ", sBase64);
	// 4. Generate MD5 Hash of Base64
	char *md1 = BibleMD5(sBase64, strlen(sBase64));
	if (fDebug)
		fprintf(stderr, "MD5 POBH %s : \n", md1);
    unsigned char *myVerses = GVFromHash(md1);
	// 5. Generate MD5 from Verses
	char *md5 = BibleMD5(myVerses, strlen(myVerses));
	free(myVerses);

	if (fDebug)
		fprintf(stderr, "MD5 after verses %s ", md5);
	// 6. Sha256 Hash from Serialized value of md5 : 
	uint8_t hash256[32] = {0x0}; 
	SerializeHash(md5, hash256);
	if (fDebug)
	{	printf("serialized256 hash:");
		phex(hash256, 32);
	}
	// Determine if block is late
	int nFactor = fLate ? 8400 : 1777;
	// Multiplication - Always multiply by 420 - per legacy code
	Multiply(hash256, 420); 
	// Divide by Factor
	if (fDebug)
		phex(hash256, 32);

	longDivision(hash256, nFactor);
	if (fDebug)
	{
		printf("POBH: ");
		phex(hash256, 32);
	}
	// Move result back to uHash
	memcpy(uHash, hash256, 32);

	free(sBase64);
	free(md1);
	free(md5);

}


void ArgToUint256(char *arg, uint8_t uHash[])
{
	// We have a 1-64 byte hex in
	uint8_t buf[32] = {0x0};
	hex2bin(buf, arg, 32);
	// Align to 256bit
	int size = strlen(arg)/2;
	int offset = 32 - size;
	memcpy(uHash + offset, buf, size);
}

