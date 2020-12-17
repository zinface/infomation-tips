cmake_minimum_required(VERSION 3.0.0)

find_package(PkgConfig REQUIRED)

# ARCHITECTURE
if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(ARCHITECTURE "amd64")
elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    set(ARCHITECTURE "arm64")
endif()

string(TIMESTAMP BUILD_TIME "%Y%m%d")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME            "${PROJECT_NAME}_${PROJECT_VERSION}-${BUILD_TIME}${BUILD_RELEASE}_${ARCHITECTURE}.deb")
set(CPACK_DEBIAN_PACKAGE_NAME         "${PROJECT_NAME}")
set(CPACK_DEBIAN_PACKAGE_VERSION      "${PROJECT_VERSION}-${BUILD_TIME}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${ARCHITECTURE}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS      "")
set(CPACK_DEBIAN_PACKAGE_PRIORITY     "optional")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER   "Individual developer <zinfacex@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION  "Display system operation information on the screen")

install(PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}

    DESTINATION /opt/zTools/app
)

install(FILES
    ${CMAKE_SOURCE_DIR}/infomation-tips.desktop
    DESTINATION /usr/share/applications
)

include(CPack)
