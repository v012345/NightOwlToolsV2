add_library(cocos_ui_static STATIC
    UIWidget.cpp
    UILayout.cpp
    UILayoutParameter.cpp
    UILayoutManager.cpp
    CocosGUI.cpp
    UIHelper.cpp
    UIListView.cpp
    UIPageView.cpp
    UIPageViewIndicator.cpp
    UIScrollView.cpp
    UIScrollViewBar.cpp
    UIButton.cpp
    UIAbstractCheckButton.cpp
    UICheckBox.cpp
    UIRadioButton.cpp
    UIImageView.cpp
    UIText.cpp
    UITextAtlas.cpp
    UITextBMFont.cpp
    UILoadingBar.cpp
    UISlider.cpp
    UITextField.cpp
    UIRichText.cpp
    UIHBox.cpp
    UIVBox.cpp
    UIRelativeBox.cpp
    UIVideoPlayer-android.cpp
    UIDeprecated.cpp
    UIScale9Sprite.cpp
    UIWebView.cpp
    UIWebViewImpl-android.cpp
    UIEditBox/UIEditBox.cpp
    UIEditBox/UIEditBoxImpl-android.cpp
    UIEditBox/UIEditBoxImpl-common.cpp
    UILayoutComponent.cpp
    UITabControl.cpp)

target_link_libraries(cocos_ui_static PRIVATE
    cocos_extension_static
)

target_include_directories(cocos_ui_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../editor-support
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external # 等价于 LOCAL_EXPORT_C_INCLUDES
)
