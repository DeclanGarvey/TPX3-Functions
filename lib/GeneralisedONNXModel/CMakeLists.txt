

find_library(ONNX_NAME onnxruntime PATHS /usr/local/lib)
find_path(ONNX_INCLUDE_DIR onnxruntime)
if(ONNX_INCLUDE_DIR)
	message("-- onnxruntime - found")
	
	add_library(GeneralisedONNXProject GeneralisedONNXModel.cc)


	target_include_directories(GeneralisedONNXProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(GeneralisedONNXProject PRIVATE AngleProject)
	target_link_libraries(GeneralisedONNXProject PRIVATE ParticleCluster)
	target_link_libraries(GeneralisedONNXProject PRIVATE ClusterFunctions)
	
	
	target_link_libraries(GeneralisedONNXProject PRIVATE onnxruntime)
	
	target_include_directories(GeneralisedONNXProject PUBLIC ${ONNX_INCLUDE_DIR}/onnxruntime)
	target_include_directories(GeneralisedONNXProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
else()
	message("onnxruntime not found MLTracking can't be built")
endif()


