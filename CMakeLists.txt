cmake_minimum_required(VERSION 3.10)
project(ContainersLab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(containers_demo main.cpp)

install(TARGETS containers_demo 
        RUNTIME DESTINATION bin
        BUNDLE DESTINATION bin)

enable_testing()
if(CMAKE_CONFIGURATION_TYPES)  
    add_test(NAME ContainersTest
             COMMAND $<TARGET_FILE:containers_demo>
    )
else()
    add_test(NAME ContainersTest
             COMMAND containers_demo
    )
endif()

# Настройка CPack
set(CPACK_PACKAGE_NAME "containers-demo")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Student")
set(CPACK_PACKAGE_DESCRIPTION "Containers demonstration project")

# Для Linux DEB
set(CPACK_DEBIAN_PACKAGE_NAME "containers-demo")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Student")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

# Для Windows ZIP
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

include(CPack)