cmake_minimum_required(VERSION 3.19)
project(HealthMate LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(HealthMate
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    userprofile.h userprofile.cpp
    medicalhistoryentry.h medicalhistoryentry.cpp
    hydrationtracker.h hydrationtracker.cpp
    medicineentry.h medicineentry.cpp
    labreportentry.h labreportentry.cpp
    healthai.h healthai.cpp

)

target_link_libraries(HealthMate
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS HealthMate
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET HealthMate
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
