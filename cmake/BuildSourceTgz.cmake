cmake_minimum_required(VERSION 3.0.0)

string(TIMESTAMP BUILD_TIME "%Y%m%d")

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}-${BUILD_TIME}")

# 项目 CMake 可扩展构建模块
install(DIRECTORY cmake
    DESTINATION target
)

# 项目 截图 资源文件
install(DIRECTORY screenshot
    DESTINATION target
)

# 项目 CMake 顶级构建配置, 组织代码、目录
install(FILES
    CMakeLists.txt
    infomation-tips.desktop

    main.cpp
    mainwindow.cpp
    mainwindow.h

    DESTINATION target
)

include(CPack)
