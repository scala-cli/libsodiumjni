package libsodiumjni;

import libsodiumjni.internal.SodiumApi;

public final class Sodium {

    private static SodiumApi api;
    private static boolean initialized = false;

    static {
        api = new SodiumApi();
    }

    public static void init() {
        if (!initialized) {
            synchronized (Sodium.class) {
                int res = api.sodium_init();
                if (res != 0) {
                    throw new RuntimeException("Cannot initialize libsodium");
                }
                initialized = true;
            }
        }
    }


    // from https://github.com/terl/lazysodium-java/blob/e3ca18a6334f2ede76516f86397672bee866ce5d/src/main/java/com/goterl/lazysodium/interfaces/Box.java#L22-L35
    private static final int CURVE25519XSALSA20POLY1305_PUBLICKEYBYTES = 32,
            CURVE25519XSALSA20POLY1305_MACBYTES = 16,
            PUBLICKEYBYTES = CURVE25519XSALSA20POLY1305_PUBLICKEYBYTES,
            MACBYTES = CURVE25519XSALSA20POLY1305_MACBYTES,
            SEALBYTES = PUBLICKEYBYTES + MACBYTES;


    public static byte[] seal(byte[] message, byte[] pubKey) {
        byte[] cipher = new byte[SEALBYTES + message.length];
        int res = api.crypto_box_seal(cipher, message, message.length, pubKey);
        if (res != 0) {
            throw new RuntimeException("Failed to seal message");
        }
        return cipher;
    }

}
