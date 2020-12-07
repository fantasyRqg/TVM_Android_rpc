//
// Created by Rqg on 2019-08-20.
//


#include <jni.h>
#include <3rdparty/dmlc-core/include/dmlc/logging.h>
#include <dmlc/logging.h>
#include "rpc_server.h"

#include "log.h"

#define LOG_TAG "acs_native"


void start_rpc(JNIEnv *env, jclass) {
    ALOGD("start_rpc");
    tvm::runtime::RPCServerCreate("0.0.0.0", 9090, 9099, "('192.168.31.79', 9190)", "android", "\"172.16.212.55\"", false);
//    tvm::runtime::RPCServerCreate("0.0.0.0", 9090, 9099, "('192.168.31.79', 9190)", "android", "", false);
    ALOGD("rpc end");
}

static const char *classPathName = "com/rqg/tvm_rpc/BridgeNative";
static JNINativeMethod methods[] = {
        {"runRPC", "()V", (void *) start_rpc},
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv *env, const char *className,
                                 JNINativeMethod *gMethods, int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv *env) {
    if (!registerNativeMethods(env, classPathName,
                               methods, sizeof(methods) / sizeof(methods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
// ----------------------------------------------------------------------------
/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv *env;
    void *venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv *env = NULL;

    ALOGI("JNI_OnLoad");
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;
    if (registerNatives(env) != JNI_TRUE) {
        ALOGE("ERROR: registerNatives failed");
        goto bail;
    }

    result = JNI_VERSION_1_4;

    bail:
    return result;
}