set(NET_SRCS
	EventLoop.cc
	EventDemultiplexer.cc
	EventHandler.cc
	poller/DefaultEventDemultiplexer.cc
	poller/Poller.cc
	poller/EPoller.cc
	)

add_library(wood_net ${NET_SRCS})
target_link_libraries(wood_net wood_base)

add_library(wood_net_cxx ${NET_SRCS})
target_link_libraries(wood_net_cxx wood_base_cxx)

set_target_properties(wood_net_cxx PROPERTIES COMPILE_FLAGS "-std=c++0x")


install(TARGETS wood_net DESTINATION lib)
install(TARGETS wood_net_cxx DESTINATION lib)


set(HEADERS

)

install(FILES ${HEADERS} DESTINATION include/wood/net)
