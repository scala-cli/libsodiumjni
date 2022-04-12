package libsodiumjni.internal;

public final class SodiumApi {

  public native int sodium_init();

  public native int crypto_box_seal(byte[] cipher, byte[] message, long messageLen, byte[] publicKey);

}
