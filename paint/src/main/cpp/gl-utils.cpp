#include "gl-utils.h"

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error ; error = glGetError()) {
        switch (error) {
            case GL_INVALID_ENUM:
                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Invalid argument enum");
                break;
            case GL_INVALID_VALUE:
                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Invalid argument value");
                break;
            case GL_INVALID_OPERATION:
                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Invalid operation");
                break;
//            case GL_STACK_OVERFLOW_KHR: // v2
//                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Stack Overflow");
//                break;
//            case GL_STAC: // v2
//                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Stack Underflow");
//                break;
            case GL_OUT_OF_MEMORY:
                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Out Of Memory");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Frame Buffer Error");
                break;
//            case GL_CONTEXT_LOST_KHR: v2
//                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Context Lost(Graphic Card Reset)");
//                break;
            default:
                LOGE(TAG, "Operation: %s Error: 0x%x(%s)", op, error, "Unknown Error");
                break;
        }
    }
}

void printGlInfo() {
    LOGI(TAG, "OpenGL ES: %-30s : %s", "GL Version", glGetString(GL_VERSION));
    LOGI(TAG, "OpenGL ES: %-30s : %s", "GL Shader Version", glGetString(GL_SHADING_LANGUAGE_VERSION));
    LOGI(TAG, "OpenGL ES: %-30s : %s", "GL Vender", glGetString(GL_VENDOR));
    LOGI(TAG, "OpenGL ES: %-30s : %s", "GL Renderer", glGetString(GL_RENDERER));
    LOGI(TAG, "OpenGL ES: %-30s : %s", "GL Extensions", glGetString(GL_EXTENSIONS));
}

GLuint loadShader(GLenum shaderType, const char *pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char buf[infoLen];
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE(TAG, "Could not compile shader %s: \n%s\n", shaderType == GL_VERTEX_SHADER ? "Vertex Shader": "Fragment Shader", buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char *pVertexSource, const char *pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader)
        return 0;
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!fragmentShader)
        return 0;

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachVertexShader");
        glAttachShader(program, fragmentShader);
        checkGlError("glAttachPixelShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char buf[bufLength];
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE(TAG, "Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}