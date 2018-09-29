set(KIN_EASY_USE_LIB 1)

set(BIN_FILE_PATH bin/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE})

if (NOT DO_NOT_HANDLE_OUTPUT)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${BIN_FILE_PATH})
	set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
endif ()


if(NOT KIN_FRAMEWORK_PATH)
	find_path(KIN_FRAMEWORK_PATH KinCommonSetting.cmake)
	if(NOT KIN_FRAMEWORK_PATH)
		find_path(KIN_FRAMEWORK_PATH KinCommonFunction.cmake)
	endif()
endif()

if(NOT KIN_FRAMEWORK_PATH)
	message(KIN_FRAMEWORK_PATH NOTFOUND)
else()
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
		${KIN_FRAMEWORK_PATH}/KinBase
		${KIN_FRAMEWORK_PATH}/KinScript
		${KIN_FRAMEWORK_PATH}/LuaCore
		${KIN_FRAMEWORK_PATH}/ChaiScript
		${KIN_FRAMEWORK_PATH}/spdlog
		${KIN_FRAMEWORK_PATH}/sol2
		${KIN_FRAMEWORK_PATH}/nlohmann
		${KIN_FRAMEWORK_PATH}/websocketpp
		${KIN_FRAMEWORK_PATH}/boost
	)
endif()
