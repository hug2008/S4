//
//  c4Keys.h
//  C4
//
//  Created by vincent Moscaritolo on 11/10/15.
//  Copyright © 2015 4th-A Technologies, LLC. All rights reserved.
//

#ifndef c4Keys_h
#define c4Keys_h

#include "c4pubtypes.h"

#ifdef __clang__
#pragma mark - Key import Export.
#endif



#define kC4KeyPBKDF2_SaltBytes      8
#define kC4KeyPBKDF2_HashBytes      16

typedef struct C4KeyContext *      C4KeyContextRef;

#define	kInvalidC4KeyContextRef		((C4KeyContextRef) NULL)

#define C4KeyContextRefIsValid( ref )		( (ref) != kInvalidC4KeyContextRef )
enum C4KeyType_
{
    kC4KeyType_Symmetric           = 1,
    kC4KeyType_Tweekable           = 2,
    kC4KeyType_PBKDF2              = 3,
    
    kC4KeyType_Invalid           =  kEnumMaxValue,
    
    ENUM_FORCE( C4KeyType_ )
};

ENUM_TYPEDEF( C4KeyType_, C4KeyType   );

typedef struct C4KeySymmetric_
{
    Cipher_Algorithm    symAlgor;
    int                 keylen;
    uint8_t        		symKey[64];
    
}C4KeySymmetric;


typedef struct C4KeyTBC_
{
    TBC_Algorithm       tbcAlgor;
    int                 keybits;
    uint64_t            key[16];
    
}C4KeyTBC;


typedef struct C4KeyPBKDF2_
{
    C4KeyType               keyAlgorithmType;
    union {
        TBC_Algorithm       tbcAlgor;
        Cipher_Algorithm    symAlgor;
    };

    uint8_t             keyHash[kC4KeyPBKDF2_HashBytes];
    uint8_t             salt[kC4KeyPBKDF2_SaltBytes];
    uint32_t            rounds;
 Cipher_Algorithm       encyptAlgor;
    uint8_t             encrypted[256];
    size_t              encryptedLen;
    
}C4KeyPBKDF2;

typedef struct C4KeyContext    C4KeyContext;

struct C4KeyContext
{
    
#define kC4KeyContextMagic		0x43346B79
    uint32_t            magic;
    C4KeyType           type;
    
    union {
        C4KeySymmetric  sym;
        C4KeyTBC        tbc;
        C4KeyPBKDF2     pbkdf2;
    };
    
};


C4Err C4Key_NewSymmetric(Cipher_Algorithm       algorithm,
                         const void             *key,
                         C4KeyContextRef    *ctx);

C4Err C4Key_NewTBC(     TBC_Algorithm       algorithm,
                   const void          *key,
                   C4KeyContextRef     *ctx);

void C4Key_Free(C4KeyContextRef ctx);


C4Err C4Key_SerializeToPassPhrase(C4KeyContextRef  ctx,
                                  const char       *passphrase,
                                  size_t           passphraseLen,
                                  uint8_t          **outData,
                                  size_t           *outSize);

C4Err C4Key_Deserialize( uint8_t *inData, size_t inLen, C4KeyContextRef *ctx);


C4Err C4Key_VerifyPassPhrase(   C4KeyContextRef  ctx,
                                const char       *passphrase,
                                size_t           passphraseLen);


C4Err C4Key_DecryptFromPassPhrase(   C4KeyContextRef  passCtx,
                                 const char       *passphrase,
                                 size_t           passphraseLen,
                                 C4KeyContextRef       *symCtx);

#endif /* c4Keys_h */