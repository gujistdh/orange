package com.Orange;

/* loaded from: classes3.dex */
public final class NetTls {
    private static volatile boolean installed = false;

    private NetTls() {
    }

    public static synchronized void install() {
        synchronized (com.Orange.NetTls.class) {
            if (installed) {
                return;
            }
            try {
                javax.net.ssl.TrustManager[] managers = {new javax.net.ssl.X509TrustManager() { // from class: com.Orange.NetTls.1
                    @Override // javax.net.ssl.X509TrustManager
                    public void checkClientTrusted(java.security.cert.X509Certificate[] chain, java.lang.String authType) {
                    }

                    @Override // javax.net.ssl.X509TrustManager
                    public void checkServerTrusted(java.security.cert.X509Certificate[] chain, java.lang.String authType) {
                    }

                    @Override // javax.net.ssl.X509TrustManager
                    public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                        return new java.security.cert.X509Certificate[0];
                    }
                }};
                javax.net.ssl.SSLContext context = javax.net.ssl.SSLContext.getInstance("TLS");
                context.init(null, managers, new java.security.SecureRandom());
                javax.net.ssl.HttpsURLConnection.setDefaultSSLSocketFactory(context.getSocketFactory());
                javax.net.ssl.HttpsURLConnection.setDefaultHostnameVerifier(new javax.net.ssl.HostnameVerifier() { // from class: com.Orange.NetTls.2
                    @Override // javax.net.ssl.HostnameVerifier
                    public boolean verify(java.lang.String hostname, javax.net.ssl.SSLSession session) {
                        return true;
                    }
                });
                installed = true;
            } catch (java.lang.Throwable th) {
            }
        }
    }
}
