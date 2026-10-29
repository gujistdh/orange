package dev1503.armmem.memory;

/* loaded from: classes.dex */
public class JNI {
    public static native void closeMemFile(int i);

    public static native int getPid();

    public static native int getPid(java.lang.String str);

    public static native int openMemFile(int i);

    public static native byte readByte(int i, long j);

    public static native byte readByte(long j);

    public static native byte readByte(long j, int i);

    public static native double readDouble(int i, long j);

    public static native double readDouble(long j);

    public static native double readDouble(long j, int i);

    public static native int readDword(int i, long j);

    public static native int readDword(long j);

    public static native int readDword(long j, int i);

    public static native float readFloat(int i, long j);

    public static native float readFloat(long j);

    public static native float readFloat(long j, int i);

    public static native long readQword(int i, long j);

    public static native long readQword(long j);

    public static native long readQword(long j, int i);

    public static native short readWord(int i, long j);

    public static native short readWord(long j);

    public static native short readWord(long j, int i);

    public static native long[] searchByte(byte b, int i);

    public static native long[] searchByte(byte b, long[] jArr);

    public static native long[] searchByte(int i, byte b, int i2);

    public static native long[] searchByte(int i, byte b, long[] jArr);

    public static native long[] searchDouble(double d, double d2, int i);

    public static native long[] searchDouble(double d, double d2, long[] jArr);

    public static native long[] searchDouble(int i, double d, double d2, int i2);

    public static native long[] searchDouble(int i, double d, double d2, long[] jArr);

    public static native long[] searchDword(int i, int i2);

    public static native long[] searchDword(int i, int i2, int i3);

    public static native long[] searchDword(int i, int i2, long[] jArr);

    public static native long[] searchDword(int i, long[] jArr);

    public static native long[] searchFloat(float f, float f2, int i);

    public static native long[] searchFloat(float f, float f2, long[] jArr);

    public static native long[] searchFloat(int i, float f, float f2, int i2);

    public static native long[] searchFloat(int i, float f, float f2, long[] jArr);

    public static native long[] searchQword(int i, long j, int i2);

    public static native long[] searchQword(int i, long j, long[] jArr);

    public static native long[] searchQword(long j, int i);

    public static native long[] searchQword(long j, long[] jArr);

    public static native long[] searchWord(int i, short s, int i2);

    public static native long[] searchWord(int i, short s, long[] jArr);

    public static native long[] searchWord(short s, int i);

    public static native long[] searchWord(short s, long[] jArr);

    public static native void writeByte(int i, long j, byte b);

    public static native void writeByte(long j, byte b);

    public static native void writeDouble(int i, long j, double d);

    public static native void writeDouble(long j, double d);

    public static native void writeDword(int i, long j, int i2);

    public static native void writeDword(long j, int i);

    public static native void writeFloat(int i, long j, float f);

    public static native void writeFloat(long j, float f);

    public static native void writeQword(int i, long j, long j2);

    public static native void writeQword(long j, long j2);

    public static native void writeWord(int i, long j, short s);

    public static native void writeWord(long j, short s);

    static {
        java.lang.System.loadLibrary("ArmMem");
    }
}
