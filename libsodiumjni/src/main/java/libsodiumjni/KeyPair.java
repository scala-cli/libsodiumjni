package libsodiumjni;

public final class KeyPair {

  private final byte[] pubKey;
  private final byte[] secKey;

  public KeyPair(byte[] pubKey, byte[] secKey) {
    this.pubKey = pubKey;
    this.secKey = secKey;
  }

  public byte[] getPubKey() {
    return pubKey;
  }
  public byte[] getSecKey() {
    return secKey;
  }
}