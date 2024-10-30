cmake_minimum_required(VERSION 3.1)

project(mvl-fb VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)


set(SOURCE_FILES
        Core/Src/main.cpp
        ../CommonTx/mlrs-tx.cpp
        ../CommonTx/config_id.cpp
        ../CommonTx/in.cpp
        ../Common/channel_order.cpp
        ../Common/common_stats.cpp
        ../Common/common_types.cpp
        ../Common/diversity.cpp
        ../Common/fhss.cpp
        ../Common/link_types.cpp
        ../Common/lq_counter.cpp
        ../Common/while.cpp
        ../Common/libs/filters.cpp
        ../Common/thirdparty/thirdparty.cpp)

#include_directories("/usr/include/libdrm" "/usr/include/cairo")

#configure_file("${PROJECT_NAME}_config.h.in" "${PROJECT_NAME}_config.h")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_compile_definitions(TX_DIY_LINUX)
target_link_libraries(${PROJECT_NAME} c m)


# Embed gstreamer
#find_package(PkgConfig REQUIRED)
#pkg_search_module(GST REQUIRED
#        gstreamer-1.0>=1.4
#        gstreamer-app-1.0>=1.4
#)
#pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
#pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
#target_link_libraries(${PROJECT_NAME} PkgConfig::gstreamer PkgConfig::gstreamer-app)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Werror -fsanitize=undefined -fsanitize=address"
  )
  target_link_options(${PROJECT_NAME}
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

#include(GNUInstallDirs)
#install(TARGETS ${PROJECT_NAME}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
