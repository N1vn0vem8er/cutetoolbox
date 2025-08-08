cmake_minimum_required(VERSION 3.19)
project(CuteDevToolBox LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Xml Concurrent WebEngineCore WebEngineWidgets PrintSupport)
find_package(LibXml2 REQUIRED)
find_package(ZLIB)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(CuteDevToolBox
    WIN32 MACOSX_BUNDLE
    resources/resources.qrc
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/widgets/jsonformatterwidget.h src/widgets/jsonformatterwidget.cpp src/widgets/jsonformatterwidget.ui
    src/widgets/sqlformatterwidget.h src/widgets/sqlformatterwidget.cpp src/widgets/sqlformatterwidget.ui
    src/widgets/xmlformatterwidget.h src/widgets/xmlformatterwidget.cpp src/widgets/xmlformatterwidget.ui
    src/widgets/htmlformatterwidget.h src/widgets/htmlformatterwidget.cpp src/widgets/htmlformatterwidget.ui
    src/widgets/hashwidget.h src/widgets/hashwidget.cpp src/widgets/hashwidget.ui
    src/widgets/loremipsumgenerator.h src/widgets/loremipsumgenerator.cpp src/widgets/loremipsumgenerator.ui
    src/widgets/uuidgeneratorwidget.h src/widgets/uuidgeneratorwidget.cpp src/widgets/uuidgeneratorwidget.ui
    src/widgets/randomtextgeneratorwidget.h src/widgets/randomtextgeneratorwidget.cpp src/widgets/randomtextgeneratorwidget.ui
    src/widgets/qrcodegenerator.h src/widgets/qrcodegenerator.cpp src/widgets/qrcodegenerator.ui
    src/widgets/regexwidget.h src/widgets/regexwidget.cpp src/widgets/regexwidget.ui
    src/widgets/textdiffwidget.h src/widgets/textdiffwidget.cpp src/widgets/textdiffwidget.ui
    src/widgets/markdownwidget.h src/widgets/markdownwidget.cpp src/widgets/markdownwidget.ui
    src/widgets/xmlschemevalidatorwidget.h src/widgets/xmlschemevalidatorwidget.cpp src/widgets/xmlschemevalidatorwidget.ui
    src/widgets/jsonschemavalidatorwidget.h src/widgets/jsonschemavalidatorwidget.cpp src/widgets/jsonschemavalidatorwidget.ui
    src/widgets/htmlcoderdecoderwidget.h src/widgets/htmlcoderdecoderwidget.cpp src/widgets/htmlcoderdecoderwidget.ui
    src/widgets/base64coderdecoderwidget.h src/widgets/base64coderdecoderwidget.cpp src/widgets/base64coderdecoderwidget.ui
    src/widgets/urlcoderdecoderwidget.h src/widgets/urlcoderdecoderwidget.cpp src/widgets/urlcoderdecoderwidget.ui
    src/widgets/gzipwidget.h src/widgets/gzipwidget.cpp src/widgets/gzipwidget.ui
    src/widgets/jsonyamlconverter.h src/widgets/jsonyamlconverter.cpp src/widgets/jsonyamlconverter.ui
    src/utils/editor/codeeditor.cpp src/utils/editor/codeeditor.h src/utils/editor/linenumberarea.cpp src/utils/editor/linenumberarea.h
    src/utils/document.h src/utils/document.cpp
    src/utils/previewpage.h src/utils/previewpage.cpp
    src/utils/syntaxhighlighters/markdownsyntaxhighlighter.h src/utils/syntaxhighlighters/markdownsyntaxhighlighter.cpp
    src/utils/syntaxhighlighters/htmlsyntaxhighlighter.h src/utils/syntaxhighlighters/htmlsyntaxhighlighter.cpp
    src/utils/syntaxhighlighters/jsonsyntaxhighlighter.h src/utils/syntaxhighlighters/jsonsyntaxhighlighter.cpp
    src/widgets/randomnumbergeneratorwidget.h src/widgets/randomnumbergeneratorwidget.cpp src/widgets/randomnumbergeneratorwidget.ui
    src/widgets/chmodcalculatorwidget.h src/widgets/chmodcalculatorwidget.cpp src/widgets/chmodcalculatorwidget.ui
    src/widgets/timestampwidget.h src/widgets/timestampwidget.cpp src/widgets/timestampwidget.ui
    src/widgets/numberbaseswidget.h src/widgets/numberbaseswidget.cpp src/widgets/numberbaseswidget.ui
)

include_directories(
    src/widgets
    src
    src/utils
)

target_link_libraries(CuteDevToolBox
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Xml
        Qt::Concurrent
        Qt::WebEngineCore
        Qt::WebEngineWidgets
        Qt::PrintSupport
        LibXml2::LibXml2
        ZLIB::ZLIB
        qrcodegencpp
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
