package dev1503.armmem.hook;

/* loaded from: classes.dex */
public class Hook {
    public static long getFunctionAddress(java.lang.String moduleName, java.lang.String functionName) {
        return dev1503.armmem.hook.JNI.getFunctionAddress(moduleName, functionName);
    }
}
