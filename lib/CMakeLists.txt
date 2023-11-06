

add_subdirectory(ParticleCluster)
add_subdirectory(ClusterFunctions)
add_subdirectory(FileOperators)
add_subdirectory(Angles)


find_package(RooUnfold)
if(RooUnfold_FOUND)
	add_subdirectory(Deconvolution)
endif()

#find_library(ONNX NAMES onnxruntime)
#if(ONNX)
#	add_subdirectory(MLParticleTracking)
#else()
#	message("onnxruntime not found MLTracking can't be built")
#endif()

