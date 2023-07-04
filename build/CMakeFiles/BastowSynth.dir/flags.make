# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# compile CXX with /usr/bin/clang++
CXX_DEFINES = -DDEBUG=1 -DJUCE_DISPLAY_SPLASH_SCREEN=0 -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_MODULE_AVAILABLE_juce_audio_basics=1 -DJUCE_MODULE_AVAILABLE_juce_audio_devices=1 -DJUCE_MODULE_AVAILABLE_juce_audio_formats=1 -DJUCE_MODULE_AVAILABLE_juce_audio_processors=1 -DJUCE_MODULE_AVAILABLE_juce_audio_utils=1 -DJUCE_MODULE_AVAILABLE_juce_core=1 -DJUCE_MODULE_AVAILABLE_juce_data_structures=1 -DJUCE_MODULE_AVAILABLE_juce_dsp=1 -DJUCE_MODULE_AVAILABLE_juce_events=1 -DJUCE_MODULE_AVAILABLE_juce_graphics=1 -DJUCE_MODULE_AVAILABLE_juce_gui_basics=1 -DJUCE_MODULE_AVAILABLE_juce_gui_extra=1 -DJUCE_SHARED_CODE=1 -DJUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone -DJUCE_TARGET_HAS_BINARY_DATA=1 -DJUCE_USE_CURL=0 -DJUCE_VST3_CAN_REPLACE_VST2=0 -DJUCE_WEB_BROWSER=0 -DJucePlugin_AAXCategory=2048 -DJucePlugin_AAXDisableBypass=0 -DJucePlugin_AAXDisableMultiMono=0 -DJucePlugin_AAXIdentifier=com.Bastow.BastowSynth -DJucePlugin_AAXManufacturerCode=JucePlugin_ManufacturerCode -DJucePlugin_AAXProductId=JucePlugin_PluginCode -DJucePlugin_ARACompatibleArchiveIDs=\"\" -DJucePlugin_ARAContentTypes=0 -DJucePlugin_ARADocumentArchiveID=\"com.Bastow.BastowSynth.aradocumentarchive.1\" -DJucePlugin_ARAFactoryID=\"com.Bastow.BastowSynth.arafactory.2.0.0\" -DJucePlugin_ARATransformationFlags=0 -DJucePlugin_AUExportPrefix=BastowSynthAU -DJucePlugin_AUExportPrefixQuoted=\"BastowSynthAU\" -DJucePlugin_AUMainType="'aumu'" -DJucePlugin_AUManufacturerCode=JucePlugin_ManufacturerCode -DJucePlugin_AUSubType=JucePlugin_PluginCode -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_AU=1 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_LV2=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJucePlugin_Build_VST3=1 -DJucePlugin_Build_VST=0 -DJucePlugin_CFBundleIdentifier=com.Bastow.BastowSynth -DJucePlugin_Desc=\"BastowSynth\" -DJucePlugin_EditorRequiresKeyboardFocus=0 -DJucePlugin_Enable_ARA=0 -DJucePlugin_IsMidiEffect=0 -DJucePlugin_IsSynth=1 -DJucePlugin_Manufacturer=\"Bastow\" -DJucePlugin_ManufacturerCode=0x4274776f -DJucePlugin_ManufacturerEmail=\"alex.bastow30@gmail.com\" -DJucePlugin_ManufacturerWebsite=\"https://bastow.weebly.com/\" -DJucePlugin_Name=\"BastowSynth\" -DJucePlugin_PluginCode=0x42415354 -DJucePlugin_ProducesMidiOutput=1 -DJucePlugin_VSTCategory=kPlugCategSynth -DJucePlugin_VSTNumMidiInputs=16 -DJucePlugin_VSTNumMidiOutputs=16 -DJucePlugin_VSTUniqueID=JucePlugin_PluginCode -DJucePlugin_Version=2.0.0 -DJucePlugin_VersionCode=0x20000 -DJucePlugin_VersionString=\"2.0.0\" -DJucePlugin_Vst3Category="\"Instrument|Synth\"" -DJucePlugin_WantsMidiInput=1 -D_DEBUG=1

CXX_INCLUDES = -I/Users/bastow_boii/Desktop/BastowSynth.V2/build/BastowSynth_artefacts/JuceLibraryCode -I/Users/bastow_boii/Desktop/BastowSynth.V2/Source/VolumeComponents -I/Users/bastow_boii/Desktop/BastowSynth.V2/Source/LAF -I/Users/bastow_boii/Desktop/BastowSynth.V2/Source/MainComponents -I/Users/bastow_boii/Desktop/BastowSynth.V2/Source/DSP -I/Users/bastow_boii/Desktop/BastowSynth.V2/Source -I/Users/bastow_boii/Desktop/BastowSynth.V2/build/juce_binarydata_BinaryData/JuceLibraryCode -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2 -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sratom -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv -I/Users/bastow_boii/Desktop/BastowSynth.V2/External/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src

CXX_FLAGSarm64 = -g -std=gnu++20 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.3.sdk -mmacosx-version-min=10.11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -g -O0 -Wall -Wshadow-all -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion -Wconditional-uninitialized -Wconstant-conversion -Wsign-conversion -Wbool-conversion -Wextra-semi -Wunreachable-code -Wcast-align -Wshift-sign-overflow -Wmissing-prototypes -Wnullable-to-nonnull-conversion -Wno-ignored-qualifiers -Wswitch-enum -Wpedantic -Wdeprecated -Wzero-as-null-pointer-constant -Wunused-private-field -Woverloaded-virtual -Wreorder -Winconsistent-missing-destructor-override

CXX_FLAGS = -g -std=gnu++20 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.3.sdk -mmacosx-version-min=10.11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -g -O0 -Wall -Wshadow-all -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion -Wconditional-uninitialized -Wconstant-conversion -Wsign-conversion -Wbool-conversion -Wextra-semi -Wunreachable-code -Wcast-align -Wshift-sign-overflow -Wmissing-prototypes -Wnullable-to-nonnull-conversion -Wno-ignored-qualifiers -Wswitch-enum -Wpedantic -Wdeprecated -Wzero-as-null-pointer-constant -Wunused-private-field -Woverloaded-virtual -Wreorder -Winconsistent-missing-destructor-override

