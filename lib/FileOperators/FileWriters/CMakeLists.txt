add_library(ParticleFileWriters FileWriterFetcher.cc ParticleFileWriters.cc MLParticleFileWriters.cc)

target_include_directories(ParticleFileWriters PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(ROOT)
target_link_libraries(ParticleFileWriters  PRIVATE ROOT::Core ROOT::RIO ROOT::Net ROOT::Tree)

target_link_libraries(ParticleFileWriters PRIVATE FileNames)
target_link_libraries(ParticleFileWriters PRIVATE ParticleCluster)
target_link_libraries(ParticleFileWriters PRIVATE ClusterFunctions)
target_link_libraries(ParticleFileWriters PRIVATE AngleProject)
target_link_libraries(ParticleFileWriters PRIVATE MLAngleProject)
target_link_libraries(ParticleFileWriters PRIVATE AnalyticClassification)

