cmake_minimum_required(VERSION 3.4.1)

# 设置源文件列表（通用部分）
set(PVMP3_SRC
    src/pvmp3_normalize.cpp
    src/pvmp3_alias_reduction.cpp
    src/pvmp3_crc.cpp
    src/pvmp3_decode_header.cpp
    src/pvmp3_decode_huff_cw.cpp
    src/pvmp3_getbits.cpp
    src/pvmp3_dequantize_sample.cpp
    src/pvmp3_framedecoder.cpp
    src/pvmp3_get_main_data_size.cpp
    src/pvmp3_get_side_info.cpp
    src/pvmp3_get_scale_factors.cpp
    src/pvmp3_mpeg2_get_scale_data.cpp
    src/pvmp3_mpeg2_get_scale_factors.cpp
    src/pvmp3_mpeg2_stereo_proc.cpp
    src/pvmp3_huffman_decoding.cpp
    src/pvmp3_huffman_parsing.cpp
    src/pvmp3_tables.cpp
    src/pvmp3_imdct_synth.cpp
    src/pvmp3_mdct_6.cpp
    src/pvmp3_dct_6.cpp
    src/pvmp3_poly_phase_synthesis.cpp
    src/pvmp3_equalizer.cpp
    src/pvmp3_seek_synch.cpp
    src/pvmp3_stereo_proc.cpp
    src/pvmp3_reorder.cpp
)

# 根据平台添加汇编或 C 实现
if(ANDROID_ABI STREQUAL "armeabi-v7a")
    enable_language(ASM)
    set_source_files_properties(
        src/asm/pvmp3_polyphase_filter_window_gcc.s
        src/asm/pvmp3_mdct_18_gcc.s
        src/asm/pvmp3_dct_9_gcc.s
        src/asm/pvmp3_dct_16_gcc.s
        PROPERTIES LANGUAGE ASM
        COMPILE_FLAGS "-marm"
    )
    list(APPEND PVMP3_SRC
        src/asm/pvmp3_polyphase_filter_window_gcc.s
        src/asm/pvmp3_mdct_18_gcc.s
        src/asm/pvmp3_dct_9_gcc.s
        src/asm/pvmp3_dct_16_gcc.s
    )
else()
    list(APPEND PVMP3_SRC
        src/pvmp3_polyphase_filter_window.cpp
        src/pvmp3_mdct_18.cpp
        src/pvmp3_dct_9.cpp
        src/pvmp3_dct_16.cpp
    )
endif()

# 定义静态库
add_library(libpvmp3dec STATIC ${PVMP3_SRC})

# 添加 include 路径（等价于 LOCAL_C_INCLUDES 和 LOCAL_EXPORT_C_INCLUDES）
target_include_directories(libpvmp3dec
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# target_compile_options(libpvmp3dec PRIVATE -Wno-implicit-const-int-float-conversion)

# target_compile_options(libpvmp3dec PRIVATE -Werror)
# target_compile_options(libpvmp3dec PRIVATE -fsanitize=signed-integer-overflow)
