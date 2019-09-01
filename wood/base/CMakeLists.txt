set(BASE_SRCS
	Thread.cc
	CurrentThread.cc
	)

add_library(wood_base ${BASE_SRCS})

target_link_libraries(wood_base pthread)

add_library(wood_base_cxx ${BASE_SRCS})
target_link_libraries(wood_base_cxx pthread)
set_target_properties(wood_base_cxx PROPERTIES COMPILE_FLAGS "-std=c++0x")


install(TARGETS wood_base DESTINATION lib)
install(TARGETS wood_base_cxx DESTINATION lib)

file(GLOB_RECURSE HEADERS "*.hh")
install(FILES ${HEADERS} DESTINATION include/wood/base)
