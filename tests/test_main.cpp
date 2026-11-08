#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

#include "../backend/models/User.h"
#include "../backend/models/Customer.h"
#include "../backend/models/Admin.h"
#include "../backend/models/RentalItem.h"
#include "../backend/models/Car.h"
#include "../backend/models/Bike.h"
#include "../backend/models/House.h"
#include "../backend/models/Furniture.h"
#include "../backend/models/RentalItemFactory.h"
#include "../backend/models/Booking.h"
#include "../backend/models/Payment.h"
#include "../backend/models/Invoice.h"
#include "../backend/models/RentalReturn.h"
#include "../backend/database/TemplateRepository.h"
#include "../backend/utils/PasswordHasher.h"
#include "../backend/utils/Exceptions.h"

int g_passedTests = 0;
int g_totalTests = 0;

#define TEST_CASE(name) \
    std::cout << "[RUNNING TEST] " << name << " ... "; \
    g_totalTests++;

#define ASSERT_TRUE(condition, msg) \
    if (!(condition)) { \
        std::cerr << "\n[TEST FAILED] " << msg << std::endl; \
        assert(false); \
    }

#define TEST_PASSED() \
    std::cout << "PASSED" << std::endl; \
    g_passedTests++;

// 1. Password Hashing Test
void testPasswordHashing() {
    TEST_CASE("Password Hashing & Verification");
    std::string plain = "Password@123";
    std::string hash = PasswordHasher::sha256(plain);
    ASSERT_TRUE(!hash.empty(), "Hash should not be empty");
    ASSERT_TRUE(hash.length() == 64, "SHA-256 output must be 64 hex characters");
    ASSERT_TRUE(PasswordHasher::verifyPassword(plain, hash), "Password verification failed");
    ASSERT_TRUE(!PasswordHasher::verifyPassword("WrongPassword", hash), "Wrong password should fail");
    TEST_PASSED();
}

// 2. Inheritance and Polymorphism Test
void testPolymorphismAndRentCalculation() {
    TEST_CASE("Polymorphic Rent Calculation & Factory Pattern");

    // Create polymorphically via Factory Pattern
    std::unique_ptr<RentalItem> car = RentalItemFactory::createItem(
        "Car", 1, "Honda City", "Comfortable sedan", 2000.0, 5000.0, "Mumbai"
    );
    std::unique_ptr<RentalItem> bike = RentalItemFactory::createItem(
        "Bike", 2, "Yamaha R15", "Sporty bike", 1000.0, 2000.0, "Pune"
    );
    std::unique_ptr<RentalItem> house = RentalItemFactory::createItem(
        "House", 3, "2 BHK Flat", "Furnished apartment", 3000.0, 15000.0, "Bengaluru"
    );
    std::unique_ptr<RentalItem> furniture = RentalItemFactory::createItem(
        "Furniture", 4, "Executive Chair", "Ergonomic chair", 100.0, 500.0, "Delhi"
    );

    // Verify polymorphic categories
    ASSERT_TRUE(car->getCategory() == "Car", "Car category mismatch");
    ASSERT_TRUE(bike->getCategory() == "Bike", "Bike category mismatch");
    ASSERT_TRUE(house->getCategory() == "House", "House category mismatch");
    ASSERT_TRUE(furniture->getCategory() == "Furniture", "Furniture category mismatch");

    // Verify polymorphic calculations with tiered discounts
    // Car: 7 days gives 10% discount -> 7 * 2000 = 14000 * 0.90 = 12600
    double carRent7Days = car->calculateRent(7);
    ASSERT_TRUE(std::abs(carRent7Days - 12600.0) < 0.01, "Car 7-day rent calculation incorrect");

    // Bike: 10 days gives 12% discount -> 10 * 1000 = 10000 * 0.88 = 8800
    double bikeRent10Days = bike->calculateRent(10);
    ASSERT_TRUE(std::abs(bikeRent10Days - 8800.0) < 0.01, "Bike 10-day rent calculation incorrect");

    // House: 30 days gives 20% discount -> 30 * 3000 = 90000 * 0.80 = 72000
    double houseRent30Days = house->calculateRent(30);
    ASSERT_TRUE(std::abs(houseRent30Days - 72000.0) < 0.01, "House 30-day rent calculation incorrect");

    // Furniture: 30 days gives 25% discount -> 30 * 100 = 3000 * 0.75 = 2250
    double furnRent30Days = furniture->calculateRent(30);
    ASSERT_TRUE(std::abs(furnRent30Days - 2250.0) < 0.01, "Furniture 30-day rent calculation incorrect");

    TEST_PASSED();
}

// 3. Operator Overloading Test
void testOperatorOverloading() {
    TEST_CASE("Operator Overloading (operator<, operator==)");

    Car carA(1, "Economy Car", "", 1200.0, 3000.0, "Delhi", "Maruti", "Alto", 2022, "Petrol", 4, "Manual", "DL-01-A");
    Car carB(2, "Luxury Car", "", 5000.0, 15000.0, "Delhi", "BMW", "3 Series", 2023, "Petrol", 5, "Automatic", "DL-01-B");
    Car carC(1, "Economy Car Duplicate", "", 1200.0, 3000.0, "Delhi", "Maruti", "Alto", 2022, "Petrol", 4, "Manual", "DL-01-A");

    // Test operator< (Daily rate comparison)
    ASSERT_TRUE(carA < carB, "carA (1200) should be less than carB (5000)");
    ASSERT_TRUE(!(carB < carA), "carB should not be less than carA");

    // Test operator== (Identity comparison)
    ASSERT_TRUE(carA == carC, "carA and carC share ID 1 and should be equal");
    ASSERT_TRUE(!(carA == carB), "carA and carB have different IDs and should not be equal");

    TEST_PASSED();
}

// 4. Friend Function Test
void testFriendFunction() {
    TEST_CASE("Friend Function (compareRentalValue & stream insertion)");

    Car cheapItem(1, "Budget Bike", "", 500.0, 1000.0, "Pune", "Hero", "Splendor", 2022, "Petrol", 2, "Manual", "MH-12-A");
    Car luxuryItem(2, "Super Car", "", 8000.0, 20000.0, "Mumbai", "Porsche", "911", 2023, "Petrol", 2, "Automatic", "MH-01-B");

    // Call friend function
    bool isBetterValue = compareRentalValue(cheapItem, luxuryItem);
    ASSERT_TRUE(isBetterValue, "Friend function compareRentalValue evaluated incorrectly");

    TEST_PASSED();
}

// 5. Template Repository (Class Template) Test
void testTemplateRepository() {
    TEST_CASE("Template Class Repository (TemplateRepository<T>)");

    TemplateRepository<Car> carRepo;
    auto c1 = std::make_shared<Car>(101, "Swift", "City car", 1400.0, 3000.0, "Goa", "Maruti", "Swift", 2023, "Petrol", 5, "Manual", "GA-01-1");
    auto c2 = std::make_shared<Car>(102, "Thar", "SUV", 3200.0, 8000.0, "Goa", "Mahindra", "Thar", 2023, "Diesel", 4, "Manual", "GA-01-2");

    carRepo.save(101, c1);
    carRepo.save(102, c2);

    ASSERT_TRUE(carRepo.count() == 2, "Template repository should contain 2 items");
    ASSERT_TRUE(carRepo.findById(101)->getTitle() == "Swift", "Find by ID failed in template repo");

    // Test generic filter predicate template
    auto affordableCars = carRepo.filter([](const Car& c) {
        return c.getDailyRate() < 2000.0;
    });

    ASSERT_TRUE(affordableCars.size() == 1, "Filter predicate template failed");
    ASSERT_TRUE(affordableCars[0]->getTitle() == "Swift", "Filter returned wrong item");

    TEST_PASSED();
}

// 6. Return Settlement & Late Fee Logic Test
void testReturnSettlement() {
    TEST_CASE("Return Check-in, Overdue Days, Late Fee & Deposit Refund");

    double deposit = 5000.0;
    int overdueDays = 3;
    double dailyLateFee = 1000.0; // 3 days * 1000 = 3000 late fee
    double damageCharges = 500.0; // Total deduction = 3500
    double outLateFee = 0.0;
    double outRefund = 0.0;

    RentalReturn::computeSettlement(deposit, overdueDays, dailyLateFee, damageCharges, outLateFee, outRefund);

    ASSERT_TRUE(std::abs(outLateFee - 3000.0) < 0.01, "Late fee calculation incorrect");
    ASSERT_TRUE(std::abs(outRefund - 1500.0) < 0.01, "Net refund calculation incorrect (5000 - 3500 should be 1500)");

    // Test case where damages and late fee exceed deposit (refund must not be negative)
    RentalReturn::computeSettlement(2000.0, 5, 1000.0, 1000.0, outLateFee, outRefund);
    ASSERT_TRUE(outRefund == 0.0, "Deposit refund should be bounded at zero when deductions exceed deposit");

    TEST_PASSED();
}

// 7. Input Validation and Exception Handling Test
void testValidationExceptions() {
    TEST_CASE("Exception Handling & Input Validation");

    bool caughtEmptyEmail = false;
    try {
        User u(1, "", "hash123");
    } catch (const ValidationException&) {
        caughtEmptyEmail = true;
    }
    ASSERT_TRUE(caughtEmptyEmail, "Should throw ValidationException for empty user email");

    bool caughtInvalidDates = false;
    try {
        Booking b(1, "BK-001", 1, 1, "2026-11-20", "2026-11-10", 0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    } catch (const ValidationException&) {
        caughtInvalidDates = true;
    }
    ASSERT_TRUE(caughtInvalidDates, "Should throw ValidationException when start date is after end date");

    TEST_PASSED();
}

int main() {
    std::cout << "========================================================\n"
              << "   SMART RENTAL MANAGEMENT SYSTEM - TEST SUITE          \n"
              << "   Verifying OOP Concepts, Business Logic & Calculations\n"
              << "========================================================\n"
              << std::endl;

    testPasswordHashing();
    testPolymorphismAndRentCalculation();
    testOperatorOverloading();
    testFriendFunction();
    testTemplateRepository();
    testReturnSettlement();
    testValidationExceptions();

    std::cout << "\n========================================================" << std::endl;
    std::cout << " TEST RESULTS: " << g_passedTests << "/" << g_totalTests
              << " TESTS PASSED SUCCESSFULLY!" << std::endl;
    std::cout << "========================================================\n" << std::endl;

    return (g_passedTests == g_totalTests) ? 0 : 1;
}
