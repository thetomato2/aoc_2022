# allow findXXX resolution in CMake/
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR})
find_package(Vcvars)

if (NOT ${Vcvars_FOUND})
	message(FATAL_ERROR "[msvcToolchain.cmake] Looks like no Visual Studio installed")
endif()

# got vcvars launcher !
set(cmd_wrapper ${Vcvars_LAUNCHER})

#DEBUG : dump env after vcvars*.bat call
#execute_process(COMMAND ${cmd_wrapper} set OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/debug/out.env.before.txt OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${cmd_wrapper} set OUTPUT_VARIABLE env_dump OUTPUT_STRIP_TRAILING_WHITESPACE)

#1. escaping troublesome chars 
string(REPLACE ";" "__semicolon__" env_dump "${env_dump}")
string(REPLACE "\\" "__backslash__" env_dump "${env_dump}")
string(REPLACE "\"" "__doublequote__" env_dump "${env_dump}")

#2. multi-line => one line
string(REGEX REPLACE "[\r\n]+" ";" env_dump "${env_dump}")

#3. keep only lines looking like xx=yy
list(FILTER env_dump INCLUDE REGEX ".+\=.+")

#4. setting captured env var right here
foreach(key_value ${env_dump})
	string(REPLACE "=" ";" key_value_as_list ${key_value})
	list(GET key_value_as_list 0 key)
	list(GET key_value_as_list 1 value)
	
	string(REPLACE "__semicolon__" "\;" key "${key}")
	string(REPLACE "__backslash__" "\\" key "${key}")
	string(REPLACE "__doublequote__" "\"" key "${key}")

	string(REPLACE "__semicolon__" ";" value "${value}")
	string(REPLACE "__backslash__" "\\" value "${value}")
	string(REPLACE "__doublequote__" "\"" value "${value}")
	
	set(ENV{${key}} "${value}")
endforeach()

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES $ENV{INCLUDE})
link_directories($ENV{LIB})

# set(CMAKE_CXX_FLAGS "-Wall"
#   CACHE STRING "my cmake cxx flags")
# set(CMAKE_CXX_FLAGS_DEBUG "-diagnostics:column -WL -Oi -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -FC -Z7 -GS- -Gs9999999"
#   CACHE STRING "my cmake cxx flags")
# set(CMAKE_CXX_FLAGS_RELEASE, "-02 -DNDEBUG"
#   CACHE STRING "my release cmake cxx flags")
#
# set(CMAKE_CXX_STANDARD_LIBRARIES ""
#   CACHE STRING "my cmake cxx flags")

message(STATUS "[msvcToolchain.cmake] env dumping done")

# DEBUG : dump en var after all this
# execute_process(COMMAND cmd /C set OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/de
