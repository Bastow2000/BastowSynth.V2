target_sources("${PROJECT_NAME}"
        PRIVATE
        PluginEditor.cpp
        BASPluginProcessor.cpp)

target_include_directories("${PROJECT_NAME}"
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Components
        ${CMAKE_CURRENT_SOURCE_DIR}/LAF
        ${CMAKE_CURRENT_SOURCE_DIR}/ADSR
        ${CMAKE_CURRENT_SOURCE_DIR}/DSP)

add_subdirectory(Components)
add_subdirectory(LAF)
add_subdirectory(DSP)
add_subdirectory(ADSR)