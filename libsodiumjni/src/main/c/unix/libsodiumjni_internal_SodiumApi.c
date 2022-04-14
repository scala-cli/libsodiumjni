#include <sodium.h>
#include "libsodiumjni_internal_SodiumApi.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_sodium_1init
  (JNIEnv *env, jclass cls) {
    return sodium_init();
}


JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1public_1key_1bytes
  (JNIEnv *env, jclass cls) {
    return crypto_box_PUBLICKEYBYTES;
}

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1secret_1key_1bytes
  (JNIEnv *env, jclass cls) {
    return crypto_box_SECRETKEYBYTES;
}

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1keypair
  (JNIEnv *env, jclass cls, jbyteArray pub_key, jbyteArray secret_key) {

    int provided_pub_key_len = (*env)->GetArrayLength(env, pub_key);
    int provided_sec_key_len = (*env)->GetArrayLength(env, secret_key);

    if (provided_pub_key_len < crypto_box_PUBLICKEYBYTES || provided_sec_key_len < crypto_box_SECRETKEYBYTES)
        return -10;

    unsigned char recipient_pk[crypto_box_PUBLICKEYBYTES];
    unsigned char recipient_sk[crypto_box_SECRETKEYBYTES];

    int res = crypto_box_keypair(recipient_pk, recipient_sk);

    if (res == 0) {
        (*env)->SetByteArrayRegion(env, pub_key, 0, crypto_box_PUBLICKEYBYTES, recipient_pk);
        (*env)->SetByteArrayRegion(env, secret_key, 0, crypto_box_SECRETKEYBYTES, recipient_sk);
    }

    return res;
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

JNIEXPORT jint JNICALL Java_libsodiumjni_internal_SodiumApi_crypto_1box_1seal_1open
  (JNIEnv *env, jclass cls, jbyteArray decrypted, jbyteArray cipher, jint cipherLen, jbyteArray pub_key, jbyteArray secret_key) {

    int msg_len = (*env)->GetArrayLength(env, decrypted);
    int cipher_len = (*env)->GetArrayLength(env, cipher);

    if (cipher_len < crypto_box_SEALBYTES + msg_len)
        return -10;

    jbyte *msg = malloc(msg_len);
    jbyte *cipher_bytes = (*env)->GetByteArrayElements(env, cipher, 0);
    jbyte *pub_key_bytes = (*env)->GetByteArrayElements(env, pub_key, 0);
    jbyte *sec_key_bytes = (*env)->GetByteArrayElements(env, secret_key, 0);

    int res = crypto_box_seal_open(msg, cipher_bytes, cipher_len, pub_key_bytes, sec_key_bytes);

    if (res == 0) {
        (*env)->SetByteArrayRegion(env, decrypted, 0, msg_len, msg);
    }

    (*env)->ReleaseByteArrayElements(env, secret_key, sec_key_bytes, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, pub_key, pub_key_bytes, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, cipher, cipher_bytes, JNI_ABORT);
    free(msg);

    return res;
}

#ifdef __cplusplus
}
#endif
