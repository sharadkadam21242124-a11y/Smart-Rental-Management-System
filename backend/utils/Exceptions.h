#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Exception Handling & Inheritance Hierarchy
 * Custom exception classes derived from std::runtime_error.
 * Used across the system for robust error reporting during validation,
 * database queries, authentication, and rental operations.
 * ============================================================================
 */

class RentalException : public std::runtime_error {
public:
    explicit RentalException(const std::string& message)
        : std::runtime_error(message) {}
};

class ValidationException : public RentalException {
public:
    explicit ValidationException(const std::string& message)
        : RentalException("Validation Error: " + message) {}
};

class NotFoundException : public RentalException {
public:
    explicit NotFoundException(const std::string& message)
        : RentalException("Not Found: " + message) {}
};

class AuthenticationException : public RentalException {
public:
    explicit AuthenticationException(const std::string& message)
        : RentalException("Auth Error: " + message) {}
};

class DatabaseException : public RentalException {
public:
    explicit DatabaseException(const std::string& message)
        : RentalException("Database Error: " + message) {}
};

class BookingConflictException : public RentalException {
public:
    explicit BookingConflictException(const std::string& message)
        : RentalException("Booking Conflict: " + message) {}
};

#endif // EXCEPTIONS_H
