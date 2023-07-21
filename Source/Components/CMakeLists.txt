target_include_directories("${PROJECT_NAME}"
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/VolumeComponents
 	${CMAKE_CURRENT_SOURCE_DIR}/ADSRComponent
        ${CMAKE_CURRENT_SOURCE_DIR}/MainComponents)

add_subdirectory(VolumeComponents)
add_subdirectory(MainComponents)
add_subdirectory(ADSRComponent)