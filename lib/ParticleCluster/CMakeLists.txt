
add_library(ParticleCluster Particle.cc PixelStructure.cc)
target_include_directories(ParticleCluster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
