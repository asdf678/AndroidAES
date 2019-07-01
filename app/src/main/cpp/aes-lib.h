//
// Created by tzy on 2019/4/16.
//

#ifndef SECRET_H
#define SECRET_H

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif






JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeEncryptECB(JNIEnv *env, jobject instance,
                                                             jbyteArray key_,
                                                             jbyteArray buf_);

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeDecryptECB(JNIEnv *env, jobject instance,
                                                             jbyteArray key_,
                                                             jbyteArray buf_);

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeEncryptCTR(JNIEnv *env, jobject instance,
                                                             jbyteArray key_,
                                                             jbyteArray buf_, jbyteArray iv_);

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeDecryptCTR(JNIEnv *env, jobject instance,
                                                             jbyteArray key_,
                                                             jbyteArray buf_, jbyteArray iv_);

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeEncryptCBC(JNIEnv *env, jobject instance,
                                                             jbyteArray key_,
                                                             jbyteArray buf_, jbyteArray iv_);

JNIEXPORT jbyteArray JNICALL
Java_aes_AESUtil_nativeDecryptCBC(JNIEnv *env, jobject instance,
                                                             jbyteArray key_,
                                                             jbyteArray buf_, jbyteArray iv_);

JNIEXPORT jstring JNICALL
Java_aes_AESUtil_nativeDecryptECB2Str(JNIEnv *env,
                                                                       jobject instance,
                                                                       jbyteArray key_,
                                                                       jbyteArray buf_) ;

#ifdef __cplusplus
}
#endif
#endif //SECRET_H

