# 假设当前路径为 cocostudio
add_library(cocostudio_static STATIC
    CCActionFrame.cpp
    CCActionFrameEasing.cpp
    CCActionManagerEx.cpp
    CCActionNode.cpp
    CCActionObject.cpp
    CCArmature.cpp
    CCBone.cpp
    CCArmatureAnimation.cpp
    CCProcessBase.cpp
    CCTween.cpp
    CCDatas.cpp
    CCBatchNode.cpp
    CCDecorativeDisplay.cpp
    CCDisplayFactory.cpp
    CCDisplayManager.cpp
    CCSkin.cpp
    CCColliderDetector.cpp
    CCArmatureDataManager.cpp
    CCArmatureDefine.cpp
    CCDataReaderHelper.cpp
    CCSpriteFrameCacheHelper.cpp
    CCTransformHelp.cpp
    CCUtilMath.cpp
    CCComAttribute.cpp
    CCComAudio.cpp
    CCComController.cpp
    CCComRender.cpp
    CCInputDelegate.cpp
    DictionaryHelper.cpp
    LocalizationManager.cpp
    CCSGUIReader.cpp
    CCSSceneReader.cpp
    TriggerBase.cpp
    TriggerMng.cpp
    TriggerObj.cpp
    CocoLoader.cpp
    WidgetReader/NodeReader/NodeReader.cpp
    WidgetReader/SingleNodeReader/SingleNodeReader.cpp
    WidgetReader/SpriteReader/SpriteReader.cpp
    WidgetReader/ParticleReader/ParticleReader.cpp
    WidgetReader/GameMapReader/GameMapReader.cpp
    WidgetReader/ProjectNodeReader/ProjectNodeReader.cpp
    WidgetReader/ComAudioReader/ComAudioReader.cpp
    WidgetReader/WidgetReader.cpp
    WidgetReader/ButtonReader/ButtonReader.cpp
    WidgetReader/CheckBoxReader/CheckBoxReader.cpp
    WidgetReader/ImageViewReader/ImageViewReader.cpp
    WidgetReader/LayoutReader/LayoutReader.cpp
    WidgetReader/ListViewReader/ListViewReader.cpp
    WidgetReader/LoadingBarReader/LoadingBarReader.cpp
    WidgetReader/PageViewReader/PageViewReader.cpp
    WidgetReader/ScrollViewReader/ScrollViewReader.cpp
    WidgetReader/SliderReader/SliderReader.cpp
    WidgetReader/TextAtlasReader/TextAtlasReader.cpp
    WidgetReader/TextBMFontReader/TextBMFontReader.cpp
    WidgetReader/TextFieldReader/TextFieldReader.cpp
    WidgetReader/TextReader/TextReader.cpp
    WidgetReader/Node3DReader/Node3DReader.cpp
    WidgetReader/Sprite3DReader/Sprite3DReader.cpp
    WidgetReader/UserCameraReader/UserCameraReader.cpp
    WidgetReader/Particle3DReader/Particle3DReader.cpp
    WidgetReader/SkeletonReader/BoneNodeReader.cpp
    WidgetReader/SkeletonReader/SkeletonNodeReader.cpp
    WidgetReader/GameNode3DReader/GameNode3DReader.cpp
    WidgetReader/Light3DReader/Light3DReader.cpp
    ActionTimeline/CCActionTimelineCache.cpp
    ActionTimeline/CCFrame.cpp
    ActionTimeline/CCTimeLine.cpp
    ActionTimeline/CCActionTimeline.cpp
    ActionTimeline/CCActionTimelineNode.cpp
    ActionTimeline/CSLoader.cpp
    ActionTimeline/CCBoneNode.cpp
    ActionTimeline/CCSkeletonNode.cpp
    ActionTimeline/CCSkinNode.cpp
    FlatBuffersSerialize.cpp
    WidgetCallBackHandlerProtocol.cpp
    WidgetReader/ArmatureNodeReader/ArmatureNodeReader.cpp
    WidgetReader/TabControlReader/TabControlReader.cpp
    CCComExtensionData.cpp
    CocoStudio.cpp
)

# 添加头文件路径（包括导出和内部使用的）
target_include_directories(cocostudio_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/.. # 等价于 LOCAL_EXPORT_C_INCLUDES
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/WidgetReader
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/tinyxml2
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../extensions
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

# 添加 C 编译参数
target_compile_options(cocostudio_static PRIVATE -fexceptions)

# 链接依赖的静态库
target_link_libraries(cocostudio_static
    PRIVATE
    cocos_ui_static
    cocosdenshion_static
    cocos_flatbuffers_static
)