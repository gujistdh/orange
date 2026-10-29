package com.Orange;

/* loaded from: classes3.dex */
public class HookEntry implements de.robv.android.xposed.IXposedHookLoadPackage {
    private static final java.lang.String MAIN_ACTIVITY_CLASS = "com.mojang.minecraftpe.MainActivity";
    private static final java.lang.String TARGET_PACKAGE = "com.netease.x19";
    private static boolean started = false;

    public void handleLoadPackage(de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam lpparam) throws java.lang.Throwable {
        if (!TARGET_PACKAGE.equals(lpparam.packageName)) {
            return;
        }
        de.robv.android.xposed.XposedBridge.log("[Orange] Target package matched: " + lpparam.packageName);
        de.robv.android.xposed.XposedHelpers.findAndHookMethod(android.app.Activity.class.getName(), lpparam.classLoader, "onCreate", new java.lang.Object[]{android.os.Bundle.class, new de.robv.android.xposed.XC_MethodHook() { // from class: com.Orange.HookEntry.1
            protected void beforeHookedMethod(de.robv.android.xposed.XC_MethodHook.MethodHookParam param) throws java.lang.Throwable {
                android.app.Activity activity = (android.app.Activity) param.thisObject;
                java.lang.String className = activity.getClass().getName();
                if (!com.Orange.HookEntry.MAIN_ACTIVITY_CLASS.equals(className)) {
                    return;
                }
                de.robv.android.xposed.XposedBridge.log("[Orange] MainActivity.onCreate hooked, starting init...");
                com.Orange.HookEntry.delayedInit(activity, 2800L);
            }
        }});
        de.robv.android.xposed.XposedHelpers.findAndHookMethod(android.app.Activity.class.getName(), lpparam.classLoader, "dispatchTouchEvent", new java.lang.Object[]{android.view.MotionEvent.class, new de.robv.android.xposed.XC_MethodHook() { // from class: com.Orange.HookEntry.2
            protected void beforeHookedMethod(de.robv.android.xposed.XC_MethodHook.MethodHookParam param) throws java.lang.Throwable {
                android.app.Activity activity = (android.app.Activity) param.thisObject;
                java.lang.String className = activity.getClass().getName();
                if (!com.Orange.HookEntry.MAIN_ACTIVITY_CLASS.equals(className) || !com.Orange.Loading.touchReady) {
                    return;
                }
                try {
                    android.view.MotionEvent ev = (android.view.MotionEvent) param.args[0];
                    if (com.Orange.Loading.nativeTouchEvent(ev.getActionMasked(), ev.getX(), ev.getY())) {
                        param.setResult(true);
                    }
                } catch (java.lang.Throwable th) {
                }
            }
        }});
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static void delayedInit(final android.app.Activity activity, long delayMillis) {
        if (started) {
            return;
        }
        started = true;
        new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(new java.lang.Runnable() { // from class: com.Orange.HookEntry.3
            @Override // java.lang.Runnable
            public void run() {
                try {
                    com.Orange.Loading.activity = activity;
                } catch (java.lang.Throwable th) {
                }
                try {
                    com.Orange.Loading.init(activity);
                } catch (java.lang.Throwable th2) {
                    android.widget.Toast.makeText(activity, "orange加载失败", 0).show();
                }
            }
        }, delayMillis);
    }
}
