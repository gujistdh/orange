package de.robv.android.xposed;

import java.lang.reflect.Member;
import java.util.HashMap;
import java.util.Map;

public final class XposedBridge {
    private static final Map<Member, XC_MethodHook> HOOKS = new HashMap<>();

    private XposedBridge() {
    }

    public static void log(String text) {
        android.util.Log.i("Xposed", text);
    }

    public static void log(Throwable t) {
        android.util.Log.e("Xposed", "Error", t);
    }

    public static XC_MethodHook.Unhook hookMethod(Member hookMethod, XC_MethodHook callback) {
        HOOKS.put(hookMethod, callback);
        return new XC_MethodHook.Unhook(hookMethod);
    }

    public static Map<Member, XC_MethodHook> getHooks() {
        return HOOKS;
    }
}
