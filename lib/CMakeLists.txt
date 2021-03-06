set(PROMETHEUS_SOURCES 
  check_names.cc
  counter.cc
  counter_builder.cc
  gauge.cc
  gauge_builder.cc
  histogram.cc
  histogram_builder.cc
  registry.cc
  summary.cc
  summary_builder.cc
  text_serializer.cc
	)
if (BUILD_CIVETWEB)
	set(PROMETHEUS_SOURCES ${PROMETHEUS_SOURCES} 
		exposer.cc
		handler.cc
		$<TARGET_OBJECTS:civetweb>
		)
endif()

add_library(prometheus-cpp
	${PROMETHEUS_SOURCES}
)

target_link_libraries(prometheus-cpp PRIVATE ${CMAKE_THREAD_LIBS_INIT})
if(UNIX AND NOT APPLE)
  target_link_libraries(prometheus-cpp PRIVATE rt)
endif()

target_include_directories(prometheus-cpp PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
target_include_directories(prometheus-cpp PRIVATE ${CIVETWEB_INCLUDE_DIR})

install(TARGETS prometheus-cpp EXPORT prometheus-cpp-targets
  RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION  ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION  ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ../include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
  include(CMakePackageConfigHelpers)
  configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/prometheus-cpp-config.cmake.in prometheus-cpp-config.cmake
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )
  install(EXPORT prometheus-cpp-targets DESTINATION ${CMAKECONFIG_INSTALL_DIR} NAMESPACE prometheus-cpp::)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/prometheus-cpp-config.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})
