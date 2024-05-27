get_current_dir_name( prject_name ${CMAKE_CURRENT_SOURCE_DIR} )
message( "============ ${prject_name}" )
message( "name =" ${prject_name} )
project( ${prject_name} VERSION 0.1 LANGUAGES CXX )
message( "============ ${CURRENT_FOLDER}" )

m_set_env( "${Project_Run_bin}" "${Project_Install_bin}" )

file( GLOB_RECURSE SRC_LIST
	"*.h"
	"*.c"
	"*.cpp"
	"*.cxx"
	"*.hpp"
)

string( REPLACE "." "_" project_name_include_dir ${PROJECT_NAME} )


# 设置变量
set( Cmake_Project_Name "${PROJECT_NAME}" )
set( Cmake_Project_Binary_Dir "${CMAKE_BINARY_DIR}" )
set( Cmake_Project_Binary_Dir "${PROJECT_BINARY_DIR}" )
set( Cmake_Project_Binary_File_Name "$<TARGET_FILE:tgt>" )

set( cmake_definitions_out "${CMAKE_CURRENT_LIST_DIR}/${cmake_auto_sub_dir_name}/macro/cmake_to_c_cpp_header_env.h" )
set( cmake_definitions_org "${cmake_definitions_tmp}/cmake_in/cmake_to_c_cpp_header_env.h.in" )

string( REPLACE "." "_" project_name_include_dir ${PROJECT_NAME} )
set( ${project_name_include_dir}_include_dir ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/ PARENT_SCOPE )

STRING( SUBSTRING "${PROJECT_NAME}" 0 1 exportFileStartName )
STRING( SUBSTRING "${PROJECT_NAME}" 1 -1 exportFileName )
STRING( TOUPPER ${exportFileStartName} exportFileStartName )

include_directories( ${DataCheck_include_dir} )
set( ${project_name_include_dir}_include_dir ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE )

# # 合并源码
list( APPEND ${SRC_LIST} ${SRC_UI} ${QM_FILES} ${SRC_DATA} ${cmake_definitions_out} ${exportFileName})
add_library( ${PROJECT_NAME} SHARED
	${SRC_LIST}
)

call_qt_deploy( ${PROJECT_NAME}
	PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Network
	Qt${QT_VERSION_MAJOR}::Core5Compat
	Qt${QT_VERSION_MAJOR}::Sql
	DataCheck
)

get_target_property( project_type_property ${PROJECT_NAME} TYPE )


if( "EXECUTABLE" STREQUAL ${project_type_property} )
	set( project_type_property_is_exe true )
else()
	set( project_type_property_is_exe false )
endif()

configure_file( "${cmake_definitions_org}" "${cmake_definitions_out}" )
SET( exportFileName ${CMAKE_CURRENT_SOURCE_DIR}/${cmake_auto_sub_dir_name}/export/${exportFileStartName}${exportFileName}_export.h )
GENERATE_EXPORT_HEADER( ${PROJECT_NAME}
	EXPORT_FILE_NAME ${exportFileName}
)
