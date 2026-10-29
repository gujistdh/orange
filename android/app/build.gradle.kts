plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.Orange"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.Orange"
        minSdk = 26
        targetSdk = 36
        versionCode = 1
        versionName = "2.0.6"

        ndk {
            abiFilters += listOf("arm64-v8a")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    kotlinOptions {
        jvmTarget = "17"
    }

    buildFeatures {
        buildConfig = true
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    defaultConfig {
        externalNativeBuild {
            cmake {
                arguments += listOf("-DANDROID_STL=c++_static")
                cppFlags += listOf("-std=c++17")
            }
        }
    }

    packaging {
        jniLibs {
            useLegacyPackaging = false
        }
    }

    lint {
        abortOnError = false
        checkReleaseBuilds = false
    }

    sourceSets {
        getByName("main") {
            jniLibs.srcDirs("src/main/jniLibs")
        }
    }
}

dependencies {
    implementation("androidx.core:core:1.13.1")
    implementation("androidx.appcompat:appcompat:1.7.0")
    implementation("androidx.emoji2:emoji2:1.5.0")
    implementation("androidx.lifecycle:lifecycle-process:2.8.7")
    implementation("androidx.profileinstaller:profileinstaller:1.4.0")
    implementation("com.google.android.material:material:1.12.0")
    implementation("org.jetbrains.kotlin:kotlin-stdlib:2.0.21")
}
