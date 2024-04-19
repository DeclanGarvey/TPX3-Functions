add_executable(AngleCalculator AngleCalculator.cc)
target_link_libraries(AngleCalculator PRIVATE AngleProject)
target_link_libraries(AngleCalculator PRIVATE FileNames)
set_target_properties(AngleCalculator PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")


add_executable(FileConvertor FileConvertor.cc)
target_link_libraries(FileConvertor PRIVATE FileNames)
target_link_libraries(FileConvertor PRIVATE FileConvertion)
set_target_properties(FileConvertor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")

find_package(RooUnfold)
if(RooUnfold_FOUND)
	add_executable(ResponseConstructor ResponseConstuctor.cc)
	target_link_libraries(ResponseConstructor PRIVATE Deconvolution)
	target_link_libraries(ResponseConstructor PRIVATE FileNames)
	set_target_properties(ResponseConstructor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")

	add_executable(Deconvoluter Deconvoluter.cc)
	target_link_libraries(Deconvoluter PRIVATE Deconvolution)
	target_link_libraries(Deconvoluter PRIVATE FileNames)
	set_target_properties(Deconvoluter PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")
endif()


find_library(ONNX_NAME onnxruntime PATHS /usr/local/lib)
find_path(ONNX_INCLUDE_DIR onnxruntime)
if(ONNX_INCLUDE_DIR)

	add_executable(MLClassifier MLClassifier.cc)
	target_link_libraries(MLClassifier PRIVATE MLClassification)
	target_link_libraries(MLClassifier PRIVATE FileNames)
	set_target_properties(MLClassifier PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")
endif()

#add_executable(MLTracking MLTracking.cc)
#target_link_libraries(MLTracking PRIVATE MLParticleTracking)
