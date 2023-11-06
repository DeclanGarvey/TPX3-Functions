# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
#project(FluenceStoppingPower)


add_library(ClusterFunctions FluenceStoppingPower.cc ClusterFeatures.cc FittingVariables.cc)
target_include_directories(ClusterFunctions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ClusterFunctions PRIVATE ParticleCluster ParticleFileReaders AngleProject)

find_package(ROOT)
target_link_libraries(ClusterFunctions PRIVATE ROOT::RIO ROOT::Net ROOT::Hist)
