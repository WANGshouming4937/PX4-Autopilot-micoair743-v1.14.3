############################################################################
#
#   Copyright (c) 2026 PX4 Development Team. All rights reserved.
#
############################################################################

px4_add_module(
	MODULE drivers__mt06_micolink
	MAIN mt06_micolink
	SRCS
		MT06Micolink.cpp
		MT06Micolink.hpp
		mt06_micolink_main.cpp
	MODULE_CONFIG
		module.yaml
	DEPENDS
		drivers_rangefinder
	)
