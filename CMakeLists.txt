cmake_minimum_required (VERSION 2.6)

project (PACT)

if(WIN32)
	set(LIBIRCCLIENT_INCLUDE "" CACHE FILEPATH "LibIRCClient Include directory.")
	set(LIBIRCCLIENT_LIB "" CACHE FILEPATH "LibIrcClient Lib directory.")

	include_directories(${LIBIRCCLIENT_INCLUDE})
	find_library(IRCCLIENT libircclient
		HINTS
		${LIBIRCCLIENT_LIB}
	)
endif()

set(PACTMSG_SRCS
	src/client.c
	src/connection.c
	src/crosssocket.c
	src/linkedlist.c
	src/pstring.c
)

file(GLOB PACTMSG_HEADERS
	src/*.h
)

set(PACTREF_SRCS
	src/pactref/main.c
)

set(PACTREF_HEADERS
	src/crosssockets.h
	src/pstring.h
)

set(TESTER_SRCS
	src/tester/main.c
)

set(TESTER_HEADERS
	src/connection.h
)

option(DEBUG "" ON)
option(IRC_CONN "" ON)

add_library(pactmsg SHARED ${PACTMSG_SRCS} ${PACTMSG_HEADERS})
if (WIN32)
	target_link_libraries(pactmsg ${IRCCLIENT} wsock32 Ws2_32)
else()
	target_link_libraries(pactmsg ircclient)
endif()

if(DEBUG)
	set_target_properties(pactmsg PROPERTIES COMPILE_DEFINITIONS "PACT_DEBUG")
endif()
if(IRC_CONN)
	set_target_properties(pactmsg PROPERTIES COMPILE_DEFINITIONS "PACT_SUPPORTEDCONN_IRC")
endif()

add_executable(pactref ${PACTREF_SRCS} ${PACTREF_HEADERS})
if (WIN32)
	target_link_libraries(pactref pactmsg wsock32 Ws2_32)
else()
	target_link_libraries(pactref pactmsg)
endif()

add_executable(tester ${TESTER_SRCS} ${TESTER_HEADERS})
target_link_libraries(tester pactmsg)