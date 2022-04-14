#include <sodium.h>
#include "libsodiumjni_internal_SodiumApi.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_sodium_1init
  (JNIEnv *env, jclass cls) {
    return sodium_init();
}

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal_1bytes
  (JNIEnv *env, jclass cls) {
    return crypto_box_SEALBYTES;
}

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal
  (JNIEnv *env, jclass cls, jbyteArray cipher, jbyteArray msg, jlong msg_len, jbyteArray pub_key) {

    int cipher_len = (*env)->GetArrayLength(env, cipher);

    if (cipher_len < crypto_box_SEALBYTES + msg_len)
        return -10;

    jbyte *msg_bytes = (*env)->GetByteArrayElements(env, msg, 0);
    jbyte *pub_key_bytes = (*env)->GetByteArrayElements(env, pub_key, 0);
    jbyte *cipher_bytes = malloc(cipher_len);

    int res = crypto_box_seal(cipher_bytes, msg_bytes, msg_len, pub_key_bytes);

    if (res == 0) {
        (*env)->SetByteArrayRegion(env, cipher, 0, cipher_len, cipher_bytes);
    }

    free(cipher_bytes);
    (*env)->ReleaseByteArrayElements(env, pub_key, pub_key_bytes, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, msg, msg_bytes, JNI_ABORT);
    return res;
}

#ifdef __cplusplus
}
#endif
