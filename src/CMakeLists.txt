
ADD_LIBRARY(lambdah SHARED lambda.c lambda.h)

set(installable_libs lambdah)
install(TARGETS ${installable_libs}
        DESTINATION lib
        EXPORT LibLambdah)
install(FILES lambda.h DESTINATION include)
