package com.mterm.lib;

public final class JNI {
    public static native int createMterm(
            String cmd,
            String cwd,
            String[] argv,
            String[] envp,
            int rows,
            int cols);

    public static native int createMtermDefault();

    public static native int destroyMterm(int id);

    public static native int readMterm(int id, byte[] buf, int size);

    public static native int writeMterm(int id, byte[] buf, int size);

    public static native int waitMterm(int id);

    public static native void setReadNonblockMterm(int id);

    public static native void setWindowSizeMterm(int id);

    public static native boolean checkRunningMterm(int id);

    static {
        System.loadLibrary("mterm");
    }

}