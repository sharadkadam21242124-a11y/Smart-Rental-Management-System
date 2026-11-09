cmake_minimum_required(VERSION 3.15)
project(SmartRentalManagement VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/models
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/services
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/database
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/api
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/utils
)

# Source files
set(SOURCES
    backend/main.cpp
    backend/models/User.cpp
    backend/models/Customer.cpp
    backend/models/Admin.cpp
    backend/models/RentalItem.cpp
    backend/models/Car.cpp
    backend/models/Bike.cpp
    backend/models/House.cpp
    backend/models/Furniture.cpp
    backend/models/RentalItemFactory.cpp
    backend/models/Booking.cpp
    backend/models/Payment.cpp
    backend/models/Invoice.cpp
    backend/models/RentalReturn.cpp
    backend/database/Database.cpp
    backend/database/UserRepository.cpp
    backend/database/RentalRepository.cpp
    backend/database/BookingRepository.cpp
    backend/database/PaymentRepository.cpp
    backend/services/AuthenticationService.cpp
    backend/services/RentalService.cpp
    backend/services/BookingService.cpp
    backend/services/BillingService.cpp
    backend/services/PaymentService.cpp
    backend/services/ReturnService.cpp
    backend/services/ReportService.cpp
    backend/api/AuthController.cpp
    backend/api/RentalController.cpp
    backend/api/BookingController.cpp
    backend/api/PaymentController.cpp
    backend/api/AdminController.cpp
    backend/api/ReportController.cpp
    backend/utils/PasswordHasher.cpp
    backend/utils/Logger.cpp
)

# MySQL Detection
set(MYSQL_FOUND_CUSTOM FALSE)
set(MYSQL_SEARCH_PATHS
    "C:/Program Files/MySQL/MySQL Server 8.0"
    "C:/Program Files/MySQL/MySQL Server 8.1"
    "C:/Program Files/MySQL/MySQL Server 8.2"
    "$ENV{MYSQL_DIR}"
)

find_path(MYSQL_INCLUDE_DIR NAMES mysql.h PATHS ${MYSQL_SEARCH_PATHS} PATH_SUFFIXES include)
find_library(MYSQL_LIBRARY NAMES libmysql mysqlclient PATHS ${MYSQL_SEARCH_PATHS} PATH_SUFFIXES lib)

if (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
    message(STATUS "Found MySQL Client: ${MYSQL_INCLUDE_DIR} and ${MYSQL_LIBRARY}")
    include_directories(${MYSQL_INCLUDE_DIR})
    add_definitions(-DUSE_MYSQL_CLIENT)
    set(MYSQL_FOUND_CUSTOM TRUE)
else()
    message(WARNING "MySQL client libraries not detected via CMake search. Building with internal MySQL connector module.")
endif()

add_executable(SmartRentalManagement ${SOURCES})

# Windows Socket & Cryptography dependencies for cpp-httplib
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    target_link_libraries(SmartRentalManagement PRIVATE ws2_32 crypt32 bcrypt)
endif()

if (MYSQL_FOUND_CUSTOM)
    target_link_libraries(SmartRentalManagement PRIVATE ${MYSQL_LIBRARY})
endif()

# Threading
find_package(Threads REQUIRED)
target_link_libraries(SmartRentalManagement PRIVATE Threads::Threads)

# Test target
set(TEST_SOURCES
    tests/test_main.cpp
    backend/models/User.cpp
    backend/models/Customer.cpp
    backend/models/Admin.cpp
    backend/models/RentalItem.cpp
    backend/models/Car.cpp
    backend/models/Bike.cpp
    backend/models/House.cpp
    backend/models/Furniture.cpp
    backend/models/RentalItemFactory.cpp
    backend/models/Booking.cpp
    backend/models/Payment.cpp
    backend/models/Invoice.cpp
    backend/models/RentalReturn.cpp
    backend/utils/PasswordHasher.cpp
    backend/utils/Logger.cpp
)

add_executable(run_tests ${TEST_SOURCES})
if (WIN32)
    target_link_libraries(run_tests PRIVATE ws2_32 crypt32 bcrypt)
endif()
target_link_libraries(run_tests PRIVATE Threads::Threads)
