cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "BastowSynth")
#set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")

set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for macOS")  # Updated architecture for M1 Macs
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum macOS version required")

project("${PROJECT_NAME}" VERSION 2.0.0)

set(formats "VST3;Standalone")

if(APPLE)
    list(APPEND formats "AU")
endif()

#add_subdirectory(JUCE)

add_subdirectory(External/JUCE)

juce_add_plugin("${PROJECT_NAME}"
    VERSION "2.0.0"
    COMPANY_NAME "Bastow"
    COMPANY_WEBSITE "https://bastow.weebly.com/"
    COMPANY_EMAIL "alex.bastow30@gmail.com"
    PLUGIN_MANUFACTURER_CODE "Btwo"
    PLUGIN_CODE "BAST"
    FORMATS ${formats}
    COPY_PLUGIN_AFTER_BUILD TRUE
    IS_SYNTH TRUE     
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    PRODUCT_NAME "${PROJECT_NAME}"
    PLUGIN_NAME "BastowSynth")

juce_generate_juce_header("${PROJECT_NAME}")

add_subdirectory(Source)

target_include_directories("${PROJECT_NAME}"
    PRIVATE
        /Users/bastow_boii/Desktop/BastowSynth.V2/Source)

# Replace with the appropriate source directory path

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_20)

target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(BinaryData
    SOURCES
	Assets/Skeleton.png
	Assets/Components/V_C1.png
	Assets/Components/V_C2.png
	Assets/Components/V_C3.png
	Assets/Components/M_C.png
	Assets/Components/ADSR_C.png
	Assets/Buttons/Normal/B1_Norm.png
	Assets/Buttons/Normal/B2_Norm.png
	Assets/Buttons/Normal/B3_Norm.png
	Assets/Buttons/Overlay/B1_Over.png
	Assets/Buttons/Overlay/B2_Over.png
	Assets/Buttons/Overlay/B3_Over.png
	Assets/Buttons/Off/B1_Off.png
	Assets/Buttons/Off/B2_Off.png
	Assets/Buttons/Off/B3_Off.png
	Resources/AvenirBook.ttf
	Resources/AvenirRoman.ttf
	Resources/AvenirBlack.ttf
	)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
	BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)