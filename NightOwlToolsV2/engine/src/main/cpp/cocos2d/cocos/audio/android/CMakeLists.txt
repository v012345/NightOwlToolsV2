
add_library(audioengine_static STATIC
    AudioEngine-inl.cpp
    ../AudioEngine.cpp
    CCThreadPool.cpp
    AssetFd.cpp
    AudioDecoder.cpp
    AudioDecoderProvider.cpp
    AudioDecoderSLES.cpp
    AudioDecoderOgg.cpp
    AudioDecoderMp3.cpp
    AudioDecoderWav.cpp
    AudioPlayerProvider.cpp
    AudioResampler.cpp
    AudioResamplerCubic.cpp
    PcmBufferProvider.cpp
    PcmAudioPlayer.cpp
    UrlAudioPlayer.cpp
    PcmData.cpp
    AudioMixerController.cpp
    AudioMixer.cpp
    PcmAudioService.cpp
    Track.cpp
    audio_utils/format.c
    audio_utils/minifloat.cpp
    audio_utils/primitives.c
    utils/Utils.cpp
    mp3reader.cpp
    tinysndfile.cpp
)

target_include_directories(audioengine_static
    PUBLIC # 让依赖者也能用
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/android
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/android-specific
)

target_link_libraries(audioengine_static
    PUBLIC
    OpenSLES
    PRIVATE
    libvorbisidec
    libpvmp3dec
)

# ###################################
# cocosdenshion_static
# ###################################
add_library(cocosdenshion_static STATIC
    cddSimpleAudioEngine.cpp
    ccdandroidUtils.cpp
    jni/cddandroidAndroidJavaEngine.cpp
)

# 设置 include 路径
target_include_directories(cocosdenshion_static
    PUBLIC # 让依赖者也能用
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/android
)

# 链接依赖的静态库模块（你需要另一个地方先 add_library(audioengine_static ...)）
target_link_libraries(cocosdenshion_static
    PRIVATE
    audioengine_static
)