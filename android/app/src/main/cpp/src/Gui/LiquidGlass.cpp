#include "orange/Gui/LiquidGlass.h"

#include <GLES3/gl3.h>

namespace Orange::Gui
{
    namespace
    {
        struct GlassRect
        {
            float X;
            float Y;
            float W;
            float H;
            float Radius;
            float Alpha;
            float ClipX;
            float ClipY;
            float ClipW;
            float ClipH;
            bool Clipped;
        };

        bool gEnabled = false;
        bool gInvalid = true;
        float gBlur = 8.0f;
        float gRefraction = 1.0f;
        float gRefractionHeight = 1.0f;
        float gAberration = 0.1f;
        int gWidth = 0;
        int gHeight = 0;

        std::vector<GlassRect> gRects;

        GLuint gTexture = 0;
        GLuint gProgram = 0;
        GLuint gVertexShader = 0;
        GLuint gFragmentShader = 0;
        GLint gUniformTexture = -1;
        GLint gUniformScreen = -1;
        GLint gUniformBlur = -1;
        GLint gUniformRefraction = -1;
        GLint gUniformAberration = -1;

        const char* const kVertexSource =
            "#version 300 es\n"
            "precision highp float;\n"
            "in vec2 aPos;\n"
            "out vec2 vUV;\n"
            "uniform vec2 uScreen;\n"
            "void main() {\n"
            "    vUV = aPos / uScreen;\n"
            "    gl_Position = vec4(aPos.x / uScreen.x * 2.0 - 1.0, 1.0 - aPos.y / uScreen.y * 2.0, 0.0, 1.0);\n"
            "}\n";

        const char* const kFragmentSource =
            "#version 300 es\n"
            "precision highp float;\n"
            "in vec2 vUV;\n"
            "out vec4 fragColor;\n"
            "uniform sampler2D uTex;\n"
            "uniform vec2 uScreen;\n"
            "uniform float uBlur;\n"
            "uniform float uRefraction;\n"
            "uniform float uAberration;\n"
            "void main() {\n"
            "    vec2 texel = 1.0 / uScreen;\n"
            "    vec2 offset = vec2(uBlur) * texel;\n"
            "    vec3 accum = vec3(0.0);\n"
            "    float total = 0.0;\n"
            "    for (int x = -3; x <= 3; ++x) {\n"
            "        for (int y = -3; y <= 3; ++y) {\n"
            "            float weight = 1.0 / (1.0 + float(x * x + y * y));\n"
            "            accum.r += texture(uTex, vUV + vec2(float(x), float(y)) * offset * (1.0 + uAberration)).r * weight;\n"
            "            accum.g += texture(uTex, vUV + vec2(float(x), float(y)) * offset).g * weight;\n"
            "            accum.b += texture(uTex, vUV + vec2(float(x), float(y)) * offset * (1.0 - uAberration)).b * weight;\n"
            "            total += weight;\n"
            "        }\n"
            "    }\n"
            "    vec3 blurred = accum / total;\n"
            "    vec2 centered = vUV - 0.5;\n"
            "    float edge = smoothstep(0.0, 0.5, length(centered) * uRefraction);\n"
            "    vec3 result = mix(blurred, vec3(1.0), edge * 0.08);\n"
            "    fragColor = vec4(result, 0.92);\n"
            "}\n";

        GLuint CompileShader(GLenum Type, const char* Source)
        {
            GLuint Shader = glCreateShader(Type);
            if (!Shader)
            {
                return 0;
            }
            glShaderSource(Shader, 1, &Source, nullptr);
            glCompileShader(Shader);
            GLint Status = 0;
            glGetShaderiv(Shader, GL_COMPILE_STATUS, &Status);
            if (!Status)
            {
                glDeleteShader(Shader);
                return 0;
            }
            return Shader;
        }
    }

    void LiquidGlass::SetEnabled(bool Value)
    {
        gEnabled = Value;
    }

    bool LiquidGlass::IsEnabled()
    {
        return gEnabled;
    }

    void LiquidGlass::SetParams(float Blur, float Refraction, float Height, float Aberration)
    {
        gBlur = Blur;
        gRefraction = Refraction;
        gRefractionHeight = Height;
        gAberration = Aberration;
    }

    float LiquidGlass::BlurRadius() { return gBlur; }
    float LiquidGlass::RefractionAmount() { return gRefraction; }
    float LiquidGlass::RefractionHeight() { return gRefractionHeight; }
    float LiquidGlass::ChromaticAberration() { return gAberration; }

    bool LiquidGlass::EnsureProgram()
    {
        if (gProgram)
        {
            return true;
        }
        gVertexShader = CompileShader(GL_VERTEX_SHADER, kVertexSource);
        if (!gVertexShader)
        {
            return false;
        }
        gFragmentShader = CompileShader(GL_FRAGMENT_SHADER, kFragmentSource);
        if (!gFragmentShader)
        {
            glDeleteShader(gVertexShader);
            gVertexShader = 0;
            return false;
        }
        gProgram = glCreateProgram();
        glAttachShader(gProgram, gVertexShader);
        glAttachShader(gProgram, gFragmentShader);
        glLinkProgram(gProgram);
        GLint Status = 0;
        glGetProgramiv(gProgram, GL_LINK_STATUS, &Status);
        if (!Status)
        {
            glDeleteProgram(gProgram);
            gProgram = 0;
            return false;
        }
        gUniformTexture = glGetUniformLocation(gProgram, "uTex");
        gUniformScreen = glGetUniformLocation(gProgram, "uScreen");
        gUniformBlur = glGetUniformLocation(gProgram, "uBlur");
        gUniformRefraction = glGetUniformLocation(gProgram, "uRefraction");
        gUniformAberration = glGetUniformLocation(gProgram, "uAberration");
        return true;
    }

    bool LiquidGlass::EnsureTexture(int Width, int Height)
    {
        if (gTexture && gWidth == Width && gHeight == Height && !gInvalid)
        {
            return true;
        }
        if (gTexture)
        {
            glDeleteTextures(1, &gTexture);
            gTexture = 0;
        }
        glGenTextures(1, &gTexture);
        if (!gTexture)
        {
            return false;
        }
        glBindTexture(GL_TEXTURE_2D, gTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        gWidth = Width;
        gHeight = Height;
        gInvalid = false;
        return true;
    }

    bool LiquidGlass::CaptureBackdrop(int Width, int Height)
    {
        if (!gEnabled || Width <= 0 || Height <= 0)
        {
            return false;
        }
        if (!EnsureProgram() || !EnsureTexture(Width, Height))
        {
            return false;
        }
        glBindTexture(GL_TEXTURE_2D, gTexture);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    void LiquidGlass::BeginFrame()
    {
        gRects.clear();
    }

    void LiquidGlass::AddRect(float X, float Y, float W, float H, float Radius, float Alpha)
    {
        GlassRect Entry{};
        Entry.X = X;
        Entry.Y = Y;
        Entry.W = W;
        Entry.H = H;
        Entry.Radius = Radius;
        Entry.Alpha = Alpha;
        Entry.Clipped = false;
        gRects.push_back(Entry);
    }

    void LiquidGlass::AddRectClipped(float X, float Y, float W, float H, float Radius, float Alpha,
                                     float ClipX, float ClipY, float ClipW, float ClipH)
    {
        GlassRect Entry{};
        Entry.X = X;
        Entry.Y = Y;
        Entry.W = W;
        Entry.H = H;
        Entry.Radius = Radius;
        Entry.Alpha = Alpha;
        Entry.ClipX = ClipX;
        Entry.ClipY = ClipY;
        Entry.ClipW = ClipW;
        Entry.ClipH = ClipH;
        Entry.Clipped = true;
        gRects.push_back(Entry);
    }

    void LiquidGlass::Render(int Width, int Height)
    {
        if (!gEnabled || !gProgram || !gTexture || gRects.empty())
        {
            return;
        }
        glUseProgram(gProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gTexture);
        if (gUniformTexture >= 0)
        {
            glUniform1i(gUniformTexture, 0);
        }
        if (gUniformScreen >= 0)
        {
            glUniform2f(gUniformScreen, static_cast<float>(Width), static_cast<float>(Height));
        }
        if (gUniformBlur >= 0)
        {
            glUniform1f(gUniformBlur, gBlur);
        }
        if (gUniformRefraction >= 0)
        {
            glUniform1f(gUniformRefraction, gRefraction);
        }
        if (gUniformAberration >= 0)
        {
            glUniform1f(gUniformAberration, gAberration);
        }

        float Quad[8];
        for (const GlassRect& Entry : gRects)
        {
            float MinX = Entry.X;
            float MinY = Entry.Y;
            float MaxX = Entry.X + Entry.W;
            float MaxY = Entry.Y + Entry.H;
            if (Entry.Clipped)
            {
                MinX = std::max(MinX, Entry.ClipX);
                MinY = std::max(MinY, Entry.ClipY);
                MaxX = std::min(MaxX, Entry.ClipX + Entry.ClipW);
                MaxY = std::min(MaxY, Entry.ClipY + Entry.ClipH);
            }
            if (MaxX <= MinX || MaxY <= MinY)
            {
                continue;
            }
            Quad[0] = MinX; Quad[1] = MinY;
            Quad[2] = MaxX; Quad[3] = MinY;
            Quad[4] = MaxX; Quad[5] = MaxY;
            Quad[6] = MinX; Quad[7] = MaxY;
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, Quad);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisableVertexAttribArray(0);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

    void LiquidGlass::Release()
    {
        if (gTexture)
        {
            glDeleteTextures(1, &gTexture);
            gTexture = 0;
        }
        if (gProgram)
        {
            glDeleteProgram(gProgram);
            gProgram = 0;
        }
        if (gVertexShader)
        {
            glDeleteShader(gVertexShader);
            gVertexShader = 0;
        }
        if (gFragmentShader)
        {
            glDeleteShader(gFragmentShader);
            gFragmentShader = 0;
        }
        gWidth = 0;
        gHeight = 0;
        gInvalid = true;
        gRects.clear();
    }

    void LiquidGlass::Invalidate()
    {
        gInvalid = true;
    }

    unsigned int Glass::Col(unsigned int Color, float Alpha)
    {
        if (Alpha < 0.0f)
        {
            Alpha = 0.0f;
        }
        if (Alpha > 1.0f)
        {
            Alpha = 1.0f;
        }
        unsigned int A = static_cast<unsigned int>(Alpha * 255.0f);
        return (Color & 0x00FFFFFFu) | (A << 24);
    }

    void Glass::Panel(ImDrawList* Draw, const ImVec2& Min, const ImVec2& Max, float Radius, float Alpha)
    {
        if (!Draw)
        {
            return;
        }
        Draw->AddRectFilled(Min, Max, Col(0x20FFFFFFu, Alpha), Radius);
        Draw->AddRect(Min, Max, Col(0x40FFFFFFu, Alpha), Radius, 0, 1.0f);
    }

    void Glass::Chip(ImDrawList* Draw, const ImVec2& Min, const ImVec2& Max, float Radius, unsigned int Color, float Alpha)
    {
        if (!Draw)
        {
            return;
        }
        Draw->AddRectFilled(Min, Max, Col(Color, Alpha), Radius);
    }
}
