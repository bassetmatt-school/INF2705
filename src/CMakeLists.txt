file(GLOB_RECURSE SRC_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.hpp")

target_sources(tp4
	PRIVATE
	${HEADER_FILES}
	${SRC_FILES}
)
