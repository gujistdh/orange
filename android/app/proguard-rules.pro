-keep class com.Orange.** { *; }
-keep class dev1503.armmem.** { *; }
-keepclasseswithmembernames class * {
    native <methods>;
}
-dontwarn de.robv.android.xposed.**
