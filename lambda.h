/*
* =====================================================================================
 *
 *       Filename:  lambda.h
 *
 *    Description:  Idiomatic C99 interface to interact with Lambda
 *
 *        Version:  0.0.1
 *        Created:  2025-04-12 17:14:01
 *       Compiler:  clang
 *
 *         Author:  Emy 💜 (https://emy.systems),
 *
 * =====================================================================================
 */

#ifndef LAMBDA_H
#define LAMBDA_H

#include <jvmti.h>
#include <jni.h>

typedef struct {
    jclass      klass;
    jobject     instance;

    jmethodID   get_version_m;
    jmethodID   get_log_m;
    jmethodID   get_mc_m;
    jmethodID   is_debug_m;
} lambda_o;


/*
 * ===  FUNCTION  ======================================================================
 *         Name:    get_lambda
 *  Description:
 * =====================================================================================
 */
void
    get_lambda(JNIEnv *env, lambda_o *lambda);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    lambda_version
 *  Description:    Returns a copied utf8 jstring of the mod's version.
 *                  Must be freed with ReleaseStringChars.
 * =====================================================================================
 */
const char*
    lambda_version(JNIEnv *env, const lambda_o *lambda);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    lambda_log
 *  Description:    Returns the Log4j class instance of Lambda.
 * =====================================================================================
 */
jobject
    lambda_log(JNIEnv *env, const lambda_o *lambda);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    lambda_mc
 *  Description:    Returns the MinecraftClient class instance.
 * =====================================================================================
 */
void*
    lambda_mc(JNIEnv *env, const lambda_o *lambda);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    lambda_is_debug
 *  Description:    Returns whether the mod is running in the development environment.
 * =====================================================================================
 */
jboolean
    lambda_is_debug(JNIEnv *env, const lambda_o *lambda);

/*
 * ====================================================================================
 * ====================================================================================
 * ====================================================================================
 */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    get_sclass
 *  Description:    Finds the given class and its static INSTANCE field.
 * =====================================================================================
 */
void
    get_sclass(
        JNIEnv      *env,
        const char  *class,
        const char  *sig,
        jclass      *klass,
        jobject     *instance);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    get_method
 *  Description:    Finds a method on the given class.
 * =====================================================================================
 */
void
    // Finds a method on the given class
    get_method(
        JNIEnv          *env,
        const char      *name,
        const char      *sig,
        const jclass    *klass,
        jmethodID       *method);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:    get_method
 *  Description:    Finds a static method on the given class.
 * =====================================================================================
 */
void
    get_smethod(
        JNIEnv          *env,
        const char      *name,
        const char      *sig,
        const jclass    *klass,
        jmethodID       *method);

#ifndef LAMBDA_IMPL
#define LAMBDA_IMPL

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SIG_INT       "I"
#define SIG_BOOLEAN   "Z"
#define SIG_BYTE      "B"
#define SIG_CHAR      "C"
#define SIG_SHORT     "S"
#define SIG_LONG      "J"
#define SIG_FLOAT     "F"
#define SIG_DOUBLE    "D"
#define SIG_VOID      "V"

#define SIG_CLASS(name)  "L" name ";"
#define SIG_ARRAY(type) "[" type
#define SIG_FUNC(ret, ...)  "(" __VA_ARGS__ ")" ret

inline
void
    // Finds the given class name and return its static INSTANCE field.
    get_sclass(
        JNIEnv      *env,
        const char  *class,
        const char  *sig,
        jclass      *klass,
        jobject     *instance
)
{
    assert(NULL != env && "The JNI env is NULL");
    assert(NULL != class && "The class name parameter is NULL");
    assert(NULL != sig && "The signature paramter is NULL");
    assert(NULL != klass && "The class name parameter is NULL");
    assert(NULL != instance && "The class instance parameter is NULL");

    *klass = (*env)->FindClass(env, class);
    assert(NULL != *klass && "Could not find the class given class");

    const jfieldID staticField =
        (*env)->GetStaticFieldID(env, *klass, "INSTANCE", sig);
    assert(NULL != staticField && "Could not find the static field for the Lambda object");

    *instance = (*env)->GetStaticObjectField(env, *klass, staticField);
    assert(NULL != instance && "Could not find the given class' static INSTANCE field");
}

inline
void
    // Finds a method on the given class
    get_method(
        JNIEnv          *env,
        const char      *name,
        const char      *sig,
        const jclass    *klass,
        jmethodID       *method
)
{
    assert(NULL != env && "The JNI env is NULL");
    assert(NULL != name && "The method name parameter is NULL");
    assert(NULL != sig && "The signature paramter is NULL");
    assert(NULL != klass && "The class name parameter is NULL");
    assert(NULL != method && "The method is parameter is NULL");

    *method = (*env)->GetMethodID(env, *klass, name, sig);

    assert(NULL != *method && "Could not find the given method in the given class");
}

inline
void
    // Finds a static method on the given class
    get_smethod(
        JNIEnv          *env,
        const char      *name,
        const char      *sig,
        const jclass    *klass,
        jmethodID       *method
)
{
    assert(NULL != env && "The JNI env is NULL");
    assert(NULL != name && "The method name parameter is NULL");
    assert(NULL != sig && "The signature paramter is NULL");
    assert(NULL != klass && "The class name parameter is NULL");
    assert(NULL != method && "The method is parameter is NULL");

    *method = (*env)->GetStaticMethodID(env, *klass, name, sig);

    assert(NULL != *method && "Could not find the given static method in the given class");
}

inline
void
    get_lambda(JNIEnv *env, lambda_o *lambda)
{
    get_sclass(env, "com/lambda/Lambda", SIG_CLASS("com/lambda/Lambda"), &lambda->klass, &lambda->instance);

    get_method(env, "getVERSION", SIG_FUNC(SIG_CLASS("java/lang/String")), &lambda->klass, &lambda->get_version_m);
    get_method(env, "getLOG", SIG_FUNC(SIG_CLASS("org/apache/logging/log4j/Logger")), &lambda->klass, &lambda->get_log_m);
    get_smethod(env, "getMc", SIG_FUNC(SIG_CLASS("net/minecraft/client/MinecraftClient")), &lambda->klass, &lambda->get_mc_m);
    get_method(env, "isDebug", SIG_FUNC(SIG_BOOLEAN), &lambda->klass, &lambda->is_debug_m);
}

inline
const char*
    // Returns a copied version string. Must be freed with ReleaseStringChars.
    lambda_version(JNIEnv *env, const lambda_o *lambda)
{
    assert(NULL != env && "The JNI env is NULL");
    assert(NULL != lambda && "The receiver struct is NULL");

    const jstring ret = (*env)->CallObjectMethod(env, lambda->instance, lambda->get_version_m);
    return (*env)->GetStringUTFChars(env, ret, 0);
}

inline
jobject
    lambda_log(JNIEnv *env, const lambda_o *lambda)
{
    assert(NULL != env && "The JNI env is NULL");
    assert(NULL != lambda && "The receiver struct is NULL");

    return (*env)->CallObjectMethod(env, lambda->instance, lambda->get_log_m);
}

inline
void*
    lambda_mc(JNIEnv *env, const lambda_o *lambda)
{
    assert(NULL != env && "The JNI env is NULL");
    assert(NULL != lambda && "The receiver struct is NULL");

    return (*env)->CallStaticObjectMethod(env, lambda->instance, lambda->get_mc_m);
}

inline
u_int8_t
    lambda_is_debug(JNIEnv *env, const lambda_o *lambda)
{
    return (*env)->CallBooleanMethod(env, lambda->instance, lambda->is_debug_m);
}

#endif
#endif
