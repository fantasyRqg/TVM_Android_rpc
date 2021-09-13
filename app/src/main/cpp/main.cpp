//
// Created by Rqg on 2019-08-20.
//


#include <jni.h>
#include <dmlc/logging.h>
#include <rpc_server.h>
#include <unistd.h>
#include <sys/wait.h>
#include <include/tvm/runtime/registry.h>

#include "log.h"

#define LOG_TAG "acs_native"


void start_rpc(JNIEnv *env, jclass, jint port, jstring _tracker_addr, jstring _custom_addr,
               jstring _thread_nums) {
    ALOGD("start_rpc");
//    "('192.168.31.79', 9190)"
//    "\"172.16.212.55\""
    auto tracker_addr = env->GetStringUTFChars(_tracker_addr, nullptr);
    auto custom_addr = env->GetStringUTFChars(_custom_addr, nullptr);
    auto thread_nums = env->GetStringUTFChars(_thread_nums, nullptr);


//    setenv("TVM_NUM_THREADS", "5", 1);
//    setenv("TVM_BIND_THREADS", "1", 1);
//
//    setenv("TVM_EXCLUDE_WORKER0", "1", 1);
//    (*tvm::runtime::Registry::Get("runtime.config_threadpool"))(-1, 5);

    tvm::runtime::RPCServerCreate("0.0.0.0", port, port + 100, tracker_addr, "android64", custom_addr,
                                  "", false);

    env->ReleaseStringUTFChars(_tracker_addr, tracker_addr);
    env->ReleaseStringUTFChars(_custom_addr, custom_addr);
    env->ReleaseStringUTFChars(_thread_nums, thread_nums);
    ALOGD("rpc end");
}

static const char *classPathName = "com/rqg/tvm_rpc/BridgeNative";
static JNINativeMethod methods[] = {
        {"runRPC", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void *) start_rpc},
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