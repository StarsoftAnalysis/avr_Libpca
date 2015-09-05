file(GLOB SOURCES *.c)
add_library(pca STATIC ${SOURCES})

install(TARGETS pca DESTINATION lib)
