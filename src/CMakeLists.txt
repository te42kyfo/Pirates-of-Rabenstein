include_directories(${RABENSTEIN_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
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

target_link_libraries(
  rabenstein
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  OpenCL)
