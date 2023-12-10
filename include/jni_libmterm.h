#ifndef _JNI_LIBMTERM_H_
#define _JNI_LIBMTERM_H_

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
    );
    JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_createMtermDefault
    (
        JNIEnv* env,
        jclass clazz
    );
    JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_destroyMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id
    );
    JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_readMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id,
        jbyteArray buf,
        jint size
    );
    JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_writeMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id,
        jbyteArray buf,
        jint size
    );
    JNIEXPORT jint JNICALL Java_com_mterm_lib_JNI_waitMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id
    );
    JNIEXPORT void JNICALL Java_com_mterm_lib_JNI_setReadNonblockMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id
    );
    JNIEXPORT void JNICALL Java_com_mterm_lib_JNI_setWindowSizeMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id,
        jint rows,
        jint cols
    );
    JNIEXPORT jboolean JNICALL Java_com_mterm_lib_JNI_checkRunningMterm
    (
        JNIEnv* env,
        jclass clazz,
        jint id
    );

#ifdef __cplusplus
}
#endif

#endif //_JNI_LIBMTERM_H_