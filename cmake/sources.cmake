# ---- Declare library ----

file(GLOB_RECURSE tokenizer_tokenizer_sources CONFIGURE_DEPENDS "src/tokenizer/*.cpp")
add_library(
    tokenizer_tokenizer
    ${tokenizer_tokenizer_sources}
)
add_library(tokenizer::tokenizer ALIAS tokenizer_tokenizer)

include(GenerateExportHeader)
generate_export_header(
    tokenizer_tokenizer
    BASE_NAME tokenizer
    EXPORT_FILE_NAME export/tokenizer/tokenizer_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(tokenizer_tokenizer PUBLIC TOKENIZER_STATIC_DEFINE)
endif()

set_target_properties(
    tokenizer_tokenizer PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME tokenizer
    OUTPUT_NAME tokenizer
)

target_include_directories(
    tokenizer_tokenizer ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    tokenizer_tokenizer SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(tokenizer_tokenizer PUBLIC cxx_std_17)

# ---- external libs ----

include(cmake/external-lib.cmake)
