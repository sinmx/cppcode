INCLUDE_DIRECTORIES(../1)
INCLUDE_DIRECTORIES(../7)

# add_executable(Ch13_copyNoMove copyNoMove.cpp)
add_executable(Ch13_FolderMain FolderMain.cpp Folder.cpp)
add_executable(Ch13_HasPtrUseCount HasPtrUseCount.cpp)
add_executable(Ch13_HasPtrValueLike HasPtrValueLike.cpp)
add_executable(Ch13_moveHP moveHP.cpp)
add_executable(Ch13_serial serial.cpp)
add_executable(Ch13_StrFolderMain StrFolderMain.cpp StrFolder.cpp String.cpp)
add_executable(Ch13_substr substr.cpp)
add_executable(Ch13_useStrVec useStrVec.cpp StrVec.cpp)

Config_DebugInfo(Ch13_useStrVec "")