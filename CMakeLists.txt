cmake_minimum_required(VERSION 3.25)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Features
option(FEATURE_TMC4671 "TMC4671 IC" OFF)
option(FEATURE_TMC6200 "TMC6200 IC" OFF)

project(tmc-api C)

set(CMAKE_DEBUG_POSTFIX d)

add_library(${PROJECT_NAME} "")
add_library(analogdevicesinc::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ".")

set(HEADER_FILES
        tmc/helpers/API_Header.h
        tmc/helpers/Bits.h
        tmc/helpers/Config.h
        tmc/helpers/Constants.h
        tmc/helpers/CRC.h
        tmc/helpers/Functions.h
        tmc/helpers/Macros.h
        tmc/helpers/RegisterAccess.h
        tmc/helpers/Types.h)

set(SOURCE_FILES
        tmc/helpers/CRC.c
        tmc/helpers/Functions.c)

install(DIRECTORY tmc/helpers
        DESTINATION include/tmc
        FILES_MATCHING
        PATTERN "*.h")

if (${FEATURE_TMC4671})
    list(APPEND HEADER_FILES
            tmc/ic/TMC4671/TMC4671.h
            tmc/ic/TMC4671/TMC4671_Constants.h
            tmc/ic/TMC4671/TMC4671_Fields.h
            tmc/ic/TMC4671/TMC4671_Register.h
            tmc/ic/TMC4671/TMC4671_Variants.h)
    list(APPEND SOURCE_FILES
            tmc/ic/TMC4671/TMC4671.c)

    install(DIRECTORY tmc/ic/TMC4671
            DESTINATION include/tmc/ic
            FILES_MATCHING
            PATTERN "*.h")
endif ()

if (${FEATURE_TMC6200})
    list(APPEND HEADER_FILES
            tmc/ic/TMC6200/TMC6200.h
            tmc/ic/TMC6200/TMC6200_Constants.h
            tmc/ic/TMC6200/TMC6200_Fields.h
            tmc/ic/TMC6200/TMC6200_Register.h)
    list(APPEND SOURCE_FILES
            tmc/ic/TMC6200/TMC6200.c)

    install(DIRECTORY tmc/ic/TMC6200
            DESTINATION include/tmc/ic
            FILES_MATCHING
            PATTERN "*.h")
endif ()

target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES} ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_target
        LIBRARY DESTINATION lib)

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_NAME}-config.cmake.in ${PROJECT_NAME}-config.cmake INSTALL_DESTINATION share/${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        DESTINATION share/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}_target
        NAMESPACE analogdevicesinc::
        FILE ${PROJECT_NAME}-targets.cmake
        DESTINATION share/${PROJECT_NAME})
