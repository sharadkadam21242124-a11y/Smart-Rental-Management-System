-- =====================================================================
-- SMART RENTAL MANAGEMENT SYSTEM - DATABASE SCHEMA (MySQL 8.0)
-- Engine: InnoDB | Character Set: utf8mb4 | Collation: utf8mb4_unicode_ci
-- Executable directly in MySQL Workbench or via mysql CLI client.
-- =====================================================================

-- 1. Create and select the database
CREATE DATABASE IF NOT EXISTS smart_rental_db
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE smart_rental_db;

-- 2. Create Application Database User with Least-Privilege Access
-- (Avoid using the root user for application backend operations)
CREATE USER IF NOT EXISTS 'rental_user'@'localhost' IDENTIFIED BY 'RentalUser@2026';
GRANT SELECT, INSERT, UPDATE, DELETE ON smart_rental_db.* TO 'rental_user'@'localhost';
FLUSH PRIVILEGES;

-- Disable foreign key checks during schema creation/resets
SET FOREIGN_KEY_CHECKS = 0;

-- Drop tables in reverse dependency order if resetting
DROP TABLE IF EXISTS activity_logs;
DROP TABLE IF EXISTS returns;
DROP TABLE IF EXISTS invoices;
DROP TABLE IF EXISTS payments;
DROP TABLE IF EXISTS bookings;
DROP TABLE IF EXISTS furniture;
DROP TABLE IF EXISTS houses;
DROP TABLE IF EXISTS bikes;
DROP TABLE IF EXISTS cars;
DROP TABLE IF EXISTS rental_items;
DROP TABLE IF EXISTS admins;
DROP TABLE IF EXISTS customers;
DROP TABLE IF EXISTS users;

SET FOREIGN_KEY_CHECKS = 1;

-- =====================================================================
-- 1. USERS TABLE (Authentication & Base Account Details)
-- =====================================================================
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    email VARCHAR(191) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role ENUM('customer', 'admin') NOT NULL DEFAULT 'customer',
    is_active TINYINT(1) NOT NULL DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_user_email_active (email, is_active)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 2. CUSTOMERS TABLE (Profile for Customer Accounts)
-- =====================================================================
CREATE TABLE customers (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    full_name VARCHAR(120) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    address TEXT NOT NULL,
    id_proof_number VARCHAR(50),
    driving_license_number VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    CONSTRAINT fk_customer_user FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_customer_phone (phone)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 3. ADMINS TABLE (Profile for Admin Accounts)
-- =====================================================================
CREATE TABLE admins (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    full_name VARCHAR(120) NOT NULL,
    phone VARCHAR(20),
    department VARCHAR(80) DEFAULT 'Operations',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_admin_user FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 4. RENTAL_ITEMS TABLE (Base Polymorphic Catalog Entity)
-- =====================================================================
CREATE TABLE rental_items (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    category ENUM('Car', 'Bike', 'House', 'Furniture') NOT NULL,
    description TEXT,
    daily_rate DECIMAL(10,2) NOT NULL CHECK (daily_rate >= 0),
    security_deposit DECIMAL(10,2) NOT NULL DEFAULT 0.00 CHECK (security_deposit >= 0),
    location VARCHAR(150) NOT NULL,
    status ENUM('available', 'rented', 'maintenance', 'retired') NOT NULL DEFAULT 'available',
    image_url VARCHAR(500) DEFAULT '',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_item_category_status (category, status),
    INDEX idx_item_location (location),
    INDEX idx_item_price (daily_rate)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 5. CARS TABLE (Hierarchical Specialization of RentalItem)
-- =====================================================================
CREATE TABLE cars (
    id INT AUTO_INCREMENT PRIMARY KEY,
    item_id INT NOT NULL UNIQUE,
    brand VARCHAR(80) NOT NULL,
    model VARCHAR(80) NOT NULL,
    year INT NOT NULL,
    fuel_type ENUM('Petrol', 'Diesel', 'Electric', 'Hybrid', 'CNG') NOT NULL,
    seating_capacity INT NOT NULL DEFAULT 5,
    transmission ENUM('Manual', 'Automatic') NOT NULL DEFAULT 'Manual',
    registration_number VARCHAR(50) NOT NULL UNIQUE,
    mileage VARCHAR(30) DEFAULT '18 kmpl',
    CONSTRAINT fk_car_item FOREIGN KEY (item_id) REFERENCES rental_items(id) ON DELETE CASCADE,
    INDEX idx_car_brand_model (brand, model)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 6. BIKES TABLE (Hierarchical Specialization of RentalItem)
-- =====================================================================
CREATE TABLE bikes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    item_id INT NOT NULL UNIQUE,
    brand VARCHAR(80) NOT NULL,
    model VARCHAR(80) NOT NULL,
    year INT NOT NULL,
    engine_capacity_cc INT NOT NULL,
    bike_type ENUM('Commuter', 'Sports', 'Cruiser', 'Scooter', 'Adventure') NOT NULL,
    registration_number VARCHAR(50) NOT NULL UNIQUE,
    helmet_included TINYINT(1) DEFAULT 1,
    CONSTRAINT fk_bike_item FOREIGN KEY (item_id) REFERENCES rental_items(id) ON DELETE CASCADE,
    INDEX idx_bike_brand (brand)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 7. HOUSES TABLE (Hierarchical Specialization of RentalItem)
-- =====================================================================
CREATE TABLE houses (
    id INT AUTO_INCREMENT PRIMARY KEY,
    item_id INT NOT NULL UNIQUE,
    property_type ENUM('Apartment', 'Villa', 'Independent House', 'Studio', 'Penthouse') NOT NULL,
    bedrooms INT NOT NULL DEFAULT 1,
    bathrooms INT NOT NULL DEFAULT 1,
    furnishing ENUM('Furnished', 'Semi-Furnished', 'Unfurnished') NOT NULL DEFAULT 'Furnished',
    area_sqft INT NOT NULL,
    address_line TEXT NOT NULL,
    parking_available TINYINT(1) DEFAULT 1,
    CONSTRAINT fk_house_item FOREIGN KEY (item_id) REFERENCES rental_items(id) ON DELETE CASCADE,
    INDEX idx_house_type (property_type, bedrooms)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 8. FURNITURE TABLE (Hierarchical Specialization of RentalItem)
-- =====================================================================
CREATE TABLE furniture (
    id INT AUTO_INCREMENT PRIMARY KEY,
    item_id INT NOT NULL UNIQUE,
    furniture_type ENUM('Living Room', 'Bedroom', 'Office', 'Dining', 'Storage') NOT NULL,
    material VARCHAR(80) NOT NULL,
    color VARCHAR(50) NOT NULL,
    item_condition ENUM('Brand New', 'Like New', 'Good') NOT NULL DEFAULT 'Like New',
    dimensions VARCHAR(100) DEFAULT '',
    assembly_required TINYINT(1) DEFAULT 0,
    CONSTRAINT fk_furniture_item FOREIGN KEY (item_id) REFERENCES rental_items(id) ON DELETE CASCADE,
    INDEX idx_furniture_type (furniture_type)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 9. BOOKINGS TABLE (Rental Transactions & Dates)
-- =====================================================================
CREATE TABLE bookings (
    id INT AUTO_INCREMENT PRIMARY KEY,
    booking_code VARCHAR(50) NOT NULL UNIQUE,
    customer_id INT NOT NULL,
    item_id INT NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE NOT NULL,
    rental_days INT NOT NULL CHECK (rental_days >= 1),
    daily_rate DECIMAL(10,2) NOT NULL,
    base_rent DECIMAL(10,2) NOT NULL,
    discount_rate DECIMAL(5,2) DEFAULT 0.00,
    discount_amount DECIMAL(10,2) DEFAULT 0.00,
    security_deposit DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    total_amount DECIMAL(10,2) NOT NULL,
    booking_status ENUM('pending', 'confirmed', 'active', 'completed', 'cancelled') NOT NULL DEFAULT 'pending',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    CONSTRAINT fk_booking_customer FOREIGN KEY (customer_id) REFERENCES customers(id) ON DELETE RESTRICT,
    CONSTRAINT fk_booking_item FOREIGN KEY (item_id) REFERENCES rental_items(id) ON DELETE RESTRICT,
    INDEX idx_booking_schedule (item_id, start_date, end_date),
    INDEX idx_booking_customer (customer_id, booking_status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 10. PAYMENTS TABLE (Audit & Demo Payment Transactions)
-- =====================================================================
CREATE TABLE payments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    transaction_id VARCHAR(80) NOT NULL UNIQUE,
    booking_id INT NOT NULL,
    customer_id INT NOT NULL,
    amount DECIMAL(10,2) NOT NULL CHECK (amount >= 0),
    payment_method ENUM('Cash', 'UPI Demo', 'Card Demo') NOT NULL,
    payment_type ENUM('Rental Payment', 'Security Deposit', 'Late Fee', 'Damage Charge', 'Deposit Refund') NOT NULL DEFAULT 'Rental Payment',
    payment_status ENUM('success', 'pending', 'failed', 'refunded') NOT NULL DEFAULT 'success',
    payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    remarks VARCHAR(255) DEFAULT 'Demo Transaction Processed Successfully',
    CONSTRAINT fk_payment_booking FOREIGN KEY (booking_id) REFERENCES bookings(id) ON DELETE RESTRICT,
    CONSTRAINT fk_payment_customer FOREIGN KEY (customer_id) REFERENCES customers(id) ON DELETE RESTRICT,
    INDEX idx_payment_booking (booking_id),
    INDEX idx_payment_status (payment_status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 11. INVOICES TABLE (Billing Summaries)
-- =====================================================================
CREATE TABLE invoices (
    id INT AUTO_INCREMENT PRIMARY KEY,
    invoice_number VARCHAR(60) NOT NULL UNIQUE,
    booking_id INT NOT NULL UNIQUE,
    issue_date DATE NOT NULL,
    due_date DATE NOT NULL,
    subtotal DECIMAL(10,2) NOT NULL,
    discount_amount DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    tax_amount DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    security_deposit DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    total_amount DECIMAL(10,2) NOT NULL,
    paid_amount DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    payment_status ENUM('paid', 'partial', 'unpaid') NOT NULL DEFAULT 'unpaid',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_invoice_booking FOREIGN KEY (booking_id) REFERENCES bookings(id) ON DELETE CASCADE,
    INDEX idx_invoice_number (invoice_number)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 12. RETURNS TABLE (Check-in, Late Fees & Security Refund Operations)
-- =====================================================================
CREATE TABLE returns (
    id INT AUTO_INCREMENT PRIMARY KEY,
    booking_id INT NOT NULL UNIQUE,
    return_code VARCHAR(60) NOT NULL UNIQUE,
    actual_return_date DATE NOT NULL,
    expected_return_date DATE NOT NULL,
    overdue_days INT NOT NULL DEFAULT 0,
    daily_late_fee DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    total_late_fee DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    damage_charges DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    item_condition ENUM('Excellent', 'Good', 'Fair', 'Damaged') NOT NULL DEFAULT 'Good',
    notes TEXT,
    deposit_refunded DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    processed_by_admin_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_return_booking FOREIGN KEY (booking_id) REFERENCES bookings(id) ON DELETE RESTRICT,
    CONSTRAINT fk_return_admin FOREIGN KEY (processed_by_admin_id) REFERENCES admins(id) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================================
-- 13. ACTIVITY_LOGS TABLE (Security & Audit Trail)
-- =====================================================================
CREATE TABLE activity_logs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    action VARCHAR(100) NOT NULL,
    details TEXT,
    ip_address VARCHAR(50) DEFAULT '127.0.0.1',
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_log_user FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE SET NULL,
    INDEX idx_log_action_time (action, timestamp)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
