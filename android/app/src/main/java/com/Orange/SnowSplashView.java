package com.Orange;

/* loaded from: classes3.dex */
public class SnowSplashView extends android.view.View {
    private static final int FLAKE_COUNT = 100;
    private static final int FLAKE_TEX = 256;
    private static final float SUB_SP = 18.0f;
    private static final java.lang.String TITLE_LINE1 = "开发者:橘子";
    private static final java.lang.String TITLE_LINE2 = "主负责:明明";
    private static final float TITLE_SP = 26.0f;
    private static final int VARIANTS = 3;
    private final android.graphics.Paint bgPaint;
    private final float[] fAlpha;
    private final float[] fSize;
    private final float[] fSpeed;
    private final float[] fSpin;
    private final float[] fSpinRate;
    private final float[] fSwayAmp;
    private final float[] fSwayPhase;
    private final float[] fSwayRate;
    private final int[] fVariant;
    private final android.graphics.Paint flakePaint;
    private final android.graphics.Bitmap[] flakeTex;
    private float flowTime;
    private final float[] fx;
    private final float[] fy;
    private float gradientOffset;
    private float gradientPeriod;
    private int height;
    private long lastNanos;
    private final java.util.Random rnd;
    private boolean seeded;
    private final android.graphics.Matrix shapeMatrix;
    private final android.graphics.Paint subPaint;
    private float subWidth;
    private final android.graphics.Paint titlePaint;
    private android.graphics.LinearGradient titleShader;
    private float titleWidth;
    private int width;

    public SnowSplashView(android.content.Context context) {
        super(context);
        this.flakePaint = new android.graphics.Paint(3);
        this.titlePaint = new android.graphics.Paint(1);
        this.subPaint = new android.graphics.Paint(1);
        this.bgPaint = new android.graphics.Paint(1);
        this.shapeMatrix = new android.graphics.Matrix();
        this.rnd = new java.util.Random(20260922L);
        this.flakeTex = new android.graphics.Bitmap[3];
        this.fx = new float[100];
        this.fy = new float[100];
        this.fSpeed = new float[100];
        this.fSize = new float[100];
        this.fSwayAmp = new float[100];
        this.fSwayPhase = new float[100];
        this.fSwayRate = new float[100];
        this.fSpin = new float[100];
        this.fSpinRate = new float[100];
        this.fAlpha = new float[100];
        this.fVariant = new int[100];
        this.titleShader = null;
        this.titleWidth = 0.0f;
        this.subWidth = 0.0f;
        this.gradientPeriod = 1.0f;
        this.gradientOffset = 0.0f;
        this.flowTime = 0.0f;
        this.width = 0;
        this.height = 0;
        this.seeded = false;
        this.lastNanos = 0L;
        init(context);
    }

    public SnowSplashView(android.content.Context context, android.util.AttributeSet attrs) {
        super(context, attrs);
        this.flakePaint = new android.graphics.Paint(3);
        this.titlePaint = new android.graphics.Paint(1);
        this.subPaint = new android.graphics.Paint(1);
        this.bgPaint = new android.graphics.Paint(1);
        this.shapeMatrix = new android.graphics.Matrix();
        this.rnd = new java.util.Random(20260922L);
        this.flakeTex = new android.graphics.Bitmap[3];
        this.fx = new float[100];
        this.fy = new float[100];
        this.fSpeed = new float[100];
        this.fSize = new float[100];
        this.fSwayAmp = new float[100];
        this.fSwayPhase = new float[100];
        this.fSwayRate = new float[100];
        this.fSpin = new float[100];
        this.fSpinRate = new float[100];
        this.fAlpha = new float[100];
        this.fVariant = new int[100];
        this.titleShader = null;
        this.titleWidth = 0.0f;
        this.subWidth = 0.0f;
        this.gradientPeriod = 1.0f;
        this.gradientOffset = 0.0f;
        this.flowTime = 0.0f;
        this.width = 0;
        this.height = 0;
        this.seeded = false;
        this.lastNanos = 0L;
        init(context);
    }

    public SnowSplashView(android.content.Context context, android.util.AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.flakePaint = new android.graphics.Paint(3);
        this.titlePaint = new android.graphics.Paint(1);
        this.subPaint = new android.graphics.Paint(1);
        this.bgPaint = new android.graphics.Paint(1);
        this.shapeMatrix = new android.graphics.Matrix();
        this.rnd = new java.util.Random(20260922L);
        this.flakeTex = new android.graphics.Bitmap[3];
        this.fx = new float[100];
        this.fy = new float[100];
        this.fSpeed = new float[100];
        this.fSize = new float[100];
        this.fSwayAmp = new float[100];
        this.fSwayPhase = new float[100];
        this.fSwayRate = new float[100];
        this.fSpin = new float[100];
        this.fSpinRate = new float[100];
        this.fAlpha = new float[100];
        this.fVariant = new int[100];
        this.titleShader = null;
        this.titleWidth = 0.0f;
        this.subWidth = 0.0f;
        this.gradientPeriod = 1.0f;
        this.gradientOffset = 0.0f;
        this.flowTime = 0.0f;
        this.width = 0;
        this.height = 0;
        this.seeded = false;
        this.lastNanos = 0L;
        init(context);
    }

    private void init(android.content.Context context) {
        this.bgPaint.setShader(new android.graphics.LinearGradient(0.0f, 0.0f, 0.0f, 1.0f, new int[]{android.graphics.Color.parseColor("#04101F"), android.graphics.Color.parseColor("#0B1436"), android.graphics.Color.parseColor("#160A2E")}, new float[]{0.0f, 0.55f, 1.0f}, android.graphics.Shader.TileMode.CLAMP));
        this.flakePaint.setStyle(android.graphics.Paint.Style.FILL);
        this.flakePaint.setColor(-1);
        this.titlePaint.setTypeface(android.graphics.Typeface.create(android.graphics.Typeface.DEFAULT, 1));
        this.titlePaint.setTextSize(android.util.TypedValue.applyDimension(2, TITLE_SP, context.getResources().getDisplayMetrics()));
        this.titlePaint.setTextAlign(android.graphics.Paint.Align.LEFT);
        this.titlePaint.setSubpixelText(true);
        this.titlePaint.setShadowLayer(android.util.TypedValue.applyDimension(2, 5.0f, context.getResources().getDisplayMetrics()), 0.0f, 0.0f, android.graphics.Color.parseColor("#8046A8FF"));
        this.subPaint.setTypeface(android.graphics.Typeface.create(android.graphics.Typeface.DEFAULT, 0));
        this.subPaint.setTextSize(android.util.TypedValue.applyDimension(2, SUB_SP, context.getResources().getDisplayMetrics()));
        this.subPaint.setTextAlign(android.graphics.Paint.Align.LEFT);
        this.subPaint.setSubpixelText(true);
        this.subPaint.setColor(android.graphics.Color.parseColor("#A9C7FF"));
        this.subPaint.setShadowLayer(android.util.TypedValue.applyDimension(2, 4.0f, context.getResources().getDisplayMetrics()), 0.0f, 0.0f, android.graphics.Color.parseColor("#4046A8FF"));
        for (int i = 0; i < 3; i++) {
            this.flakeTex[i] = buildFlake(i);
        }
    }

    private android.graphics.Bitmap buildFlake(int variant) {
        float branchA;
        float branchB;
        int i;
        android.graphics.Bitmap bmp = android.graphics.Bitmap.createBitmap(256, 256, android.graphics.Bitmap.Config.ARGB_8888);
        android.graphics.Canvas c = new android.graphics.Canvas(bmp);
        android.graphics.Paint p = new android.graphics.Paint(1);
        p.setStyle(android.graphics.Paint.Style.STROKE);
        p.setStrokeWidth(4.0f);
        p.setStrokeCap(android.graphics.Paint.Cap.ROUND);
        p.setStrokeJoin(android.graphics.Paint.Join.ROUND);
        p.setColor(-1);
        float cx = 128.0f;
        float cy = 128.0f;
        if (variant == 0) {
            branchA = 0.34f;
        } else {
            branchA = variant == 1 ? 0.28f : 0.4f;
        }
        if (variant == 0) {
            branchB = 0.24f;
        } else {
            branchB = variant == 1 ? 0.32f : 0.2f;
        }
        android.graphics.Path path = new android.graphics.Path();
        int k = 0;
        while (true) {
            if (k >= 6) {
                break;
            }
            float cx2 = cx;
            double ang = java.lang.Math.toRadians(k * 60.0d);
            float dx = (float) java.lang.Math.cos(ang);
            float dy = (float) java.lang.Math.sin(ang);
            float tipX = cx2 + (dx * 112.64f);
            float tipY = (dy * 112.64f) + 128.0f;
            path.moveTo(cx2, 128.0f);
            path.lineTo(tipX, tipY);
            float baseA = 112.64f * 0.42f;
            float baseB = 112.64f * 0.68f;
            addBranch(path, cx2 + (dx * baseA), 128.0f + (dy * baseA), ang, 112.64f * branchA);
            addBranch(path, cx2 + (dx * baseB), 128.0f + (dy * baseB), ang, 112.64f * branchB);
            if (variant == 2) {
                addBranch(path, tipX, tipY, ang, 112.64f * 0.16f);
            }
            k++;
            cx = cx2;
        }
        float cx3 = cx;
        c.drawPath(path, p);
        if (variant == 1) {
            return bmp;
        }
        p.setStyle(android.graphics.Paint.Style.STROKE);
        p.setStrokeWidth(3.0f);
        android.graphics.Path hex = new android.graphics.Path();
        float hr = 21.76f;
        int k2 = 0;
        for (i = 6; k2 < i; i = 6) {
            android.graphics.Path path2 = path;
            double ang2 = java.lang.Math.toRadians((k2 * 60.0d) + 30.0d);
            android.graphics.Bitmap bmp2 = bmp;
            float px = cx3 + (((float) java.lang.Math.cos(ang2)) * hr);
            float hr2 = hr;
            float cy2 = cy;
            float py = cy2 + (((float) java.lang.Math.sin(ang2)) * hr2);
            if (k2 == 0) {
                hex.moveTo(px, py);
            } else {
                hex.lineTo(px, py);
            }
            k2++;
            cy = cy2;
            path = path2;
            bmp = bmp2;
            hr = hr2;
        }
        android.graphics.Bitmap bmp3 = bmp;
        hex.close();
        c.drawPath(hex, p);
        return bmp3;
    }

    private void addBranch(android.graphics.Path path, float x, float y, double armAngle, float len) {
        for (int s = -1; s <= 1; s += 2) {
            double a = (s * java.lang.Math.toRadians(58.0d)) + armAngle;
            path.moveTo(x, y);
            path.lineTo((((float) java.lang.Math.cos(a)) * len) + x, (((float) java.lang.Math.sin(a)) * len) + y);
        }
    }

    private void seedFlakes() {
        if (this.width <= 0 || this.height <= 0) {
            return;
        }
        float minSide = java.lang.Math.min(this.width, this.height);
        for (int i = 0; i < 100; i++) {
            this.fx[i] = this.rnd.nextFloat() * this.width;
            this.fy[i] = this.rnd.nextFloat() * this.height;
            this.fSpeed[i] = this.height * ((this.rnd.nextFloat() * 0.15f) + 0.035f);
            this.fSize[i] = ((this.rnd.nextFloat() * 0.052f) + 0.02f) * minSide;
            this.fSwayAmp[i] = ((this.rnd.nextFloat() * 0.055f) + 0.01f) * minSide;
            this.fSwayPhase[i] = this.rnd.nextFloat() * 6.2832f;
            this.fSwayRate[i] = (this.rnd.nextFloat() * 1.15f) + 0.35f;
            this.fSpin[i] = this.rnd.nextFloat() * 360.0f;
            this.fSpinRate[i] = (this.rnd.nextBoolean() ? 1.0f : -1.0f) * ((this.rnd.nextFloat() * 52.0f) + 12.0f);
            this.fAlpha[i] = (this.rnd.nextFloat() * 200.0f) + 55.0f;
            this.fVariant[i] = this.rnd.nextInt(3);
        }
        this.seeded = true;
    }

    private void ensureTitleShader() {
        if (this.titleShader != null) {
            return;
        }
        this.titleWidth = this.titlePaint.measureText(TITLE_LINE1);
        this.subWidth = this.subPaint.measureText(TITLE_LINE2);
        if (this.titleWidth <= 1.0f) {
            return;
        }
        int blue = android.graphics.Color.parseColor("#3AA0FF");
        int purple = android.graphics.Color.parseColor("#8B5CF6");
        this.gradientPeriod = this.titleWidth;
        this.titleShader = new android.graphics.LinearGradient(-this.gradientPeriod, 0.0f, 0.0f, 0.0f, new int[]{blue, purple, blue}, new float[]{0.0f, 0.5f, 1.0f}, android.graphics.Shader.TileMode.REPEAT);
        this.titlePaint.setShader(this.titleShader);
    }

    @Override // android.view.View
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        this.width = w;
        this.height = h;
        this.bgPaint.setShader(new android.graphics.LinearGradient(0.0f, 0.0f, 0.0f, h, new int[]{android.graphics.Color.parseColor("#04101F"), android.graphics.Color.parseColor("#0B1436"), android.graphics.Color.parseColor("#160A2E")}, new float[]{0.0f, 0.55f, 1.0f}, android.graphics.Shader.TileMode.CLAMP));
        seedFlakes();
    }

    @Override // android.view.View
    protected void onDraw(android.graphics.Canvas canvas) {
        float dt;
        super.onDraw(canvas);
        if (this.width > 0 && this.height > 0) {
            if (!this.seeded) {
                seedFlakes();
            }
            ensureTitleShader();
            long now = java.lang.System.nanoTime();
            if (this.lastNanos == 0) {
                dt = 0.016f;
            } else {
                float dt2 = (float) ((now - this.lastNanos) / 1.0E9d);
                if (dt2 < 0.001f) {
                    dt2 = 0.001f;
                }
                if (dt2 > 0.05f) {
                    dt2 = 0.05f;
                }
                dt = dt2;
            }
            this.lastNanos = now;
            this.flowTime += dt;
            canvas.drawRect(0.0f, 0.0f, this.width, this.height, this.bgPaint);
            float halfTex = 128.0f;
            int i = 0;
            while (i < 100) {
                float[] fArr = this.fy;
                fArr[i] = fArr[i] + (this.fSpeed[i] * dt);
                float[] fArr2 = this.fSpin;
                fArr2[i] = fArr2[i] + (this.fSpinRate[i] * dt);
                if (this.fSpin[i] > 360.0f) {
                    float[] fArr3 = this.fSpin;
                    fArr3[i] = fArr3[i] - 360.0f;
                }
                if (this.fSpin[i] < 0.0f) {
                    float[] fArr4 = this.fSpin;
                    fArr4[i] = fArr4[i] + 360.0f;
                }
                float drawSize = this.fSize[i];
                if (this.fy[i] - drawSize > this.height) {
                    this.fy[i] = (-drawSize) - ((this.rnd.nextFloat() * this.height) * 0.15f);
                    this.fx[i] = this.rnd.nextFloat() * this.width;
                    this.fSpeed[i] = this.height * ((this.rnd.nextFloat() * 0.15f) + 0.035f);
                    this.fSize[i] = java.lang.Math.min(this.width, this.height) * ((this.rnd.nextFloat() * 0.052f) + 0.02f);
                    this.fAlpha[i] = (this.rnd.nextFloat() * 200.0f) + 55.0f;
                    this.fSpinRate[i] = (this.rnd.nextBoolean() ? 1.0f : -1.0f) * ((this.rnd.nextFloat() * 52.0f) + 12.0f);
                }
                float drift = (float) java.lang.Math.sin(this.fSwayPhase[i] + (this.flowTime * this.fSwayRate[i]));
                float px = this.fx[i] + (this.fSwayAmp[i] * drift);
                float py = this.fy[i];
                float scale = (2.0f * drawSize) / 256.0f;
                int alpha = (int) this.fAlpha[i];
                if (alpha > 255) {
                    alpha = 255;
                }
                if (alpha < 0) {
                    alpha = 0;
                }
                this.flakePaint.setAlpha(alpha);
                int save = canvas.save();
                canvas.translate(px, py);
                canvas.rotate(this.fSpin[i]);
                canvas.scale(scale, scale);
                canvas.drawBitmap(this.flakeTex[this.fVariant[i]], -halfTex, -halfTex, this.flakePaint);
                canvas.restoreToCount(save);
                i++;
                halfTex = halfTex;
            }
            if (this.titleShader != null) {
                this.gradientOffset += this.gradientPeriod * dt * 0.55f;
                while (this.gradientOffset > this.gradientPeriod) {
                    this.gradientOffset -= this.gradientPeriod;
                }
                this.shapeMatrix.setTranslate(this.gradientOffset, 0.0f);
                this.titleShader.setLocalMatrix(this.shapeMatrix);
                float pulse = (((float) java.lang.Math.sin(this.flowTime * 1.6f)) * 0.28f) + 0.72f;
                this.titlePaint.setShadowLayer(this.titlePaint.getTextSize() * 0.26f, 0.0f, 0.0f, android.graphics.Color.argb((int) (150.0f * pulse), 98, 176, 255));
                float cy = this.height * 0.5f;
                float gap = this.titlePaint.getTextSize() * 1.15f;
                float tx1 = (this.width - this.titleWidth) * 0.5f;
                float ty1 = (cy - (gap * 0.5f)) - ((this.titlePaint.descent() + this.titlePaint.ascent()) * 0.5f);
                canvas.drawText(TITLE_LINE1, tx1, ty1, this.titlePaint);
                float tx2 = (this.width - this.subWidth) * 0.5f;
                float ty2 = ((gap * 0.5f) + cy) - ((this.subPaint.descent() + this.subPaint.ascent()) * 0.5f);
                int subAlpha = (int) ((70.0f * pulse) + 175.0f);
                if (subAlpha > 255) {
                    subAlpha = 255;
                }
                this.subPaint.setAlpha(subAlpha);
                canvas.drawText(TITLE_LINE2, tx2, ty2, this.subPaint);
            }
            postInvalidateOnAnimation();
        }
    }
}
