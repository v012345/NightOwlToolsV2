
add_library(spine_static STATIC
    Animation.c
    AnimationState.c
    AnimationStateData.c
    Atlas.c
    AtlasAttachmentLoader.c
    Attachment.c
    AttachmentLoader.c
    AttachmentVertices.cpp
    Bone.c
    BoneData.c
    BoundingBoxAttachment.c
    Cocos2dAttachmentLoader.cpp
    Event.c
    EventData.c
    IkConstraint.c
    IkConstraintData.c
    Json.c
    MeshAttachment.c
    PathAttachment.c
    PathConstraint.c
    PathConstraintData.c
    RegionAttachment.c
    Skeleton.c
    SkeletonAnimation.cpp
    SkeletonBatch.cpp
    SkeletonBinary.c
    SkeletonBounds.c
    SkeletonData.c
    SkeletonJson.c
    SkeletonRenderer.cpp
    Skin.c
    Slot.c
    SlotData.c
    TransformConstraint.c
    TransformConstraintData.c
    VertexAttachment.c
    extension.c
    spine-cocos2dx.cpp
)

# 添加头文件搜索路径
target_include_directories(spine_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/.. # LOCAL_EXPORT_C_INCLUDES
)

# 链接静态依赖库
target_link_libraries(spine_static
    PRIVATE
    cocos2dx_internal_static
)