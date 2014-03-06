#define LOG_TAG "DoremiJNI Demo"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "jni.h"
#include <android/log.h>

#define __DEBUG__

#ifdef __DEBUG__
#define LOGV(...) __android_log_print( ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__ )
#define LOGD(...) __android_log_print( ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )
#define LOGI(...) __android_log_print( ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__ )
#define LOGW(...) __android_log_print( ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__ )
#define LOGE(...) __android_log_print( ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__ )
#else
#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#endif

struct fields_t {
	jclass	clazzJNISvr;
	jmethodID midnativeFunc1;
	jmethodID midnativeFunc2;
	jfieldID  fidJniData1;
	jfieldID  fidJniData2;
};

static fields_t fields;

//full name determiners
static const char* const kClassName =
       "com/adam/app/hellojni/JNISvr";


void CallJavaFunction(JNIEnv *env, jobject clazz)
{
	LOGV("[%s] +++\n", __FUNCTION__);

	env->CallVoidMethod(clazz, fields.midnativeFunc1);

}

//Jni function implement
static jstring getJNIString(JNIEnv *env, jobject clazz)
{
	jstring str = NULL;

	const char* value = "Doremi JNI Demo Str";

	str = env->NewStringUTF(value);	
	return str;
}

static void testCallBack(JNIEnv *env, jobject clazz)
{
	LOGV("[%s]: clazz[%p]\n", __FUNCTION__, clazz);
	CallJavaFunction(env, clazz);
}

static void testStaticCallBack(JNIEnv *env, jobject clazz)
{
	LOGV("[%s]: clazz[%p]\n", __FUNCTION__, clazz);
    
    env->CallStaticVoidMethod(fields.clazzJNISvr, fields.midnativeFunc2);
    
}

static void setJavaData(JNIEnv *env, jobject clazz, jint value)
{
	LOGV("[%s] +++\n", __FUNCTION__);
//	LOGV("[%s]: value[%d]\n", __FUNCTION__, value);

//	jobject object = env->AllocObject(fields.clazzJNISvr);
//	LOGV("[%s]: object[%p]\n", __FUNCTION__, object);

	LOGV("[%s]: clazz[%p]\n", __FUNCTION__, clazz);
	env->SetIntField(clazz, fields.fidJniData1, value);

}

static void setJavaStaticData(JNIEnv *env, jobject clazz, jint value)
{
	LOGV("[%s] +++\n", __FUNCTION__);

	LOGV("[%s]: clazz[%p]\n", __FUNCTION__, clazz);
	env->SetStaticIntField(fields.clazzJNISvr, fields.fidJniData2, value);

}

//=====================================================================================
/*
 * Array of methods.
 *
 * Each entry has three fields: the name of the method, the method
 * signature, and a pointer to the native implementation.
 */
static const JNINativeMethod gMethods[] = {
    {"_getJNIString", "()Ljava/lang/String;", (void*)getJNIString},
    {"_testCallBack", "()V", (void*)testCallBack},
    {"_testStaticCallBack", "()V", (void*)testStaticCallBack},
    {"_setJavaData", "(I)V", (void*)setJavaData},
    {"_setJavaStaticData", "(I)V", (void*)setJavaStaticData},
};

static int registerMethods(JNIEnv* env) {

    jclass clazz;

    /* look up the class */
    clazz = env->FindClass(kClassName);
    if (clazz == NULL) {
        LOGE("Can't find class %s\n", kClassName);
        return -1;
    }

    //get java function id
    fields.midnativeFunc1 = env->GetMethodID(clazz, "nativeFunc1", "()V");
    fields.midnativeFunc2 = env->GetStaticMethodID(clazz, "nativeStaticFunc1", "()V");

    //get java data id
    fields.fidJniData1 = env->GetFieldID(clazz, "data1", "I");
    fields.fidJniData2 = env->GetStaticFieldID(clazz, "data2", "I");


    /* register all the methods */
    if (env->RegisterNatives(clazz, gMethods,
            sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK)
    {
        LOGE("Failed registering methods for %s\n", kClassName);
        return -1;
    }
    

    //get clazz reference
    fields.clazzJNISvr = (jclass)env->NewGlobalRef(clazz);

    /* fill out the rest of the ID cache */
    return 0;
}

/*
 * This is called by the VM when the shared library is first loaded.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;
	LOGI("JNI_OnLoad");

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed\n");
        goto fail;
    }
    assert(env != NULL);

    if (registerMethods(env) != 0) {
        LOGE("ERROR: PlatformLibrary native registration failed\n");
        goto fail;
    }



    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

fail:
    return result;
}
