//
// Created by tzy on 2019/4/16.
//
/*
0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20
14	1	1	1	1	1	1	1	1	1	1	1	1	1	1	'\0'	'\0'	'\0'
en	9	9	9	9	9	9	9	9	9	9	9	9	9	9	9	9	'\0'
de	1	1	1	1	1	1	1	1	1	1	1	1	1	1	'\0'	'\0'	'\0'


16	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	'\0'
en	9	9	9	9	9	9	9	9	9	9	9	9	9	9	9	9	'\0'
de	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	'\0'


20	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	'\0'
en	9	9	9	9	9	9	9	9	9	9	9	9	9	9	9	9	1	1	1	1	'\0'
de	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1
 */
#include <cstdio>
#include <cstring>
//#include<exception>
#include "aes-lib.h"
#include <android/log.h>
# include <stdio.h>

#include <algorithm>

const char *LOG_TGA = "AES_LOG_TGA";
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TGA, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TGA, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TGA, __VA_ARGS__)
#define DEFAULT_EMPTY '\0'

extern "C" {
#include "aes.h"
}

#if defined (AES128)
    int m_key_length = 16;

#elif defined(AES192)
    int m_key_length = 24;

#elif defined(AES256)
    int m_key_length = 32;

#endif


static void
encrypt_cbc(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length);

static void
decrypt_cbc(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length);


static void
encrypt_ctr(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length);

static void
decrypt_ctr(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length);


static void decrypt_ecb(const uint8_t *key, uint8_t *_io, const jsize result_size,
                        const jsize min_result_length);

static void encrypt_ecb(const uint8_t *key, uint8_t *_io, const jsize result_size,
                        const jsize min_result_length);


static jsize copy_buf(JNIEnv *env, jbyteArray array, jsize _arr_length, uint8_t *&result,
                      const jsize min_result_length);


static jbyteArray
create_encrypt_result(JNIEnv *env, const uint8_t *result, const jsize result_size);


static jsize generate_valid_buf_size(const uint8_t *result, const jsize result_size);


static jbyteArray create_decrypt_result(JNIEnv *env, const uint8_t *result,
                                        const jsize result_size);


static void
safe_create_secret_key(jbyte *key, jint key_size, const jsize valid_length,
                       uint8_t *&key_result);
static void print_arr(const uint8_t *key_result, int len) {
    for (int i = 0; i < len; ++i) {
        LOGI("@@%i", *(key_result + i));

    }
    LOGI("@@%s", "-----------------------------------------");

}
/*
 * ecb:
 * AES256: key_length :32,in_length:16,out_length:16
 * AES192: key_length :24,in_length:16,out_length:16
 * AES128: key_length :16,in_length:16,out_length:16
 * */



JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeEncryptECB(JNIEnv *env, jobject instance,
                                                                   jbyteArray key_,
                                                                   jbyteArray buf_) {
    jsize buf_len = env->GetArrayLength(buf_);
    jsize key_len = env->GetArrayLength(key_);
//    print_arr(reinterpret_cast<uint8_t *>(env->GetByteArrayElements(buf_, NULL)), buf_len);
    LOGI("buf length:%i,key length:%i", buf_len, key_len);
    uint8_t *key_result = NULL;

    safe_create_secret_key(env->GetByteArrayElements(key_, NULL), key_len, m_key_length, key_result);
    LOGI("@@@%s", "encode");

    uint8_t *result = NULL;
    jsize result_len = copy_buf(env, buf_, buf_len, result, 16);
    encrypt_ecb((key_result), result, result_len, 16);
    delete[]key_result;
    return create_encrypt_result(env, result, result_len);


}

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeDecryptECB(JNIEnv *env, jobject instance,
                                                                   jbyteArray key_,
                                                                   jbyteArray buf_) {


    jsize buf_len = env->GetArrayLength(buf_);
    jsize key_len = env->GetArrayLength(key_);

    uint8_t *key_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(key_, NULL), key_len, m_key_length, key_result);

    uint8_t *result = NULL;
    jsize result_len = copy_buf(env, buf_, buf_len, result, 16);
    decrypt_ecb(key_result, result, result_len, 16);
    delete[]key_result;
    return create_decrypt_result(env, result, result_len);
}


/*
 * ctr:
 * AES256: key_length :32,in_length:64,out_length:64,iv_length:16
 * AES192: key_length :24,in_length:64,out_length:64,iv_length:16
 * AES128: key_length :16,in_length:64,out_length:64,iv_length:16
 * */


JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeEncryptCTR(JNIEnv *env, jobject instance,
                                                                   jbyteArray key_,
                                                                   jbyteArray buf_,
                                                                   jbyteArray iv_) {
//    jbyte *key = env->GetByteArrayElements(key_, NULL);
//    jbyte *iv = env->GetByteArrayElements(iv_, NULL);

    jsize buf_len = env->GetArrayLength(buf_);
    jsize key_len = env->GetArrayLength(key_);
    jsize iv_len = env->GetArrayLength(iv_);

    uint8_t *key_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(key_, NULL), key_len, m_key_length, key_result);

    uint8_t *iv_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(iv_, NULL), iv_len, 16, iv_result);

    LOGI("buf length:%i,key length:%i,iv length:%i", buf_len, key_len, iv_len);

    uint8_t *result = NULL;
    jsize result_len = copy_buf(env, buf_, buf_len, result, 64);
    encrypt_ctr(key_result,
                iv_result,
                result, result_len, 64);
    delete[] key_result;
    delete[] iv_result;
    return create_encrypt_result(env, result, result_len);


}

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeDecryptCTR(JNIEnv *env, jobject instance,
                                                                   jbyteArray key_,
                                                                   jbyteArray buf_,
                                                                   jbyteArray iv_) {
//    jbyte *key = env->GetByteArrayElements(key_, NULL);
//    jbyte *iv = env->GetByteArrayElements(iv_, NULL);

    jsize buf_len = env->GetArrayLength(buf_);
    jsize key_len = env->GetArrayLength(key_);
    jsize iv_len = env->GetArrayLength(iv_);

    uint8_t *key_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(key_, NULL), key_len, m_key_length, key_result);

    uint8_t *iv_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(iv_, NULL), iv_len, 16, iv_result);
    uint8_t *result = NULL;
    jsize result_len = copy_buf(env, buf_, buf_len, result, 64);
    decrypt_ctr(key_result, iv_result,
                result, result_len, 64);
    delete[] key_result;
    delete[] iv_result;
    return create_decrypt_result(env, result, result_len);
}

/*
 * cbc
 * AES256: key_length :32,in_length:64,out_length:64,iv_length:16
 * AES192: key_length :24,in_length:64,out_length:64,iv_length:16
 * AES128: key_length :16,in_length:64,out_length:64,iv_length:16
 * */


JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeEncryptCBC(JNIEnv *env, jobject instance,
                                                                   jbyteArray key_,
                                                                   jbyteArray buf_,
                                                                   jbyteArray iv_) {
    jsize buf_len = env->GetArrayLength(buf_);
    jsize key_len = env->GetArrayLength(key_);
    jsize iv_len = env->GetArrayLength(iv_);

    uint8_t *key_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(key_, NULL), key_len, m_key_length, key_result);

    uint8_t *iv_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(iv_, NULL), iv_len, 16, iv_result);
    LOGI("buf length:%i,key length:%i,iv length:%i", buf_len, key_len, iv_len);
    uint8_t *result = NULL;
    jsize result_len = copy_buf(env, buf_, buf_len, result, 64);
    encrypt_cbc(key_result, iv_result,
                result, result_len, 64);
    delete[] key_result;
    delete[] iv_result;
    return create_encrypt_result(env, result, result_len);
}

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeDecryptCBC(JNIEnv *env, jobject instance,
                                                                   jbyteArray key_,
                                                                   jbyteArray buf_,
                                                                   jbyteArray iv_) {
    jsize buf_len = env->GetArrayLength(buf_);
    jsize key_len = env->GetArrayLength(key_);
    jsize iv_len = env->GetArrayLength(iv_);

    uint8_t *key_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(key_, NULL), key_len, m_key_length, key_result);

    uint8_t *iv_result = NULL;
    safe_create_secret_key(env->GetByteArrayElements(iv_, NULL), iv_len, 16, iv_result);
    uint8_t *result = NULL;
    jsize result_len = copy_buf(env, buf_, buf_len, result, 64);
    decrypt_cbc(key_result, iv_result,
                result, result_len, 64);
    delete[] key_result;
    delete[] iv_result;
    return create_decrypt_result(env, result, result_len);


}

static void
encrypt_cbc(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length) {
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    uint8_t *io;
    for (jsize i = 0; i < result_size; i += min_result_length) {
        io = _io + i;
        AES_CBC_encrypt_buffer(&ctx, io, 64);
    }

}

static void
decrypt_cbc(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length) {
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
//    memcpy(out, in, sizeof(uint8_t) * 64);



    uint8_t *io;
    for (jsize i = 0; i < result_size; i += min_result_length) {
        io = _io + i;
//        AES_ECB_encrypt(&ctx, io);
        AES_CBC_decrypt_buffer(&ctx, io, 64);

    }
}


static void
encrypt_ctr(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length) {
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    uint8_t *io;
    for (jsize i = 0; i < result_size; i += min_result_length) {
        io = _io + i;
//        AES_ECB_encrypt(&ctx, io);
//        AES_ECB_decrypt(&ctx, io);
        AES_CTR_xcrypt_buffer(&ctx, io, 64);


    }

//    memcpy(out, in, sizeof(uint8_t) * 64);
}

static void
decrypt_ctr(const uint8_t *key, const uint8_t *iv, uint8_t *_io, const jsize result_size,
            const jsize min_result_length) {
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);


    uint8_t *io;
    for (jsize i = 0; i < result_size; i += min_result_length) {
        io = _io + i;
//        AES_ECB_encrypt(&ctx, io);
        AES_CTR_xcrypt_buffer(&ctx, io, 64);

    }
}


static void decrypt_ecb(const uint8_t *key, uint8_t *_io, const jsize result_size,
                        const jsize min_result_length) {

    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

//    memcpy(out, in, sizeof(uint8_t) * 16);
    uint8_t *io;
    for (jsize i = 0; i < result_size; i += min_result_length) {
        io = _io + i;
//        AES_ECB_encrypt(&ctx, io);
        AES_ECB_decrypt(&ctx, io);

    }

}

static void encrypt_ecb(const uint8_t *key, uint8_t *_io, const jsize result_size,
                        const jsize min_result_length) {
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

//    memcpy(out, in, sizeof(uint8_t) * 16);
//    const jsize n = strlen(reinterpret_cast<const char *>(_io));

    uint8_t *io;
    for (jsize i = 0; i < result_size; i += min_result_length) {
        io = _io + i;
        AES_ECB_encrypt(&ctx, io);
    }


}


/*
 * arr_length    min_result_length   result_size     end_length
 * 10                 16                16               17
 *  16                16                16               17
 *  20                16                20               21
 *
 *
 * */

static jsize copy_buf(JNIEnv *env, jbyteArray array, jsize _arr_length, uint8_t *&result,
                      const jsize min_result_length) {
    const jsize result_size =
            (_arr_length + min_result_length - 1) / min_result_length *
            min_result_length;//字符数组不满min_result_length的补充至min_result_length：长度必须是min_result_length的倍数，
//    const jsize result_size = std::max(arr_length, min_result_length);
//    const jsize end_length = result_size + 1;//+1是为了结束占位符
    jsize n = std::min(_arr_length, result_size);
    result = new uint8_t[result_size];
//    jbyte *key = env->GetByteArrayElements(array, NULL);


//    memcpy((void *) result, (const void *) key, sizeof(uint8_t) * n);


    env->GetByteArrayRegion(array, 0, n, reinterpret_cast<jbyte *>(result));
    if (_arr_length < result_size) {
        LOGI("buf copy and fill size:%i", result_size - _arr_length);
        for (jsize i = _arr_length; i < result_size; ++i) {
            result[i] = DEFAULT_EMPTY;
        }
    }


    return result_size;
}


/*
 * result_size
 *  16
 *
 *  20
 * */
static jbyteArray
create_encrypt_result(JNIEnv *env, const uint8_t *result, const jsize result_size) {
//    jsize result_size = strlen(reinterpret_cast<const char *>(result));
    jbyteArray j_byte_result = env->NewByteArray(result_size);
    env->SetByteArrayRegion(j_byte_result, 0, result_size, reinterpret_cast<const jbyte *>(result));
    delete[] result;
    return j_byte_result;
}

/*
 * result_size
 *  16
 *
 *  20
 * */

static jsize generate_valid_buf_size(const uint8_t *result, const jsize result_size) {
    for (jsize i = 0; i < result_size; ++i) {
        if ((*(result + i)) == DEFAULT_EMPTY) {
            return i;
        }
    }
    return result_size;
}


static jbyteArray create_decrypt_result(JNIEnv *env, const uint8_t *result,
                                        const jsize result_size) {
    jsize valid_size = generate_valid_buf_size(result, result_size);
    jbyteArray j_byte_result = env->NewByteArray(valid_size);
    env->SetByteArrayRegion(j_byte_result, 0, valid_size, reinterpret_cast<const jbyte *>(result));
    delete[] result;
//    print_arr(result, result_size);


    return j_byte_result;
}

static void
safe_create_secret_key(jbyte *key, jint key_size, const jsize valid_length,
                       uint8_t *&key_result) {

    jsize n = std::min(key_size, valid_length);
    uint8_t *_key_result = new uint8_t[valid_length];
//    memcpy(key_result, key, key_size);
    memcpy((void *) _key_result, (const void *) key, n);


    for (int i = key_size; i < valid_length; ++i) {
        _key_result[i] = DEFAULT_EMPTY;
        LOGI("%s","##");
    }
    key_result = _key_result;
//    key_result = new uint8_t[valid_length];
//    for (int i = 0; i < valid_length; ++i) {
//        key_result[i] = 0x40;
//    }

}

