set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-shared")

# no optimization in tcc:
set (CMAKE_C_FLAGS_INIT "")
set (CMAKE_C_FLAGS_DEBUG_INIT "-g")
set (CMAKE_C_FLAGS_MINSIZEREL_INIT "-DNDEBUG")
set (CMAKE_C_FLAGS_RELEASE_INIT "-DNDEBUG")
set (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-g -DNDEBUG")
