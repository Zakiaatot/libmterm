#include <cstdlib>
#include <cstring>
#include "jni_libmterm.h"
#include "libmterm.h"
#include "error_number.h"

JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_createMterm
(
    JNIEnv* env,
    jclass clazz,
    jstring cmd,
    jstring cwd,
    jobjectArray argv,
    jobjectArray envp,
    jint rows,
    jint cols
)
{
    // argv jstr to char *
    jsize size = argv ? env->GetArrayLength(argv) : 0;
    char** argvPtr = NULL;
    if (size > 0)
    {
        argvPtr = (char**)malloc((size + 1) * sizeof(char*));
        if (!argvPtr) return JNI_MALLOC_ERROR;
        for (int i = 0; i < size; ++i)
        {
            jstring argJstr = (jstring)(env->GetObjectArrayElement(argv, i));
            const char* argUtf8 = env->GetStringUTFChars(argJstr, NULL);
            if (!argUtf8) return JNI_JSTRTOUTF8CHAR_ERROR;
            argvPtr[i] = strdup(argUtf8);
            env->ReleaseStringUTFChars(argJstr, argUtf8);
        }
        argvPtr[size] = NULL;
    }

    // envp jstr to char *
    size = envp ? env->GetArrayLength(envp) : 0;
    char** envpPtr = NULL;
    if (size > 0)
    {
        envpPtr = (char**)malloc((size + 1) * sizeof(char*));
        if (!envpPtr) return JNI_MALLOC_ERROR;
        for (int i = 0; i < size; ++i)
        {
            jstring envJstr = (jstring)(env->GetObjectArrayElement(envp, i));
            const char* envUtf8 = env->GetStringUTFChars(envJstr, NULL);
            if (!envUtf8) return JNI_JSTRTOUTF8CHAR_ERROR;
            envpPtr[i] = strdup(envUtf8);
            env->ReleaseStringUTFChars(envJstr, envUtf8);
        }
        envpPtr[size] = NULL;
    }

    //cmd cwd
    const char* cmdPtr = env->GetStringUTFChars(cmd, NULL);
    const char* cwdPtr = env->GetStringUTFChars(cwd, NULL);

    // create
    int res = CreateMterm(cmdPtr, cwdPtr, argvPtr, envpPtr, rows, cols);

    // release str
    if (argvPtr)
    {
        for (char** tmp = argvPtr; *tmp; ++tmp)
            free(*tmp);
        free(argvPtr);
    }
    if (envpPtr)
    {
        for (char** tmp = envpPtr; *tmp; ++tmp)
            free(*tmp);
        free(envpPtr);
    }

    return res;
}

JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_createMtermDefault
(
    JNIEnv* env,
    jclass clazz
)
{
    return CreateMtermDefault();
}

JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_destroyMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id
)
{
    return DestroyMterm(id);
}

JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_readMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id,
    jbyteArray buf,
    jint size
)
{
    jbyte* buffer = env->GetByteArrayElements(buf, NULL);
    int res = ReadMterm(id, (char*)buffer, size);
    env->ReleaseByteArrayElements(buf, buffer, 0);
    return res;
}

JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_writeMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id,
    jbyteArray buf,
    jint size
)
{
    jbyte* buffer = env->GetByteArrayElements(buf, NULL);
    int res = WriteMterm(id, (char*)buffer, size);
    env->ReleaseByteArrayElements(buf, buffer, 0);
    return res;
}

JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_waitMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id
)
{
    return  WaitMterm(id);
}

JNIEXPORT void JNICALL Java_com_mterm_lib_JNI_setReadNonblockMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id
)
{
    return SetReadNonblockMterm(id);
}

JNIEXPORT void JNICALL Java_com_mterm_lib_JNI_setWindowSizeMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id,
    jint rows,
    jint cols
)
{
    return SetWindowSizeMterm(id, rows, cols);
}

JNIEXPORT jboolean JNICALL Java_com_mterm_lib_JNI_checkRunningMterm
(
    JNIEnv* env,
    jclass clazz,
    jint id
)
{
    return CheckRunningMterm(id);
}