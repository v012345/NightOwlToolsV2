
add_library(recast_static STATIC
    DebugUtils/DebugDraw.cpp
    DebugUtils/DetourDebugDraw.cpp
    Detour/DetourAlloc.cpp
    Detour/DetourCommon.cpp
    Detour/DetourNavMesh.cpp
    Detour/DetourNavMeshBuilder.cpp
    Detour/DetourNavMeshQuery.cpp
    Detour/DetourNode.cpp
    DetourCrowd/DetourCrowd.cpp
    DetourCrowd/DetourLocalBoundary.cpp
    DetourCrowd/DetourObstacleAvoidance.cpp
    DetourCrowd/DetourPathCorridor.cpp
    DetourCrowd/DetourPathQueue.cpp
    DetourCrowd/DetourProximityGrid.cpp
    DetourTileCache/DetourTileCache.cpp
    DetourTileCache/DetourTileCacheBuilder.cpp
    fastlz/fastlz.c
)

target_include_directories(recast_static PUBLIC ..)