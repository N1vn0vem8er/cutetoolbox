cmake_minimum_required(VERSION 3.19)
project(CuteDevToolBox LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(CuteDevToolBox
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/widgets/jsonformatterwidget.h src/widgets/jsonformatterwidget.cpp src/widgets/jsonformatterwidget.ui
    src/widgets/sqlformatterwidget.h src/widgets/sqlformatterwidget.cpp src/widgets/sqlformatterwidget.ui
    src/widgets/xmlformatterwidget.h src/widgets/xmlformatterwidget.cpp src/widgets/xmlformatterwidget.ui
    src/widgets/htmlformatterwidget.h src/widgets/htmlformatterwidget.cpp src/widgets/htmlformatterwidget.ui
    src/widgets/hashwidget.h src/widgets/hashwidget.cpp src/widgets/hashwidget.ui
)

include_directories(src/widgets)

target_link_libraries(CuteDevToolBox
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS CuteDevToolBox
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CuteDevToolBox
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
