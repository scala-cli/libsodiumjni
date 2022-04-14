package libsodiumjni.internal;

public final class SodiumApi {

  public static native int sodium_init();

  public static native int crypto_box_public_key_bytes();
  public static native int crypto_box_secret_key_bytes();
  public static native int crypto_box_keypair(byte[] pubKey, byte[] secretKey);

  public static native int crypto_box_seal_bytes();
  public static native int crypto_box_seal(byte[] cipher, byte[] message, long messageLen, byte[] publicKey);

  public static native int crypto_box_seal_open(byte[] decrypted, byte[] cipher, int cipherLen, byte[] pubKey, byte[] secretKey);

}
