add_library(cocos_extension_static STATIC
    GUI/CCControlExtension/CCControl.cpp
    GUI/CCControlExtension/CCControlButton.cpp
    GUI/CCControlExtension/CCControlColourPicker.cpp
    GUI/CCControlExtension/CCControlHuePicker.cpp
    GUI/CCControlExtension/CCControlPotentiometer.cpp
    GUI/CCControlExtension/CCControlSaturationBrightnessPicker.cpp
    GUI/CCControlExtension/CCControlSlider.cpp
    GUI/CCControlExtension/CCControlStepper.cpp
    GUI/CCControlExtension/CCControlSwitch.cpp
    GUI/CCControlExtension/CCControlUtils.cpp
    GUI/CCControlExtension/CCInvocation.cpp
    GUI/CCScrollView/CCScrollView.cpp
    GUI/CCScrollView/CCTableView.cpp
    GUI/CCScrollView/CCTableViewCell.cpp
)

target_link_libraries(cocos_extension_static PRIVATE
    cocos2dx_internal_static
    box2d_static
)

# 开启 C++ 异常支持
target_compile_options(cocos_extension_static PRIVATE -fexceptions)

# 添加头文件搜索路径
target_include_directories(cocos_extension_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # 相当于 $(LOCAL_PATH)
    ${CMAKE_CURRENT_SOURCE_DIR}/.. # 上一级目录
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI/CCControlExtension
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI/CCScrollView
)