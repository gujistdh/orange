package com.Orange;

/* loaded from: classes3.dex */
public class Loading {
    private static final java.lang.String CLICK_SOUND_PATH = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/sounds/click.mp3";
    public static android.app.Activity activity;
    public static volatile boolean touchReady = false;
    private static boolean isLibraryLoaded = false;
    private static boolean hooksStarted = false;
    private static android.media.SoundPool clickSoundPool = null;
    private static int clickSoundId = 0;
    private static volatile boolean clickSoundLoaded = false;
    private static android.app.AlertDialog g_kamiDialog = null;
    private static android.app.AlertDialog g_userDialog = null;

    public static native boolean nativeInitHooks(byte[] bArr);

    public static native void nativeKamiCancel();

    public static native void nativeKamiStartup();

    public static native void nativeKamiSubmit(java.lang.String str);

    public static native void nativeSetDensity(float f);

    public static native void nativeTextInputResult(java.lang.String str);

    public static native boolean nativeTouchEvent(int i, float f, float f2);

    public static native void nativeTpEditInput(int i, int i2, java.lang.String str);

    public static native void nativeUserNameCancel();

    public static native void nativeUserNameResult(java.lang.String str);

    public static void init(final android.app.Activity act) {
        com.Orange.NetTls.install();
        new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(new java.lang.Runnable() { // from class: com.Orange.Loading.1
            @Override // java.lang.Runnable
            public void run() {
                com.Orange.Loading.activity = act;
                com.Orange.Loading.ensureLibraryLoaded(act);
            }
        }, 1000L);
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static void ensureLibraryLoaded(final android.app.Activity act) {
        if (isLibraryLoaded) {
            return;
        }
        new java.lang.Thread(new java.lang.Runnable() { // from class: com.Orange.Loading.2
            @Override // java.lang.Runnable
            public void run() {
                com.Orange.Loading.loadNativeLibrary(act);
            }
        }).start();
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static void loadNativeLibrary(final android.app.Activity act) {
        try {
            java.lang.System.loadLibrary("Orange");
            isLibraryLoaded = true;
            onLibraryLoaded(act);
        } catch (java.lang.UnsatisfiedLinkError e) {
            java.lang.String firstError = java.lang.String.valueOf(e.getMessage());
            try {
                extractSoFromApkAndLoad(act);
                isLibraryLoaded = true;
                onLibraryLoaded(act);
            } catch (java.lang.Throwable e2) {
                final java.lang.String msg = "SO加载失败 " + firstError + " | " + e2;
                new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.3
                    @Override // java.lang.Runnable
                    public void run() {
                        com.Orange.Loading.showToastOnMain(act, msg);
                    }
                });
            }
        }
    }

    private static void extractSoFromApkAndLoad(android.app.Activity act) throws java.lang.Exception {
        java.lang.String apkPath = getModuleApkPath();
        if (apkPath == null || apkPath.isEmpty()) {
            throw new java.lang.Exception("Cannot get module APK path");
        }
        java.io.File apkFile = new java.io.File(apkPath);
        if (!apkFile.exists()) {
            throw new java.lang.Exception("APK file not found: " + apkPath);
        }
        java.io.File destDir = new java.io.File(act.getCacheDir(), "orange_native");
        if (!destDir.exists()) {
            destDir.mkdirs();
        }
        java.lang.String stamp = apkFile.length() + "_" + apkFile.lastModified();
        java.io.File markFile = new java.io.File(destDir, "build.txt");
        boolean fresh = markFile.exists() && stamp.equals(readSmall(markFile));
        java.lang.String[] names = {"libImGui.so", "liblsposed.so", "libOrange.so"};
        java.io.File[] dests = new java.io.File[names.length];
        for (int i = 0; i < names.length; i++) {
            java.io.File f = new java.io.File(destDir, names[i]);
            if (!fresh || !f.exists() || f.length() <= 0) {
                extractOne(apkPath, names[i], f);
            }
            dests[i] = f;
        }
        if (!fresh) {
            writeSmall(markFile, stamp);
        }
        java.lang.System.load(dests[0].getAbsolutePath());
        java.lang.System.load(dests[2].getAbsolutePath());
    }

    private static void extractOne(java.lang.String apkPath, java.lang.String soName, java.io.File dest) throws java.lang.Exception {
        java.util.zip.ZipFile zipFile = new java.util.zip.ZipFile(apkPath);
        java.util.zip.ZipEntry target = null;
        try {
            java.util.Enumeration<? extends java.util.zip.ZipEntry> entries = zipFile.entries();
            while (true) {
                if (!entries.hasMoreElements()) {
                    break;
                }
                java.util.zip.ZipEntry e = entries.nextElement();
                if (e.getName().endsWith("/" + soName)) {
                    target = e;
                    break;
                }
            }
            if (target == null) {
                throw new java.lang.Exception("APK 中未找到 " + soName);
            }
            java.io.InputStream in = zipFile.getInputStream(target);
            java.io.FileOutputStream out = new java.io.FileOutputStream(dest);
            byte[] buffer = new byte[8192];
            while (true) {
                int len = in.read(buffer);
                if (len != -1) {
                    out.write(buffer, 0, len);
                } else {
                    out.close();
                    in.close();
                    dest.setExecutable(true);
                    dest.setReadable(true);
                    return;
                }
            }
        } finally {
            zipFile.close();
        }
    }

    private static java.lang.String readSmall(java.io.File f) {
        try {
            byte[] buf = new byte[128];
            java.io.FileInputStream in = new java.io.FileInputStream(f);
            int n = in.read(buf);
            in.close();
            if (n <= 0) {
                return "";
            }
            return new java.lang.String(buf, 0, n).trim();
        } catch (java.lang.Throwable th) {
            return "";
        }
    }

    private static void writeSmall(java.io.File f, java.lang.String text) {
        try {
            java.io.FileOutputStream out = new java.io.FileOutputStream(f);
            out.write(text.getBytes("UTF-8"));
            out.close();
        } catch (java.lang.Throwable th) {
        }
    }

    private static java.lang.String getModuleApkPath() {
        try {
            return com.Orange.Loading.class.getProtectionDomain().getCodeSource().getLocation().getPath();
        } catch (java.lang.Throwable th) {
            try {
                return com.Orange.HookEntry.class.getProtectionDomain().getCodeSource().getLocation().getPath();
            } catch (java.lang.Throwable th2) {
                return null;
            }
        }
    }

    private static void onLibraryLoaded(android.app.Activity act) {
        try {
            float density = act.getResources().getDisplayMetrics().density;
            nativeSetDensity(density);
        } catch (java.lang.Throwable th) {
        }
        try {
            nativeKamiStartup();
        } catch (java.lang.Throwable th2) {
        }
        startHooksOnce(act);
        touchReady = true;
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static void startHooksOnce(android.app.Activity act) {
        if (act == null || hooksStarted) {
            return;
        }
        hooksStarted = true;
        try {
            initHooksNow(act);
        } catch (java.lang.Throwable th) {
            hooksStarted = false;
        }
    }

    private static void initHooksNow(final android.app.Activity act) {
        new java.lang.Thread(new java.lang.Runnable() { // from class: com.Orange.Loading.4
            @Override // java.lang.Runnable
            public void run() {
                boolean ok = false;
                boolean linkBad = false;
                for (int attempt = 0; attempt < 10; attempt++) {
                    try {
                        boolean ok2 = com.Orange.Loading.nativeInitHooks(null);
                        ok = ok2;
                    } catch (java.lang.UnsatisfiedLinkError e) {
                        linkBad = true;
                    } catch (java.lang.Throwable th) {
                        ok = false;
                    }
                    if (ok) {
                        break;
                    }
                    try {
                        java.lang.Thread.sleep(700L);
                    } catch (java.lang.InterruptedException e2) {
                    }
                }
                if (linkBad) {
                    com.Orange.Loading.hooksStarted = false;
                    com.Orange.Loading.showToastOnMain(act, "Orange:SO版本过旧");
                } else if (ok) {
                    com.Orange.Loading.showToastOnMain(act, "Orange加载成功");
                } else {
                    com.Orange.Loading.hooksStarted = false;
                }
            }
        }).start();
    }

    public static void playClickSound() {
        android.app.Activity act = activity;
        if (act == null) {
            return;
        }
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.5
            @Override // java.lang.Runnable
            public void run() {
                try {
                    if (com.Orange.Loading.clickSoundPool == null) {
                        com.Orange.Loading.clickSoundPool = new android.media.SoundPool(2, 3, 0);
                        com.Orange.Loading.clickSoundPool.setOnLoadCompleteListener(new android.media.SoundPool.OnLoadCompleteListener() { // from class: com.Orange.Loading.5.1
                            @Override // android.media.SoundPool.OnLoadCompleteListener
                            public void onLoadComplete(android.media.SoundPool sp, int sampleId, int status) {
                                if (status == 0) {
                                    com.Orange.Loading.clickSoundLoaded = true;
                                }
                            }
                        });
                        com.Orange.Loading.clickSoundId = com.Orange.Loading.clickSoundPool.load(com.Orange.Loading.CLICK_SOUND_PATH, 1);
                    }
                    if (com.Orange.Loading.clickSoundLoaded && com.Orange.Loading.clickSoundId != 0) {
                        java.io.File f = new java.io.File(com.Orange.Loading.CLICK_SOUND_PATH);
                        if (f.exists()) {
                            com.Orange.Loading.clickSoundPool.play(com.Orange.Loading.clickSoundId, 1.0f, 1.0f, 1, 0, 1.0f);
                        }
                    }
                } catch (java.lang.Throwable th) {
                }
            }
        });
    }

    public static void showTextInputDialog(final java.lang.String title, final java.lang.String current) {
        final android.app.Activity act = activity;
        if (act == null) {
            return;
        }
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.6
            @Override // java.lang.Runnable
            public void run() {
                try {
                    android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(act);
                    builder.setTitle(title != null ? title : "Input");
                    final android.widget.EditText input = new android.widget.EditText(act);
                    input.setText(current != null ? current : "");
                    input.setSelection(input.getText().length());
                    input.setTextSize(16.0f);
                    android.view.ViewGroup.LayoutParams lp = new android.view.ViewGroup.LayoutParams(-1, -2);
                    input.setLayoutParams(lp);
                    builder.setView(input);
                    builder.setPositiveButton("OK", new android.content.DialogInterface.OnClickListener() { // from class: com.Orange.Loading.6.1
                        @Override // android.content.DialogInterface.OnClickListener
                        public void onClick(android.content.DialogInterface dialog, int which) {
                            try {
                                com.Orange.Loading.nativeTextInputResult(input.getText().toString());
                            } catch (java.lang.Throwable th) {
                            }
                        }
                    });
                    builder.setNegativeButton("Cancel", new android.content.DialogInterface.OnClickListener() { // from class: com.Orange.Loading.6.2
                        @Override // android.content.DialogInterface.OnClickListener
                        public void onClick(android.content.DialogInterface dialog, int which) {
                            try {
                                com.Orange.Loading.nativeTextInputResult("");
                            } catch (java.lang.Throwable th) {
                            }
                        }
                    });
                    builder.setCancelable(false);
                    builder.show();
                } catch (java.lang.Throwable th) {
                }
            }
        });
    }

    public static void showTpEditDialog(final int row, final int field, final java.lang.String current) {
        final android.app.Activity act = activity;
        if (act == null) {
            return;
        }
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.7
            @Override // java.lang.Runnable
            public void run() {
                java.lang.String title;
                try {
                    android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(act);
                    if (field == 0) {
                        title = "传送点名称";
                    } else if (field == 1) {
                        title = "X 坐标";
                    } else if (field == 2) {
                        title = "Y 坐标";
                    } else {
                        title = "Z 坐标";
                    }
                    builder.setTitle(title);
                    final android.widget.EditText input = new android.widget.EditText(act);
                    input.setText(current != null ? current : "");
                    input.setSelection(input.getText().length());
                    input.setTextSize(16.0f);
                    if (field >= 1 && field <= 3) {
                        input.setInputType(12290);
                    }
                    android.view.ViewGroup.LayoutParams lp = new android.view.ViewGroup.LayoutParams(-1, -2);
                    input.setLayoutParams(lp);
                    builder.setView(input);
                    builder.setPositiveButton("确定", new android.content.DialogInterface.OnClickListener() { // from class: com.Orange.Loading.7.1
                        @Override // android.content.DialogInterface.OnClickListener
                        public void onClick(android.content.DialogInterface dialog, int which) {
                            try {
                                com.Orange.Loading.nativeTpEditInput(row, field, input.getText().toString());
                            } catch (java.lang.Throwable th) {
                            }
                        }
                    });
                    builder.setNegativeButton("取消", new android.content.DialogInterface.OnClickListener() { // from class: com.Orange.Loading.7.2
                        @Override // android.content.DialogInterface.OnClickListener
                        public void onClick(android.content.DialogInterface dialog, int which) {
                            try {
                                com.Orange.Loading.nativeTpEditInput(row, field, current);
                            } catch (java.lang.Throwable th) {
                            }
                        }
                    });
                    builder.setCancelable(false);
                    builder.show();
                } catch (java.lang.Throwable th) {
                }
            }
        });
    }

    public static void showKamiDialog() {
        final android.app.Activity act = activity;
        if (act == null) {
            return;
        }
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.8
            @Override // java.lang.Runnable
            public void run() {
                try {
                    if (com.Orange.Loading.g_kamiDialog == null || !com.Orange.Loading.g_kamiDialog.isShowing()) {
                        float d = act.getResources().getDisplayMetrics().density;
                        android.graphics.drawable.GradientDrawable bg = new android.graphics.drawable.GradientDrawable();
                        bg.setOrientation(android.graphics.drawable.GradientDrawable.Orientation.TL_BR);
                        bg.setColors(new int[]{-1, -1510917});
                        bg.setCornerRadius(30.0f * d);
                        android.widget.LinearLayout linearLayout = new android.widget.LinearLayout(act);
                        linearLayout.setOrientation(1);
                        int pad = (int) (24.0f * d);
                        linearLayout.setPadding(pad, pad, pad, pad);
                        linearLayout.setBackground(bg);
                        final android.widget.TextView title = new android.widget.TextView(act);
                        title.setText("Orange");
                        title.setTextSize(26.0f);
                        title.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
                        title.setGravity(17);
                        title.getViewTreeObserver().addOnGlobalLayoutListener(new android.view.ViewTreeObserver.OnGlobalLayoutListener() { // from class: com.Orange.Loading.8.1
                            @Override // android.view.ViewTreeObserver.OnGlobalLayoutListener
                            public void onGlobalLayout() {
                                try {
                                    int w = title.getWidth();
                                    if (w > 0) {
                                        android.graphics.LinearGradient shader = new android.graphics.LinearGradient(0.0f, 0.0f, w, 0.0f, new int[]{-10831624, -15374912}, (float[]) null, android.graphics.Shader.TileMode.CLAMP);
                                        title.getPaint().setShader(shader);
                                        title.invalidate();
                                        title.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                                    }
                                } catch (java.lang.Throwable th) {
                                }
                            }
                        });
                        linearLayout.addView(title, new android.widget.LinearLayout.LayoutParams(-1, -2));
                        final android.widget.EditText input = new android.widget.EditText(act);
                        input.setHint("卡密");
                        input.setText("");
                        input.setTextSize(15.0f);
                        input.setSingleLine(true);
                        android.graphics.drawable.GradientDrawable inputBg = new android.graphics.drawable.GradientDrawable();
                        inputBg.setColor(-920069);
                        inputBg.setCornerRadius(d * 14.0f);
                        input.setBackground(inputBg);
                        int inputPad = (int) (10.0f * d);
                        input.setPadding(inputPad, inputPad, inputPad, inputPad);
                        android.widget.LinearLayout.LayoutParams inputLp = new android.widget.LinearLayout.LayoutParams(-1, -2);
                        inputLp.topMargin = (int) (18.0f * d);
                        linearLayout.addView(input, inputLp);
                        android.widget.LinearLayout linearLayout2 = new android.widget.LinearLayout(act);
                        linearLayout2.setOrientation(0);
                        android.widget.Button btnCancel = new android.widget.Button(act);
                        btnCancel.setText("取消");
                        btnCancel.setTextSize(15.0f);
                        btnCancel.setAllCaps(false);
                        btnCancel.setTextColor(-11184811);
                        android.graphics.drawable.GradientDrawable cancelBg = new android.graphics.drawable.GradientDrawable();
                        cancelBg.setColor(-1774862);
                        cancelBg.setCornerRadius(d * 14.0f);
                        btnCancel.setBackground(cancelBg);
                        android.widget.Button btnLogin = new android.widget.Button(act);
                        btnLogin.setText("登录");
                        btnLogin.setTextSize(15.0f);
                        btnLogin.setAllCaps(false);
                        btnLogin.setTextColor(-1);
                        btnLogin.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
                        android.graphics.drawable.GradientDrawable loginBg = new android.graphics.drawable.GradientDrawable();
                        loginBg.setOrientation(android.graphics.drawable.GradientDrawable.Orientation.TL_BR);
                        loginBg.setColors(new int[]{-11556363, -15374912});
                        loginBg.setCornerRadius(d * 14.0f);
                        btnLogin.setBackground(loginBg);
                        android.widget.LinearLayout.LayoutParams cancelLp = new android.widget.LinearLayout.LayoutParams(0, -2, 1.0f);
                        android.widget.LinearLayout.LayoutParams loginLp = new android.widget.LinearLayout.LayoutParams(0, -2, 1.0f);
                        loginLp.leftMargin = (int) (12.0f * d);
                        linearLayout2.addView(btnCancel, cancelLp);
                        linearLayout2.addView(btnLogin, loginLp);
                        android.widget.LinearLayout.LayoutParams rowLp = new android.widget.LinearLayout.LayoutParams(-1, -2);
                        rowLp.topMargin = (int) (20.0f * d);
                        linearLayout.addView(linearLayout2, rowLp);
                        final android.app.AlertDialog create = new android.app.AlertDialog.Builder(act).create();
                        create.setView(linearLayout);
                        create.setCancelable(false);
                        btnCancel.setOnClickListener(new android.view.View.OnClickListener() { // from class: com.Orange.Loading.8.2
                            @Override // android.view.View.OnClickListener
                            public void onClick(android.view.View v) {
                                try {
                                    com.Orange.Loading.nativeKamiCancel();
                                } catch (java.lang.Throwable th) {
                                }
                                try {
                                    create.dismiss();
                                } catch (java.lang.Throwable th2) {
                                }
                            }
                        });
                        btnLogin.setOnClickListener(new android.view.View.OnClickListener() { // from class: com.Orange.Loading.8.3
                            @Override // android.view.View.OnClickListener
                            public void onClick(android.view.View v) {
                                java.lang.String kami = input.getText().toString().trim();
                                if (kami.isEmpty()) {
                                    android.widget.Toast.makeText(act, "请输入卡密", 0).show();
                                } else {
                                    try {
                                        com.Orange.Loading.nativeKamiSubmit(kami);
                                    } catch (java.lang.Throwable th) {
                                    }
                                }
                            }
                        });
                        create.show();
                        try {
                            try {
                                create.getWindow().setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(0));
                            } catch (java.lang.Throwable th) {
                            }
                        } catch (java.lang.Throwable th2) {
                        }
                        com.Orange.Loading.g_kamiDialog = create;
                    }
                } catch (java.lang.Throwable th3) {
                }
            }
        });
    }

    public static void showUserDialog(final java.lang.String preset) {
        final android.app.Activity act = activity;
        if (act == null) {
            return;
        }
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.9
            @Override // java.lang.Runnable
            public void run() {
                try {
                    if (com.Orange.Loading.g_userDialog == null || !com.Orange.Loading.g_userDialog.isShowing()) {
                        float d = act.getResources().getDisplayMetrics().density;
                        android.graphics.drawable.GradientDrawable bg = new android.graphics.drawable.GradientDrawable();
                        bg.setOrientation(android.graphics.drawable.GradientDrawable.Orientation.TL_BR);
                        bg.setColors(new int[]{-1, -1510917});
                        bg.setCornerRadius(30.0f * d);
                        android.widget.LinearLayout linearLayout = new android.widget.LinearLayout(act);
                        linearLayout.setOrientation(1);
                        int pad = (int) (24.0f * d);
                        linearLayout.setPadding(pad, pad, pad, pad);
                        linearLayout.setBackground(bg);
                        final android.widget.TextView title = new android.widget.TextView(act);
                        title.setText("设置用户");
                        title.setTextSize(26.0f);
                        title.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
                        title.setGravity(17);
                        title.getViewTreeObserver().addOnGlobalLayoutListener(new android.view.ViewTreeObserver.OnGlobalLayoutListener() { // from class: com.Orange.Loading.9.1
                            @Override // android.view.ViewTreeObserver.OnGlobalLayoutListener
                            public void onGlobalLayout() {
                                try {
                                    int w = title.getWidth();
                                    if (w > 0) {
                                        android.graphics.LinearGradient shader = new android.graphics.LinearGradient(0.0f, 0.0f, w, 0.0f, new int[]{-10831624, -15374912}, (float[]) null, android.graphics.Shader.TileMode.CLAMP);
                                        title.getPaint().setShader(shader);
                                        title.invalidate();
                                        title.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                                    }
                                } catch (java.lang.Throwable th) {
                                }
                            }
                        });
                        linearLayout.addView(title, new android.widget.LinearLayout.LayoutParams(-1, -2));
                        final android.widget.EditText input = new android.widget.EditText(act);
                        input.setHint("用户");
                        input.setText(preset == null ? "" : preset);
                        input.setTextSize(15.0f);
                        input.setSingleLine(true);
                        android.graphics.drawable.GradientDrawable inputBg = new android.graphics.drawable.GradientDrawable();
                        inputBg.setColor(-920069);
                        inputBg.setCornerRadius(d * 14.0f);
                        input.setBackground(inputBg);
                        int inputPad = (int) (10.0f * d);
                        input.setPadding(inputPad, inputPad, inputPad, inputPad);
                        android.widget.LinearLayout.LayoutParams inputLp = new android.widget.LinearLayout.LayoutParams(-1, -2);
                        inputLp.topMargin = (int) (18.0f * d);
                        linearLayout.addView(input, inputLp);
                        android.widget.LinearLayout linearLayout2 = new android.widget.LinearLayout(act);
                        linearLayout2.setOrientation(0);
                        android.widget.Button btnCancel = new android.widget.Button(act);
                        btnCancel.setText("取消");
                        btnCancel.setTextSize(15.0f);
                        btnCancel.setAllCaps(false);
                        btnCancel.setTextColor(-11184811);
                        android.graphics.drawable.GradientDrawable cancelBg = new android.graphics.drawable.GradientDrawable();
                        cancelBg.setColor(-1774862);
                        cancelBg.setCornerRadius(d * 14.0f);
                        btnCancel.setBackground(cancelBg);
                        android.widget.Button btnOk = new android.widget.Button(act);
                        btnOk.setText("确定");
                        btnOk.setTextSize(15.0f);
                        btnOk.setAllCaps(false);
                        btnOk.setTextColor(-1);
                        btnOk.setTypeface(android.graphics.Typeface.DEFAULT_BOLD);
                        android.graphics.drawable.GradientDrawable okBg = new android.graphics.drawable.GradientDrawable();
                        okBg.setOrientation(android.graphics.drawable.GradientDrawable.Orientation.TL_BR);
                        okBg.setColors(new int[]{-11556363, -15374912});
                        okBg.setCornerRadius(d * 14.0f);
                        btnOk.setBackground(okBg);
                        android.widget.LinearLayout.LayoutParams cancelLp = new android.widget.LinearLayout.LayoutParams(0, -2, 1.0f);
                        android.widget.LinearLayout.LayoutParams okLp = new android.widget.LinearLayout.LayoutParams(0, -2, 1.0f);
                        okLp.leftMargin = (int) (12.0f * d);
                        linearLayout2.addView(btnCancel, cancelLp);
                        linearLayout2.addView(btnOk, okLp);
                        android.widget.LinearLayout.LayoutParams rowLp = new android.widget.LinearLayout.LayoutParams(-1, -2);
                        rowLp.topMargin = (int) (20.0f * d);
                        linearLayout.addView(linearLayout2, rowLp);
                        final android.app.AlertDialog create = new android.app.AlertDialog.Builder(act).create();
                        create.setView(linearLayout);
                        create.setCancelable(false);
                        btnCancel.setOnClickListener(new android.view.View.OnClickListener() { // from class: com.Orange.Loading.9.2
                            @Override // android.view.View.OnClickListener
                            public void onClick(android.view.View v) {
                                try {
                                    com.Orange.Loading.nativeUserNameCancel();
                                } catch (java.lang.Throwable th) {
                                }
                                try {
                                    create.dismiss();
                                } catch (java.lang.Throwable th2) {
                                }
                            }
                        });
                        btnOk.setOnClickListener(new android.view.View.OnClickListener() { // from class: com.Orange.Loading.9.3
                            @Override // android.view.View.OnClickListener
                            public void onClick(android.view.View v) {
                                java.lang.String name = input.getText().toString().trim();
                                if (name.isEmpty()) {
                                    android.widget.Toast.makeText(act, "请输入用户名称", 0).show();
                                    return;
                                }
                                try {
                                    com.Orange.Loading.nativeUserNameResult(name);
                                } catch (java.lang.Throwable th) {
                                }
                                try {
                                    create.dismiss();
                                } catch (java.lang.Throwable th2) {
                                }
                            }
                        });
                        create.show();
                        try {
                            try {
                                create.getWindow().setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(0));
                            } catch (java.lang.Throwable th) {
                            }
                        } catch (java.lang.Throwable th2) {
                        }
                        com.Orange.Loading.g_userDialog = create;
                    }
                } catch (java.lang.Throwable th3) {
                }
            }
        });
    }

    public static void onKamiAuthResult(final boolean success, final java.lang.String msg) {
        final android.app.Activity act = activity;
        if (act == null) {
            return;
        }
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.10
            @Override // java.lang.Runnable
            public void run() {
                java.lang.String text;
                if (success) {
                    try {
                        if (com.Orange.Loading.g_kamiDialog != null && com.Orange.Loading.g_kamiDialog.isShowing()) {
                            com.Orange.Loading.g_kamiDialog.dismiss();
                        }
                    } catch (java.lang.Throwable th) {
                    }
                    com.Orange.Loading.startHooksOnce(act);
                    return;
                }
                try {
                    if (msg != null && !msg.isEmpty()) {
                        text = msg;
                        android.widget.Toast.makeText(act, text, 0).show();
                    }
                    text = "Orange - API - 对接失败 请重新验证 - 2.0.6";
                    android.widget.Toast.makeText(act, text, 0).show();
                } catch (java.lang.Throwable th2) {
                }
            }
        });
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static void showToastOnMain(final android.app.Activity act, final java.lang.String msg) {
        if (act == null) {
            return;
        }
        try {
            new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.11
                @Override // java.lang.Runnable
                public void run() {
                    android.widget.Toast.makeText(act, msg, 0).show();
                }
            });
        } catch (java.lang.Throwable th) {
        }
    }

    public static void showToast(java.lang.String msg) {
        if (msg == null || msg.isEmpty()) {
            return;
        }
        showToastOnMain(activity, msg);
    }

    public static void captureScreenshotToFile(final java.lang.String absolutePath) {
        final android.app.Activity act = activity;
        if (act == null || absolutePath == null) {
            return;
        }
        new java.lang.Thread(new java.lang.Runnable() { // from class: com.Orange.Loading.12
            @Override // java.lang.Runnable
            public void run() {
                try {
                    java.io.File parent = new java.io.File(absolutePath).getParentFile();
                    if (parent != null && !parent.exists()) {
                        parent.mkdirs();
                    }
                    android.graphics.Bitmap bitmap = com.Orange.Loading.captureSurfaceOnMain(act);
                    if (bitmap == null) {
                        return;
                    }
                    java.io.FileOutputStream fos = new java.io.FileOutputStream(absolutePath);
                    bitmap.compress(android.graphics.Bitmap.CompressFormat.PNG, 100, fos);
                    fos.flush();
                    fos.close();
                    bitmap.recycle();
                } catch (java.lang.Throwable th) {
                }
            }
        }).start();
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static android.graphics.Bitmap captureSurfaceOnMain(final android.app.Activity act) {
        final android.graphics.Bitmap[] out = new android.graphics.Bitmap[1];
        final java.lang.Throwable[] err = new java.lang.Throwable[1];
        final java.lang.Object lock = new java.lang.Object();
        new android.os.Handler(android.os.Looper.getMainLooper()).post(new java.lang.Runnable() { // from class: com.Orange.Loading.13
            /* JADX WARN: Removed duplicated region for block: B:54:0x00b9 A[EXC_TOP_SPLITTER, SYNTHETIC] */
            @Override // java.lang.Runnable
            /*
                Code decompiled incorrectly, please refer to instructions dump.
                To view partially-correct add '--show-bad-code' argument
            */
            public void run() {
                /*
                    r12 = this;
                    r0 = 0
                    android.app.Activity r1 = r1     // Catch: java.lang.Throwable -> Lb1
                    android.view.View r1 = com.Orange.Loading.m58$$Nest$smfindSurfaceView(r1)     // Catch: java.lang.Throwable -> Lb1
                    if (r1 != 0) goto L14
                    android.app.Activity r2 = r1     // Catch: java.lang.Throwable -> Lb1
                    android.view.Window r2 = r2.getWindow()     // Catch: java.lang.Throwable -> Lb1
                    android.view.View r2 = r2.getDecorView()     // Catch: java.lang.Throwable -> Lb1
                    r1 = r2
                L14:
                    int r2 = r1.getWidth()     // Catch: java.lang.Throwable -> Lb1
                    int r3 = r1.getHeight()     // Catch: java.lang.Throwable -> Lb1
                    if (r2 <= 0) goto L20
                    if (r3 > 0) goto L3a
                L20:
                    android.app.Activity r4 = r1     // Catch: java.lang.Throwable -> Lb1
                    android.content.res.Resources r4 = r4.getResources()     // Catch: java.lang.Throwable -> Lb1
                    android.util.DisplayMetrics r4 = r4.getDisplayMetrics()     // Catch: java.lang.Throwable -> Lb1
                    int r4 = r4.widthPixels     // Catch: java.lang.Throwable -> Lb1
                    r2 = r4
                    android.app.Activity r4 = r1     // Catch: java.lang.Throwable -> Lb1
                    android.content.res.Resources r4 = r4.getResources()     // Catch: java.lang.Throwable -> Lb1
                    android.util.DisplayMetrics r4 = r4.getDisplayMetrics()     // Catch: java.lang.Throwable -> Lb1
                    int r4 = r4.heightPixels     // Catch: java.lang.Throwable -> Lb1
                    r3 = r4
                L3a:
                    android.graphics.Bitmap$Config r4 = android.graphics.Bitmap.Config.ARGB_8888     // Catch: java.lang.Throwable -> Lb1
                    android.graphics.Bitmap r4 = android.graphics.Bitmap.createBitmap(r2, r3, r4)     // Catch: java.lang.Throwable -> Lb1
                    boolean r5 = r1 instanceof android.view.SurfaceView     // Catch: java.lang.Throwable -> Lb1
                    r6 = 1500(0x5dc, double:7.41E-321)
                    if (r5 == 0) goto L7a
                    r5 = r4
                    r8 = r1
                    android.view.SurfaceView r8 = (android.view.SurfaceView) r8     // Catch: java.lang.Throwable -> Lb1
                    com.Orange.Loading$13$1 r9 = new com.Orange.Loading$13$1     // Catch: java.lang.Throwable -> Lb1
                    r9.<init>()     // Catch: java.lang.Throwable -> Lb1
                    android.os.Handler r10 = new android.os.Handler     // Catch: java.lang.Throwable -> Lb1
                    android.os.Looper r11 = android.os.Looper.getMainLooper()     // Catch: java.lang.Throwable -> Lb1
                    r10.<init>(r11)     // Catch: java.lang.Throwable -> Lb1
                    android.view.PixelCopy.request(r8, r5, r9, r10)     // Catch: java.lang.Throwable -> Lb1
                    java.lang.Object r9 = r2     // Catch: java.lang.Throwable -> Lb1
                    monitor-enter(r9)     // Catch: java.lang.Throwable -> Lb1
                    android.graphics.Bitmap[] r10 = r3     // Catch: java.lang.Throwable -> L77
                    r10 = r10[r0]     // Catch: java.lang.Throwable -> L77
                    if (r10 != 0) goto L6b
                    java.lang.Object r10 = r2     // Catch: java.lang.InterruptedException -> L6a java.lang.Throwable -> L77
                    r10.wait(r6)     // Catch: java.lang.InterruptedException -> L6a java.lang.Throwable -> L77
                    goto L6b
                L6a:
                    r6 = move-exception
                L6b:
                    monitor-exit(r9)     // Catch: java.lang.Throwable -> L77
                    android.graphics.Bitmap[] r6 = r3     // Catch: java.lang.Throwable -> Lb1
                    r6 = r6[r0]     // Catch: java.lang.Throwable -> Lb1
                    if (r6 != 0) goto L76
                    android.graphics.Bitmap[] r6 = r3     // Catch: java.lang.Throwable -> Lb1
                    r6[r0] = r5     // Catch: java.lang.Throwable -> Lb1
                L76:
                    goto Lad
                L77:
                    r6 = move-exception
                    monitor-exit(r9)     // Catch: java.lang.Throwable -> L77
                    throw r6     // Catch: java.lang.Throwable -> Lb1
                L7a:
                    android.app.Activity r5 = r1     // Catch: java.lang.Throwable -> Lb1
                    android.view.Window r5 = r5.getWindow()     // Catch: java.lang.Throwable -> Lb1
                    r8 = r4
                    com.Orange.Loading$13$2 r9 = new com.Orange.Loading$13$2     // Catch: java.lang.Throwable -> Lb1
                    r9.<init>()     // Catch: java.lang.Throwable -> Lb1
                    android.os.Handler r10 = new android.os.Handler     // Catch: java.lang.Throwable -> Lb1
                    android.os.Looper r11 = android.os.Looper.getMainLooper()     // Catch: java.lang.Throwable -> Lb1
                    r10.<init>(r11)     // Catch: java.lang.Throwable -> Lb1
                    android.view.PixelCopy.request(r5, r8, r9, r10)     // Catch: java.lang.Throwable -> Lb1
                    java.lang.Object r9 = r2     // Catch: java.lang.Throwable -> Lb1
                    monitor-enter(r9)     // Catch: java.lang.Throwable -> Lb1
                    android.graphics.Bitmap[] r10 = r3     // Catch: java.lang.Throwable -> Lae
                    r10 = r10[r0]     // Catch: java.lang.Throwable -> Lae
                    if (r10 != 0) goto La2
                    java.lang.Object r10 = r2     // Catch: java.lang.InterruptedException -> La1 java.lang.Throwable -> Lae
                    r10.wait(r6)     // Catch: java.lang.InterruptedException -> La1 java.lang.Throwable -> Lae
                    goto La2
                La1:
                    r6 = move-exception
                La2:
                    monitor-exit(r9)     // Catch: java.lang.Throwable -> Lae
                    android.graphics.Bitmap[] r6 = r3     // Catch: java.lang.Throwable -> Lb1
                    r6 = r6[r0]     // Catch: java.lang.Throwable -> Lb1
                    if (r6 != 0) goto Lad
                    android.graphics.Bitmap[] r6 = r3     // Catch: java.lang.Throwable -> Lb1
                    r6[r0] = r8     // Catch: java.lang.Throwable -> Lb1
                Lad:
                    goto Lbf
                Lae:
                    r6 = move-exception
                    monitor-exit(r9)     // Catch: java.lang.Throwable -> Lae
                    throw r6     // Catch: java.lang.Throwable -> Lb1
                Lb1:
                    r1 = move-exception
                    java.lang.Throwable[] r2 = r4
                    r2[r0] = r1
                    java.lang.Object r0 = r2
                    monitor-enter(r0)
                    java.lang.Object r2 = r2     // Catch: java.lang.Throwable -> Lc0
                    r2.notifyAll()     // Catch: java.lang.Throwable -> Lc0
                    monitor-exit(r0)     // Catch: java.lang.Throwable -> Lc0
                Lbf:
                    return
                Lc0:
                    r2 = move-exception
                    monitor-exit(r0)     // Catch: java.lang.Throwable -> Lc0
                    throw r2
                */
                throw new UnsupportedOperationException("Method not decompiled: com.Orange.Loading.AnonymousClass13.run():void");
            }
        });
        synchronized (lock) {
            if (out[0] == null && err[0] == null) {
                try {
                    lock.wait(2000L);
                } catch (java.lang.InterruptedException e) {
                }
            }
        }
        return out[0];
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static android.view.View findSurfaceView(android.app.Activity act) {
        try {
            android.view.View root = act.getWindow().getDecorView();
            return findSurfaceViewRecursive(root);
        } catch (java.lang.Throwable th) {
            return null;
        }
    }

    private static android.view.View findSurfaceViewRecursive(android.view.View v) {
        if (v == null) {
            return null;
        }
        if (v instanceof android.view.SurfaceView) {
            return v;
        }
        if (v instanceof android.view.ViewGroup) {
            android.view.ViewGroup vg = (android.view.ViewGroup) v;
            for (int i = 0; i < vg.getChildCount(); i++) {
                android.view.View c = vg.getChildAt(i);
                android.view.View r = findSurfaceViewRecursive(c);
                if (r != null) {
                    return r;
                }
            }
        }
        return null;
    }
}
