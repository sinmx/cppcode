include(ExternalData)
set(ExternalData_LINK_CONTENT MD5)
set(ExternalData_SOURCE_ROOT ${CMAKE_CURRENT_BINARY_DIR})
set(ExternalData_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR}/ExternalData)
set(input ${CMAKE_CURRENT_BINARY_DIR}/Dir)
set(output ${CMAKE_CURRENT_BINARY_DIR}/ExternalData/Dir)
set(staged "${input}/.ExternalData_MD5_c18ff9804c8deec9eaeb17063cda8b7b")
set(content "To be transformed into a content link.")
file(REMOVE ${staged})
file(REMOVE_RECURSE ${input})
file(WRITE ${input}/ToLink.txt "${content}")
ExternalData_Expand_Arguments(Data args "DATA{${input}/,REGEX:.*}")
if("x${args}" STREQUAL "x${output}")
  message(STATUS "Raw data correctly transformed to content link!")
else()
  message(FATAL_ERROR "Data reference transformed to:\n  ${args}\n"
    "but we expected:\n  ${output}")
endif()
if(EXISTS "${staged}")
  message(STATUS "Staged content exists!")
else()
  message(FATAL_ERROR "Staged content missing!")
endif()

# Expand again to check whether staged content is ignored.
ExternalData_Expand_Arguments(Data args "DATA{${input}/,REGEX:.*}")
file(STRINGS "${staged}" staged_content LIMIT_INPUT 1024)
if("${content}" STREQUAL "${staged_content}")
  message(STATUS "Staged content is correct!")
else()
  message(STATUS "Staged content is incorrect!")
endif()
if(EXISTS "${staged}.md5")
  message(FATAL_ERROR "Staged content was incorrectly re-staged!")
else()
  message(STATUS "Staged content was correctly not re-staged!")
endif()
