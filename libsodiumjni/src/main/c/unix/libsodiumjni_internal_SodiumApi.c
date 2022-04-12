#include <sodium.h>
#include "libsodiumjni_internal_SodiumApi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    sodium_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_sodium_1init
  (JNIEnv *env, jobject obj) {
    return sodium_init();
}

/*
 * Class:     libsodiumjni_internal_SodiumApi
 * Method:    crypto_box_seal
 * Signature: ([B[BJ[B)I
 */
JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal
  (JNIEnv *env, jobject obj, jbyteArray cipher, jbyteArray msg, jlong msg_len, jbyteArray pub_key) {

    jbyte *msg_bytes = (*env)->GetByteArrayElements(env, msg, 0);
    jbyte *pub_key_bytes = (*env)->GetByteArrayElements(env, pub_key, 0);
    int cipher_len = (*env)->GetArrayLength(env, cipher);
    jbyte *cipher_bytes = malloc(cipher_len);
    int res = crypto_box_seal(cipher_bytes, msg_bytes, msg_len, pub_key_bytes);
    (*env)->SetByteArrayRegion(env, cipher, 0, cipher_len, cipher_bytes);
    free(cipher_bytes);
    (*env)->ReleaseByteArrayElements(env, pub_key, pub_key_bytes, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, msg, msg_bytes, JNI_ABORT);
    return res;
}

#ifdef __cplusplus
}
#endif
