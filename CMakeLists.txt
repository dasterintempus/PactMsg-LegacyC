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
	src/ircconnection.c
	src/refconnection.c
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

file(GLOB INTEGRATION_TEST_SRCS
	src/tests/integration/*.c
)

set(INTEGRATION_TEST_HEADERS
    src/tests/greatest.h
	src/connection.h
)

file(GLOB UNIT_TEST_SRCS
    src/tests/unit/test_*.c
    src/tests/unit/main.c
)

set(UNIT_TEST_HEADERS
    src/tests/greatest.h
	src/connection.h
)

option(PACT_DEBUG "" ON)
option(IRC_CONN "" ON)

add_library(pactmsg STATIC ${PACTMSG_SRCS} ${PACTMSG_HEADERS})
if (WIN32)
    target_compile_definitions(pactmsg PRIVATE  "_CRT_SECURE_NO_WARNINGS")
    target_compile_definitions(pactmsg PRIVATE  "_WINSOCK_DEPRECATED_NO_WARNINGS")
	target_link_libraries(pactmsg ${IRCCLIENT} wsock32 Ws2_32)
else()
	target_link_libraries(pactmsg ircclient)
endif()

if(PACT_DEBUG)
	target_compile_definitions(pactmsg PRIVATE "PACT_DEBUG")
endif()
if(IRC_CONN)
	target_compile_definitions(pactmsg PRIVATE "PACT_SUPPORTEDCONN_IRC")
endif()

add_executable(pactref ${PACTREF_SRCS} ${PACTREF_HEADERS})
if (WIN32)
	target_link_libraries(pactref pactmsg wsock32 Ws2_32)
else()
	target_link_libraries(pactref pactmsg)
endif()

add_executable(unittests ${UNIT_TEST_SRCS} ${UNIT_TEST_HEADERS})
target_link_libraries(unittests pactmsg)

add_executable(integrationtests ${INTEGRATION_TEST_SRCS} ${INTEGRATION_TEST_HEADERS})
target_link_libraries(integrationtests pactmsg)