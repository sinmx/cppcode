
add_library(iface INTERFACE)
target_sources(iface INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/empty_1.cpp")

install(TARGETS iface EXPORT exp)
install(EXPORT exp DESTINATION cmake)
