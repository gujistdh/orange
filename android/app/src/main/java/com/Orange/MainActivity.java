package com.Orange;

/* loaded from: classes3.dex */
public class MainActivity extends androidx.appcompat.app.AppCompatActivity {
    @Override // androidx.fragment.app.FragmentActivity, androidx.activity.ComponentActivity, androidx.core.app.ComponentActivity, android.app.Activity
    protected void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        androidx.activity.EdgeToEdge.enable(this);
        setContentView(com.Orange.R.layout.activity_main);
    }
}
