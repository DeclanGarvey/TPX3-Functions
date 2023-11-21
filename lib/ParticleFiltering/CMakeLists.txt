
add_library(ParticleFileReaders ParticleFileReaders.cc)
target_include_directories(ParticleFileReaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(ROOT)
target_link_libraries(ParticleFileReaders  PUBLIC ROOT::Core ROOT::RIO ROOT::Net ROOT::Tree)
target_link_libraries(ParticleFileReaders PUBLIC FileNames)
target_link_libraries(ParticleFileReaders PUBLIC ParticleCluster)
target_link_libraries(ParticleFileReaders PUBLIC ClusterFunctions)
target_link_libraries(ParticleFileReaders PUBLIC AngleProject)

add_library(DirectoryReaders DirectoryReaders.cc)
target_include_directories(DirectoryReaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(ROOT)
target_link_libraries(DirectoryReaders  PUBLIC ROOT::Core ROOT::RIO ROOT::Net ROOT::Tree)

add_library(ConfigFileReader ConfigFileReader.cc)
target_include_directories(ConfigFileReader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
