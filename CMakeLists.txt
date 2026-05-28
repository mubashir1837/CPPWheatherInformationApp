cmake_minimum_required(VERSION 3.16)
project(WeatherInformationApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

add_executable(WeatherInformationApp
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
)

target_link_libraries(WeatherInformationApp PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

# Install rules for CPack
install(TARGETS WeatherInformationApp DESTINATION bin)
install(FILES WeatherApp.desktop DESTINATION share/applications)

# CPack configuration for generating a .deb file
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "weather-information-app")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Ali <mubashirali1837@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Modern C++ Weather Information Application")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31), libqt6core6t64, libqt6gui6t64, libqt6widgets6t64, libqt6network6t64")

include(CPack)
