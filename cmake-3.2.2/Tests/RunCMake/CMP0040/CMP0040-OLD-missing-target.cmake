cmake_policy(SET CMP0040 OLD)

add_custom_command(TARGET foobar PRE_BUILD
  COMMAND "${CMAKE_COMMAND} -E echo hello world"
)
