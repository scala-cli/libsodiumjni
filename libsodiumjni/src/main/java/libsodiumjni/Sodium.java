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


    public static byte[] seal(byte[] message, byte[] pubKey) {
        byte[] cipher = new byte[api.crypto_box_seal_bytes() + message.length];
        int res = api.crypto_box_seal(cipher, message, message.length, pubKey);
        if (res != 0) {
            throw new RuntimeException("Failed to seal message");
        }
        return cipher;
    }

}
