#include "lambda.h"

jboolean loaded = 0;

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
    if (strcmp(classSignature, "Lcom/lambda/core/Loader;") != 0 || loaded)
        return;

    lambdah_init(jvmti, env);
    loaded = 1;

    fprintf(stderr, "[liblambdah] Loaded class %s\n", classSignature);

    const lambda *l = get_lambda(jvmti, env);

    jobject logObj = (*env)->CallObjectMethod(env, l->instance, l->log);

    jclass loggerClass =
        (*env)->FindClass(env, "org/apache/logging/log4j/Logger");

    jmethodID log =
        (*env)->GetMethodID(env, loggerClass, "info", SIG_FUNC(SIG_VOID, SIG_CLASS("java/lang/String")));

    const jstring str =
        (*env)->NewStringUTF(env, "transfem coding :3");

    (*env)->CallVoidMethod(env, logObj, log, str);
}

JNIEXPORT
jint
JNICALL
    // This entrypoint is called during the load of the VM.
    // In this entrypoint, not all the functions of the VM can be used.
    Agent_OnLoad(
        JavaVM* vm,
        char* options,
        void* reserved
) {
    fprintf(stderr, "[liblambdah] Agent loaded\n");

    jvmtiEnv* jvmti = NULL;
    (*vm)->GetEnv(vm, (void**)&jvmti, JVMTI_VERSION_21);

    jvmtiEventCallbacks cb = {0};
    cb.ClassPrepare = cbClassPrepare;

    (*jvmti)->SetEventCallbacks(jvmti, &cb, sizeof(cb));
    (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE,
        JVMTI_EVENT_CLASS_PREPARE, NULL);

    return JNI_OK;
}