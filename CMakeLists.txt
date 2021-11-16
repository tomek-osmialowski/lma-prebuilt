cmake_minimum_required(VERSION 3.16)

project(lg-prebuilt VERSION 1.0.4)

add_library(liblma-prebuilt INTERFACE)

set(LG_PREBUILT_HEADERS sdk/include/)
set(LG_PREBUILT_LIBRARY_PATH sdk/lib)
set(LG_PREBUILT_LIBLMA_NAME liblma.so)
set(LG_PREBUILT_DEPENDENT_LIBRARIES) #libplayready in the future

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(VERSION_CONFIG "${GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(PROJECT_CONFIG "${GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(NAMESPACE "${PROJECT_NAME}::")

target_include_directories(liblma-prebuilt
    INTERFACE
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LG_PREBUILT_HEADERS}>
    )

target_link_libraries(liblma-prebuilt
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LG_PREBUILT_LIBRARY_PATH}/${LG_PREBUILT_LIBLMA_NAME}>
        ${LG_PREBUILT_DEPENDENT_LIBRARIES}
    )

#INSTALL
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "cmake/Config.cmake.in"
    "${PROJECT_CONFIG}"
    INSTALL_DESTINATION "${INSTALL_CONFIGDIR}"
)

# build config version into bin directory
write_basic_package_version_file(
    "${VERSION_CONFIG}"
    COMPATIBILITY AnyNewerVersion
)

# install target
install(
    TARGETS liblma-prebuilt
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

#install files
install(
    FILES
        "${VERSION_CONFIG}"
        "${PROJECT_CONFIG}"
    DESTINATION ${INSTALL_CONFIGDIR}
)

if (DEFINED VULKAN_PREBUILT_HEADERS)
    # install headers
    install(
        DIRECTORY
            ${VULKAN_PREBUILT_HEADERS}
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if (DEFINED LG_PREBUILT_LIBLMA_NAME)
    # install prebuilt libraries
    install(
        FILES
            ${LG_PREBUILT_LIBRARY_PATH}/${LG_PREBUILT_LIBLMA_NAME}
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
    )
endif()

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${NAMESPACE}"
    DESTINATION "${INSTALL_CONFIGDIR}"
)
