file(GLOB_RECURSE SRC_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.hpp")

target_sources(tp3
	PRIVATE
	${HEADER_FILES}
	${SRC_FILES}
)
