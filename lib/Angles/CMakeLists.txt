
add_library(AngleProject ReadAngles.cc AngleCalulationFunctions.cc)


target_include_directories(AngleProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AngleProject PRIVATE ParticleCluster)
target_link_libraries(AngleProject PRIVATE ClusterFunctions)
target_link_libraries(AngleProject PRIVATE FileNames)
target_link_libraries(AngleProject PRIVATE ParticleFileReaders)




find_library(ONNX_NAME onnxruntime PATHS /usr/local/lib)
find_path(ONNX_INCLUDE_DIR onnxruntime)
if(ONNX_INCLUDE_DIR)
	message("-- onnxruntime - found")
	message(ONNX_NAME)
	add_library(MLAngleProject RandomForrestTheta.cc)
	
	
	target_link_libraries(MLAngleProject PRIVATE onnxruntime)
	
	target_include_directories(MLAngleProject PUBLIC ${ONNX_INCLUDE_DIR}/onnxruntime)
	target_include_directories(MLAngleProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	
	target_link_libraries(MLAngleProject PRIVATE ParticleCluster)
	target_link_libraries(MLAngleProject PRIVATE ClusterFunctions)
	
else()
	message("onnxruntime not found MLTracking can't be built")
endif()


