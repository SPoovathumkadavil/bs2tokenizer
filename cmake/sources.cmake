# ---- Declare library ----

file(GLOB_RECURSE pbtokenizer_tokenizer_sources CONFIGURE_DEPENDS "src/tokenizer/*.cpp")
add_library(
    pbtokenizer_tokenizer
    ${pbtokenizer_tokenizer_sources}
)
add_library(pbtokenizer::tokenizer ALIAS pbtokenizer_tokenizer)

include(GenerateExportHeader)
generate_export_header(
    pbtokenizer_tokenizer
    BASE_NAME tokenizer
    EXPORT_FILE_NAME export/tokenizer/tokenizer_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(pbtokenizer_tokenizer PUBLIC TOKENIZER_STATIC_DEFINE)
endif()

set_target_properties(
    pbtokenizer_tokenizer PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME tokenizer
    OUTPUT_NAME tokenizer
)

target_include_directories(
    pbtokenizer_tokenizer ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    pbtokenizer_tokenizer SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(pbtokenizer_tokenizer PUBLIC cxx_std_17)

# ---- external libs ----

include(cmake/external-lib.cmake)
