
find_library(ONNX_NAME onnxruntime PATHS /usr/local/lib)
find_path(ONNX_INCLUDE_DIR onnxruntime)
if(ONNX_INCLUDE_DIR)
	add_library(MLClassification ElectronProtonClassifier.cc ProtonSpectroscope.cc MLClassification.cc)
	
	target_include_directories(MLClassification PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	find_package( ROOT)
	target_link_libraries(MLClassification PRIVATE  ROOT::Tree ROOT::Net ROOT::Core ROOT::Unfold ROOT::Matrix ROOT::Hist ROOT::RIO ROOT::MathCore ROOT::Physics ROOT::RooFitCore ROOT::RooFit ROOT::HistFactory ROOT::Graf ROOT::Postscript ROOT::Gpad ROOT::XMLParser ROOT::RooBatchCompute)
	target_link_libraries(MLClassification PUBLIC GeneralisedONNXProject)
	target_link_libraries(MLClassification PRIVATE ParticleCluster)
	target_link_libraries(MLClassification PRIVATE ClusterFunctions)
	target_link_libraries(MLClassification PRIVATE DirectoryReaders)
	target_link_libraries(MLClassification PRIVATE ParticleFileReaders)
	target_link_libraries(MLClassification PRIVATE ParticleFiltering)
	
else()
	message("onnxruntime not found MLTracking can't be built")
endif()


