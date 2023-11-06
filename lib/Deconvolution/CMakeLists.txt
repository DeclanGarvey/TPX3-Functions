add_library(Deconvolution RooUnfoldResponseConstructorClass.cc ResponseConstruction.cc Deconvolution.cc)
target_compile_features(Deconvolution PUBLIC cxx_std_17)
target_include_directories(Deconvolution PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Deconvolution PRIVATE DirectoryReaders)
target_link_libraries(Deconvolution PRIVATE ParticleFileReaders)

#find_package(ROOT)
find_package( ROOT)
#target_link_libraries(UnfoldingProject PRIVATE ${ROOT_LIBRARIES})
target_link_libraries(Deconvolution PRIVATE  ROOT::Tree ROOT::Net ROOT::Core ROOT::Unfold ROOT::Matrix ROOT::Hist ROOT::RIO ROOT::MathCore ROOT::Physics ROOT::RooFitCore ROOT::RooFit ROOT::HistFactory ROOT::Graf ROOT::Postscript ROOT::Gpad ROOT::XMLParser ROOT::RooBatchCompute)

#include(CMakePrintHelpers)

#cmake_print_variables(RooUnfold_FOUND)
find_package(RooUnfold)
#cmake_print_variables(RooUnfold_FOUND)
target_link_libraries(Deconvolution  PRIVATE  RooUnfold)

