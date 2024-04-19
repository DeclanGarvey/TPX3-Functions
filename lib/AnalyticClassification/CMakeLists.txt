# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
#project(FluenceStoppingPower)


add_library(AnalyticClassification MorphologicalClassification.cc)
target_include_directories(AnalyticClassification PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(AnalyticClassification PRIVATE ParticleCluster ClusterFunctions AngleProject)
