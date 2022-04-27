/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class libsodiumjni_internal_SodiumApi */

#ifndef _Included_libsodiumjni_internal_SodiumApi
#define _Included_libsodiumjni_internal_SodiumApi
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    sodium_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_sodium_1init
  (JNIEnv *, jclass);

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_public_key_bytes
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1public_1key_1bytes
  (JNIEnv *, jclass);

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_secret_key_bytes
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1secret_1key_1bytes
  (JNIEnv *, jclass);

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_keypair
 * Signature: ([B[B)I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1keypair
  (JNIEnv *, jclass, jbyteArray, jbyteArray);

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_seal_bytes
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal_1bytes
  (JNIEnv *, jclass);

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_seal
 * Signature: ([B[BJ[B)I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal
  (JNIEnv *, jclass, jbyteArray, jbyteArray, jlong, jbyteArray);

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_seal_open
 * Signature: ([B[BI[B[B)I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal_1open
  (JNIEnv *, jclass, jbyteArray, jbyteArray, jint, jbyteArray, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif