include(RunCMake)

# Default case parameters.
set(CASE_DROP_METHOD "http")
set(CASE_DROP_SITE "-no-site-")
set(CASE_CTEST_SUBMIT_ARGS "")

function(run_ctest CASE_NAME)
  configure_file(${RunCMake_SOURCE_DIR}/test.cmake.in
                 ${RunCMake_BINARY_DIR}/${CASE_NAME}/test.cmake @ONLY)
  configure_file(${RunCMake_SOURCE_DIR}/CTestConfig.cmake.in
                 ${RunCMake_BINARY_DIR}/${CASE_NAME}/CTestConfig.cmake @ONLY)
  configure_file(${RunCMake_SOURCE_DIR}/CMakeLists.txt.in
                 ${RunCMake_BINARY_DIR}/${CASE_NAME}/CMakeLists.txt @ONLY)
  run_cmake_command(${CASE_NAME} ${CMAKE_CTEST_COMMAND}
    -C Debug
    -S ${RunCMake_BINARY_DIR}/${CASE_NAME}/test.cmake
    -V
    --output-log ${RunCMake_BINARY_DIR}/${CASE_NAME}-build/testOutput.log
    ${ARGN}
    )
endfunction()

#-----------------------------------------------------------------------------
# Test bad argument combinations.

function(run_ctest_submit CASE_NAME)
  set(CASE_CTEST_SUBMIT_ARGS "${ARGN}")
  run_ctest(${CASE_NAME})
endfunction()

run_ctest_submit(BadArg bad-arg)
run_ctest_submit(BadPARTS PARTS bad-part)
run_ctest_submit(BadFILES FILES bad-file)
run_ctest_submit(RepeatRETURN_VALUE RETURN_VALUE res RETURN_VALUE res)
run_ctest_submit(PARTSCDashUpload PARTS Configure CDASH_UPLOAD)
run_ctest_submit(PARTSCDashUploadType PARTS Configure CDASH_UPLOAD_TYPE)
run_ctest_submit(CDashUploadPARTS CDASH_UPLOAD bad-upload PARTS)
run_ctest_submit(CDashUploadFILES CDASH_UPLOAD bad-upload FILES)
run_ctest_submit(CDashUploadRETRY_COUNT CDASH_UPLOAD bad-upload RETRY_COUNT)
run_ctest_submit(CDashUploadRETRY_DELAY CDASH_UPLOAD bad-upload RETRY_DELAY)
run_ctest_submit(CDashUploadNone CDASH_UPLOAD)

function(run_ctest_CDashUploadFTP)
  set(CASE_DROP_METHOD ftp)
  run_ctest_submit(CDashUploadFTP CDASH_UPLOAD ${CMAKE_CURRENT_LIST_FILE})
endfunction()
run_ctest_CDashUploadFTP()

#-----------------------------------------------------------------------------
# Test failed drops by various protocols

function(run_ctest_submit_FailDrop CASE_DROP_METHOD)
  run_ctest(FailDrop-${CASE_DROP_METHOD})
endfunction()

run_ctest_submit_FailDrop(cp)
run_ctest_submit_FailDrop(ftp)
run_ctest_submit_FailDrop(http)
run_ctest_submit_FailDrop(https)
run_ctest_submit_FailDrop(scp)
run_ctest_submit_FailDrop(xmlrpc)
