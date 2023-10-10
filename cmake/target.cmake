add_library(${DEPLOY_LIB_NAME} SHARED ${WATER_LEVEL_SRC})
target_include_directories(${DEPLOY_LIB_NAME} PUBLIC ${CUDA_INCLUDE_DIR})
target_link_libraries(${DEPLOY_LIB_NAME} PUBLIC ${DEP_LIBS})

add_executable(${MAIN_NAME} ${WATER_LEVEL_HEADER} ${WATER_LEVEL_MAIN})
target_link_libraries(${MAIN_NAME} PUBLIC ${DEP_LIBS} ${DEPLOY_LIB_NAME})


