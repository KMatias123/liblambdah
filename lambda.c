#include "lambda.h"

static
void
JNICALL
    // jvmtiEventClassPrepare is the stage in which a class' field and methods are loaded.
    // It is fired after jvmtiEventClassLoad and everything in the class can be used.
    cbClassPrepare(
        jvmtiEnv* jvmti,
        JNIEnv* env,
        jthread thread,
        jclass klass
)
{
    char* classSignature = NULL;
    (*jvmti)->GetClassSignature(jvmti, klass, &classSignature, NULL);

    // When this class is loaded, Lambda is ready
    if (strcmp(classSignature, "Lcom/lambda/core/Loader;") != 0)
        return;

    printf("[JVMIT] Loaded class %s\n", classSignature);

    lambda_o lambda = {0};
    get_lambda(env, &lambda);

    const jobject logObj = lambda_log(env, &lambda);

    const jclass loggerClass =
        (*env)->FindClass(env, "org/apache/logging/log4j/Logger");

    const jmethodID log =
        (*env)->GetMethodID(env, loggerClass, "info", SIG_FUNC(SIG_VOID, SIG_CLASS("java/lang/String")));

    const jstring str =
        (*env)->NewStringUTF(env, "transfem coding :3");

    (*env)->CallVoidMethod(env, logObj, log, str);
}

JNIEXPORT
jint
JNICALL
    // This entrypoint is called during the live phase of the VM. In this entrypoint,
    // all the functions in the jvm can be used.
    Agent_OnAttach(
        JavaVM* vm,
        char* options,
        void* reserved
) {
    printf("[JVMTI] Agent loaded\n");

    jvmtiEnv* jvmti = NULL;
    (*vm)->GetEnv(vm, (void**)&jvmti, JVMTI_VERSION_21);

    // jvmtiCapabilities caps = {0};
    // caps.can_generate_all_class_hook_events = 1;
    // (*jvmti)->AddCapabilities(jvmti, &caps);

    jvmtiEventCallbacks cb = {0};
    cb.ClassPrepare = cbClassPrepare;

    (*jvmti)->SetEventCallbacks(jvmti, &cb, sizeof(cb));
    (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE,
        JVMTI_EVENT_CLASS_PREPARE, NULL);

    return JNI_OK;
}