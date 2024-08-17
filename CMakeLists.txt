cmake_minimum_required(VERSION 2.7 FATAL_ERROR)

project(Reconstruction)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/* .cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/* .cc)
file(GLOB MACRO_FILES
	"*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB DATA_FILES
	"*.dat"
)

file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(reconstruction reconstruction.cc ${souces} ${headers})
target_link_libraries(reconstruction ${Geant4_LIBRARIES})

add_custom_target(Reconstruction DEPENDS reconstruction)
