add_library(cocos_flatbuffers_static STATIC
    flatc.cpp
    idl_gen_cpp.cpp
    idl_gen_fbs.cpp
    idl_gen_general.cpp
    idl_gen_go.cpp
    idl_gen_text.cpp
    idl_parser.cpp
)
target_compile_options(cocos_flatbuffers_static PRIVATE -fexceptions)
target_include_directories(cocos_flatbuffers_static PUBLIC ..)
