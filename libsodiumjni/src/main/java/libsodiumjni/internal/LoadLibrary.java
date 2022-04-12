package libsodiumjni.internal;

public final class LoadLibrary {

    private static boolean initialized = false;

    public static void initialize(String libraryPath) {
        if (!initialized) {
            synchronized (LoadLibrary.class) {
                System.load(libraryPath);
                initialized = true;
            }
        }

    }

    public static void assumeInitialized() {
        initialized = true;
    }

}
