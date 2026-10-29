package de.robv.android.xposed;

public final class XposedHelpers {
    private XposedHelpers() {
    }

    public static java.lang.reflect.Method findMethodExact(Class<?> clazz, String methodName, Object... parameterTypes) {
        Class<?>[] types = new Class<?>[parameterTypes.length];
        for (int i = 0; i < parameterTypes.length; i++) {
            Object p = parameterTypes[i];
            types[i] = p instanceof Class<?> ? (Class<?>) p : (Class<?>) ((java.lang.reflect.Member) p).getDeclaringClass();
        }
        try {
            java.lang.reflect.Method m = clazz.getDeclaredMethod(methodName, types);
            m.setAccessible(true);
            return m;
        } catch (NoSuchMethodException e) {
            throw new NoSuchMethodError(methodName);
        }
    }

    public static void findAndHookMethod(String className, ClassLoader classLoader, String methodName, Object... parameterTypesAndCallback) {
        try {
            Class<?> clazz = Class.forName(className, false, classLoader);
            findAndHookMethod(clazz, methodName, parameterTypesAndCallback);
        } catch (ClassNotFoundException e) {
            throw new NoClassDefFoundError(className);
        }
    }

    public static void findAndHookMethod(Class<?> clazz, String methodName, Object... parameterTypesAndCallback) {
        if (parameterTypesAndCallback.length == 0 || !(parameterTypesAndCallback[parameterTypesAndCallback.length - 1] instanceof XC_MethodHook)) {
            throw new IllegalArgumentException("no callback defined");
        }
        XC_MethodHook callback = (XC_MethodHook) parameterTypesAndCallback[parameterTypesAndCallback.length - 1];
        Object[] types = new Object[parameterTypesAndCallback.length - 1];
        System.arraycopy(parameterTypesAndCallback, 0, types, 0, types.length);
        java.lang.reflect.Method m = findMethodExact(clazz, methodName, types);
        XposedBridge.hookMethod(m, callback);
    }

    public static Object getObjectField(Object obj, String fieldName) {
        try {
            java.lang.reflect.Field f = obj.getClass().getDeclaredField(fieldName);
            f.setAccessible(true);
            return f.get(obj);
        } catch (Exception e) {
            throw new NoSuchFieldError(fieldName);
        }
    }

    public static void setObjectField(Object obj, String fieldName, Object value) {
        try {
            java.lang.reflect.Field f = obj.getClass().getDeclaredField(fieldName);
            f.setAccessible(true);
            f.set(obj, value);
        } catch (Exception e) {
            throw new NoSuchFieldError(fieldName);
        }
    }

    public static Object callMethod(Object obj, String methodName, Object... args) {
        try {
            for (java.lang.reflect.Method m : obj.getClass().getDeclaredMethods()) {
                if (m.getName().equals(methodName) && m.getParameterCount() == args.length) {
                    m.setAccessible(true);
                    return m.invoke(obj, args);
                }
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        throw new NoSuchMethodError(methodName);
    }
}
