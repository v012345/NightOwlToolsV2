# 添加库（静态）
add_library(libvorbisidec STATIC
    Tremolo/bitwise.c
    Tremolo/codebook.c
    Tremolo/dsp.c
    Tremolo/floor0.c
    Tremolo/floor1.c
    Tremolo/floor_lookup.c
    Tremolo/framing.c
    Tremolo/mapping0.c
    Tremolo/mdct.c
    Tremolo/misc.c
    Tremolo/res012.c
    Tremolo/treminfo.c
    Tremolo/vorbisfile.c
)

# 添加 include 路径
target_include_directories(libvorbisidec
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Tremolo
)

# 添加编译选项
target_compile_options(libvorbisidec
    PRIVATE
    -DONLY_C
    -O2
)
