add_subdirectory(ParticleCluster)
add_subdirectory(ClusterFunctions)
add_subdirectory(GeneralisedONNXModel)
add_subdirectory(Angles)
add_subdirectory(FileOperators)
add_subdirectory(ParticleFiltering)

find_package(RooUnfold)
if(RooUnfold_FOUND)
	add_subdirectory(Deconvolution)
endif()

add_subdirectory(AnalyticClassification)
