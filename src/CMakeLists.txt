
ADD_LIBRARY(lambda SHARED lambda.c lambda.h)

set(installable_libs lambda)
install(TARGETS ${installable_libs}
        DESTINATION lib
        EXPORT LibLambda)
install(FILES lambda.h DESTINATION include)
