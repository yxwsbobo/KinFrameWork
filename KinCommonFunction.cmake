
macro(KinAddFindModule Name)
	if(NOT KIN_FRAMEWORK_PATH)
		find_path(KIN_FRAMEWORK_PATH KinCommonSetting.cmake)
		if(NOT KIN_FRAMEWORK_PATH)
			find_path(KIN_FRAMEWORK_PATH KinCommonFunction.cmake)
		endif()
	endif()
	
	if(NOT KIN_FRAMEWORK_PATH)
		message(KIN_FRAMEWORK_PATH NOTFOUND)
	else()
		set(${Name}_INCLUDE_DIR ${KIN_FRAMEWORK_PATH}/${Name}/Source)
		set(${Name}_LIBRARIES_PATH ${KIN_FRAMEWORK_PATH}/${Name}/bin/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE})
		set(${Name}_LIBRARIES ${Name} ${ARGN})

		if (KIN_EASY_USE_LIB)
			include_directories(${${Name}_INCLUDE_DIR})
			link_directories(${${Name}_LIBRARIES_PATH})
			mark_as_advanced(${Name}_INCLUDE_DIR ${Name}_LIBRARIES)
			set(KinAllLibs ${KinAllLibs} ${${Name}_LIBRARIES})
		endif (KIN_EASY_USE_LIB)
	endif()
    
endmacro()

macro(KinAddFindModuleNoLib Name)
	if(NOT KIN_FRAMEWORK_PATH)
		find_path(KIN_FRAMEWORK_PATH KinCommonSettin.cmake)
		if(NOT KIN_FRAMEWORK_PATH)
			find_path(KIN_FRAMEWORK_PATH KinCommonFunction.cmake)
		endif()
	endif()
	
	if(NOT KIN_FRAMEWORK_PATH)
		message(KIN_FRAMEWORK_PATH NOTFOUND)
	else()
		set(${Name}_INCLUDE_DIR ${KIN_FRAMEWORK_PATH}/${Name}/Source)

		if (KIN_EASY_USE_LIB)
			include_directories(${${Name}_INCLUDE_DIR})
			mark_as_advanced(${Name}_INCLUDE_DIR)
		endif (KIN_EASY_USE_LIB)
	endif()
    
endmacro()

