if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/tokenizer-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package tokenizer)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT tokenizer_Development
)

install(
    TARGETS tokenizer_tokenizer
    EXPORT tokenizerTargets
    RUNTIME #
    COMPONENT tokenizer_Runtime
    LIBRARY #
    COMPONENT tokenizer_Runtime
    NAMELINK_COMPONENT tokenizer_Development
    ARCHIVE #
    COMPONENT tokenizer_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    tokenizer_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE tokenizer_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(tokenizer_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${tokenizer_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT tokenizer_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${tokenizer_INSTALL_CMAKEDIR}"
    COMPONENT tokenizer_Development
)

install(
    EXPORT tokenizerTargets
    NAMESPACE tokenizer::
    DESTINATION "${tokenizer_INSTALL_CMAKEDIR}"
    COMPONENT tokenizer_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
