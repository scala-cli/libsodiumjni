package libsodiumjni.internal;

public final class SodiumApi {

  public static native int sodium_init();

  public static native int crypto_box_seal_bytes();
  public static native int crypto_box_seal(byte[] cipher, byte[] message, long messageLen, byte[] publicKey);

}
