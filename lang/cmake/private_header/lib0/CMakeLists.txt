ADD_LIBRARY(lib0 lib0.c)

target_include_directories(lib0 PRIVATE
    private
    )

target_include_directories(lib0 PUBLIC
    interface
    )
