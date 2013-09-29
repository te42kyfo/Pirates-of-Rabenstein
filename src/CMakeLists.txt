include_directories(${RABENSTEIN_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
link_directories( ${RABENSTEIN_INCLUDE_DIR}/OpenCLHelper/)
find_package(OpenGL REQUIRED)


find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTOPENCL TRUE)

set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTMAIN TRUE)

file(GLOB SRCS
  *.cpp
  )

file(GLOB MOC_HEADERS
  ${RABENSTEIN_INCLUDE_DIR}/*.hpp
  )

QT4_WRAP_CPP(MOC
  ${MOC_HEADERS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_BINARY_DIR})

add_executable(rabenstein
  ${SRCS}
  ${MOC}
  )

SET( CLEW_LIBRARY ${RABENSTEIN_INCLUDE_DIR}/OpenClHelper/libclew.so)
SET( OPENCLHELPER_LIBRARY ${RABENSTEIN_INCLUDE_DIR}/OpenClHelper/libOpenCLHelper.so)

target_link_libraries(
  rabenstein
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  OpenCL
  GLEW
  ${CLEW_LIBRARY}
  ${OPENCLHELPER_LIBRARY} )
 
