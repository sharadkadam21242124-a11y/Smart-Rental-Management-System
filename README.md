# Smart Rental Management System
## Advanced Academic C++17 & MySQL 8.0 Full-Stack Application

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![MySQL 8.0](https://img.shields.io/badge/MySQL-8.0_InnoDB-4479A1?logo=mysql)](https://www.mysql.com/)
[![Bootstrap 5](https://img.shields.io/badge/Bootstrap-5.3-7952B3?logo=bootstrap)](https://getbootstrap.com/)
[![Chart.js](https://img.shields.io/badge/Chart.js-4.4-FF6384?logo=chartdotjs)](https://www.chartjs.org/)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-064F8C?logo=cmake)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

An enterprise-grade, multi-category rental management platform engineered with modern **C++17**, a high-performance REST server (`cpp-httplib`), **MySQL 8.0** with ACID transaction safety, and a responsive frontend web interface across **28 dedicated pages**.

Designed and implemented as an academic capstone and OOP viva defense project for a Second-Year Computer Engineering student, demonstrating all **29 Object-Oriented Programming (OOP) concepts** with working software architecture.

---

## Table of Contents

- [Key Features](#key-features)
- [System Architecture](#system-architecture)
- [Project Directory Structure](#project-directory-structure)
- [28 Responsive Web Pages](#28-responsive-web-pages)
- [Database Design & Setup](#database-design--setup)
- [Building from Source](#building-from-source)
- [Running the Application](#running-the-application)
- [Automated OOP Test Suite](#automated-oop-test-suite)
- [Verified Demo Credentials](#verified-demo-credentials)
- [29 OOP Concepts viva Reference](#29-oop-concepts-viva-reference)
- [REST API Endpoints Reference](#rest-api-endpoints-reference)
- [Security Features](#security-features)

---

## Key Features

- **4 Multi-Domain Rental Categories:**
  - **Cars:** SUVs, sedans, luxury, and electric vehicles with fuel types, seating capacity, and transmission filters.
  - **Bikes:** Cruisers, sports bikes, and commuter motorcycles with engine cc specifications.
  - **Houses:** Luxury villas, apartments, and studios with bedroom, bathroom, and sq.ft details.
  - **Furniture:** Living room sets, ergonomic executive desks, and premium sofas.
- **Role-Based Portals:**
  - **Customer Portal:** Browse catalog, dynamic date & rate calculation, live availability checks, book items, view itemized invoices, record simulated payments (Cash, UPI Demo, Card Demo), inspect booking history, update profile, and cancel eligible reservations.
  - **Admin Console:** Fleet inventory CRUD, customer account enable/disable, booking management, return check-in with overdue/damage fee calculations, security deposit refunds, CSV exports, and Chart.js telemetry reports.
- **Enterprise Financial Integrity:**
  - Security deposits are tracked strictly as liabilities, separated from earned revenue.
  - Transparent itemized bills displaying base rent, duration discounts, security deposits, and net balances.
- **Pure C++17 Core Logic:**
  - Standalone SHA-256 password hashing.
  - RAII transaction protection (`TransactionGuard`) preventing orphaned states.
  - Thread-safe connection pooling and prepared statements against MySQL 8.0.

---

## System Architecture

```
                      +---------------------------------------+
                      |      Frontend Client (28 Pages)       |
                      |  HTML5 / CSS3 / JavaScript / Chart.js |
                      +---------------------------------------+
                                          |
                               REST API via HTTP / JSON
                                          |
                      +---------------------------------------+
                      |         C++17 Backend Server          |
                      |          (cpp-httplib :8080)          |
                      +---------------------------------------+
                       /          |              |          \
            +------------+  +------------+ +------------+ +------------+
            | Auth API   |  | Rental API | | Booking API| | Admin API  |
            +------------+  +------------+ +------------+ +------------+
                       \          |              |          /
                      +---------------------------------------+
                      |      Domain Services & OOP Logic      |
                      |  Polymorphism, Inheritance, Factory   |
                      +---------------------------------------+
                                          |
                      +---------------------------------------+
                      |        MySQL 8.0 InnoDB Database      |
                      |      13 Tables, Foreign Keys, ACID    |
                      +---------------------------------------+
```

---

## Project Directory Structure

```
SmartRentalManagement/
├── backend/
│   ├── api/                  # REST Controllers & cpp-httplib headers
│   │   ├── AuthController.cpp / .h
│   │   ├── RentalController.cpp / .h
│   │   ├── BookingController.cpp / .h
│   │   ├── PaymentController.cpp / .h
│   │   ├── AdminController.cpp / .h
│   │   ├── ReportController.cpp / .h
│   │   └── httplib.h
│   ├── database/             # Database connection, repositories, RAII guards
│   │   ├── Database.cpp / .h
│   │   ├── UserRepository.cpp / .h
│   │   ├── RentalRepository.cpp / .h
│   │   ├── BookingRepository.cpp / .h
│   │   ├── PaymentRepository.cpp / .h
│   │   └── TemplateRepository.h
│   ├── models/               # Domain OOP classes (User, Customer, RentalItem, etc.)
│   ├── services/             # Core business services & billing computations
│   ├── utils/                # Logger, PasswordHasher (SHA-256), Exceptions
│   └── main.cpp              # Server entry point & static file routing
├── config/
│   └── database.example.json # Safe configuration template (placeholders only)
├── database/
│   ├── schema.sql            # 13 InnoDB tables, constraints, foreign keys
│   ├── seed.sql              # 40 items, 10 customers, 2 admins, 8 sample bookings
│   └── DATABASE_DESIGN.md    # ER diagram, schema specs, and design documentation
├── docs/
│   ├── OOP_CONCEPTS.md       # Complete mapping of 29 OOP concepts for viva defense
│   └── API_DOCUMENTATION.md  # Detailed REST API endpoints reference
├── frontend/
│   ├── css/style.css         # Master navy & white theme stylesheet
│   ├── js/api.js             # API client, token management, toasts, auth sync
│   └── *.html                # 28 dedicated web pages
├── tests/
│   └── test_main.cpp         # Standalone unit test suite for OOP concepts
└── CMakeLists.txt            # CMake build definition
```

---

## 28 Responsive Web Pages

| Category | Page File | Description |
|---|---|---|
| **Public** | `index.html` | Homepage with hero section, category navigation, featured fleet, testimonials |
| | `about.html` | Platform mission, engineering architecture, and leadership team |
| | `contact.html` | Customer support, contact form, inquiry routing |
| | `login.html` | Role-based authentication modal with quick demo credentials |
| | `register.html` | Customer registration with client & server-side validation |
| **Customer** | `customer-dashboard.html` | Real-time rental stats, active bookings table, expenditure summary |
| | `categories.html` | Category selector covering Cars, Bikes, Houses, and Furniture |
| | `cars.html` | Dedicated automobile fleet listing with transmission & fuel filters |
| | `bikes.html` | Two-wheeler catalog with engine cc and motorcycle type filters |
| | `houses.html` | Real estate listing with BHK, sq.ft, and furnishing filters |
| | `furniture.html` | Furnishing inventory with material, dimension, and condition filters |
| | `item-details.html` | Complete specifications view, pricing breakdown, and booking launcher |
| | `search.html` | Global catalog search with live filtering and multi-parameter sorting |
| | `booking.html` | Reservation scheduler with live availability check & duration discounts |
| | `booking-confirmation.html` | Printable booking voucher with unique booking reference code |
| | `billing.html` | Detailed GST invoice breakdown with print & download options |
| | `payment.html` | Simulated payment terminal (Cash, UPI Demo, Card Demo) |
| | `my-bookings.html` | Customer active and historical booking reservations management |
| | `rental-history.html` | Completed rental transactions and item check-in log |
| | `profile.html` | Customer account details and profile update interface |
| **Admin** | `admin-dashboard.html` | Fleet telemetry, revenue metrics, and interactive Chart.js graphs |
| | `manage-items.html` | Fleet inventory manager (Add, Edit, Enable/Disable, Delete) |
| | `manage-customers.html` | Customer directory with account suspension / activation controls |
| | `manage-bookings.html` | Reservation status control (Approve, Complete, Cancel) |
| | `manage-payments.html` | Financial ledger tracking transactions, deposits, and refunds |
| | `return-management.html` | Check-in inspection terminal, late fee & damage fee settlement |
| | `reports.html` | Revenue analytics, category distributions, and CSV file exports |
| | `admin-profile.html` | Administrator account profile & password update console |

---

## Database Design & Setup

The database uses **MySQL 8.0** with the InnoDB storage engine for strict foreign key constraints and ACID transactional guarantees.

### Step 1: Execute Schema & Seed Scripts in MySQL

Open MySQL Workbench or your MySQL terminal and run:

```sql
-- 1. Create the schema and 13 InnoDB tables
source database/schema.sql;

-- 2. Populate verified seed data (40 items, 10 customers, 2 admins, sample bookings)
source database/seed.sql;
```

### Step 2: Configure Local Credentials

Copy `config/database.example.json` to `config/database.json`:

```bash
cp config/database.example.json config/database.json
```

Edit `config/database.json` with your local MySQL credentials:

```json
{
  "host": "127.0.0.1",
  "port": 3306,
  "database": "smart_rental_db",
  "username": "YOUR_DATABASE_USERNAME",
  "password": "YOUR_DATABASE_PASSWORD",
  "connectionPoolSize": 5,
  "connect_timeout_seconds": 10
}
```

> **Note:** `config/database.json` is protected by `.gitignore` and is never committed to GitHub.

---

## Building from Source

### Prerequisites

- **C++17 Compiler:** GCC 10+ (MinGW-w64 on Windows), Clang 11+, or MSVC 2019/2022
- **CMake:** Version 3.15 or newer
- **Build Tool:** Ninja or Make / MSBuild
- **MySQL 8.0 C Client Library:** (`mysql.h`, `libmysql`)

### Build Steps

```bash
# 1. Create and enter build directory
mkdir build
cd build

# 2. Configure with CMake
cmake -B . -S .. -G "Ninja"

# 3. Compile the application and test suite
cmake --build .
```

This compiles two executables:
1. `SmartRentalManagement` (the web application and REST API server)
2. `run_tests` (the standalone OOP verification test suite)

---

## Running the Application

Start the server binary:

```bash
# From the project root:
./build/SmartRentalManagement.exe
```

The server binds to `http://localhost:8080`.

Open your web browser and navigate to:
```
http://localhost:8080
```

---

## Automated OOP Test Suite

Run the standalone test binary to verify calculations and OOP mechanisms without requiring a database connection:

```bash
./build/run_tests.exe
```

### Verified Test Assertions:
- **Password Hashing:** SHA-256 computation and legacy verification.
- **Polymorphism & Factory:** Polymorphic rate calculation across Car, Bike, House, Furniture.
- **Operator Overloading:** `operator<` (price comparison) and `operator==` (identity check).
- **Friend Functions:** `compareRentalValue` and stream insertion `operator<<`.
- **Template Repositories:** Generic CRUD operations using `TemplateRepository<T>`.
- **Return Settlement:** Overdue calculation, tiered late fees, damage assessment, deposit refund.
- **Exception Handling:** Custom domain exceptions (`ValidationException`, `NotFoundException`, etc.).

---

## Verified Demo Credentials

Pre-seeded demonstration accounts for viva and testing:

| Role | Email Address | Password | Permissions |
|---|---|---|---|
| **Administrator** | `admin@smartrental.com` | `Password@123` | Full inventory, customers, bookings, returns, reports |
| **Customer** | `aarav.sharma@gmail.com` | `Password@123` | Catalog browsing, booking, demo payments, profile |
| **Customer** | `priya.patel@gmail.com` | `Password@123` | Catalog browsing, booking, demo payments, profile |

*(Demo accounts also accept `Admin@2026` / `Customer@2026` via the backward-compatible verification layer).*

---

## 29 OOP Concepts Viva Reference

All 29 standard OOP concepts are genuinely implemented in the C++ backend:

| # | Concept | Primary Location in Codebase |
|---|---|---|
| 1 | Classes and Objects | `backend/models/*.h`, `backend/models/*.cpp` |
| 2 | Data Members and Methods | `User.h`, `RentalItem.h`, `Booking.h` |
| 3 | Encapsulation | Private members with getters, setters, and validation in all models |
| 4 | Constructors | Default, parameterized, and copy constructors in `Customer`, `Car`, `Booking` |
| 5 | Destructors & RAII | `Database::~Database()`, `TransactionGuard::~TransactionGuard()` |
| 6 | Access Specifiers | `public`, `protected`, `private` across all class definitions |
| 7 | Single Inheritance | `Customer : public User`, `Admin : public User` |
| 8 | Multilevel Inheritance | `RentalItem` $\rightarrow$ `Car` $\rightarrow$ `LuxuryCar` |
| 9 | Multiple Inheritance | `ElectricCar : public Car, public BatteryPowered` |
| 10 | Hierarchical Inheritance | `RentalItem` base for `Car`, `Bike`, `House`, `Furniture` |
| 11 | Hybrid Inheritance | Combination of hierarchical and multiple inheritance in vehicles |
| 12 | Virtual Base Classes | Virtual inheritance resolving diamond problem in `HybridVehicle` |
| 13 | Function Overloading | Multiple `calculateRent()` and `filterItems()` signatures |
| 14 | Operator Overloading | `operator<`, `operator==`, `operator<<` in `RentalItem` and `QueryRow` |
| 15 | Function Overriding | `toJson()`, `calculateRent()`, `getSpecifications()` in subclasses |
| 16 | Virtual Functions | `virtual double calculateRent(...)` in `RentalItem` |
| 17 | Pure Virtual Functions | `= 0` declarations in `RentalItem` base class |
| 18 | Abstract Classes | `RentalItem` cannot be instantiated directly |
| 19 | Friend Functions | `friend bool compareRentalValue(...)` in `RentalItem.h` |
| 20 | Friend Classes | `friend class RentalItemInspector;` in `RentalItem.h` |
| 21 | Static Data Members | `Database::s_instance`, `RentalItem::s_itemCount` |
| 22 | Static Member Functions | `Database::getInstance()`, `RentalItemFactory::createItem()` |
| 23 | `this` Pointer | Method chaining and disambiguation in model setters |
| 24 | Class Templates | `TemplateRepository<T>` in `backend/database/TemplateRepository.h` |
| 25 | Function Templates | `RentalService::sortItems<Comparator>(...)` in `RentalService.h` |
| 26 | Exception Handling | `try / catch` with custom hierarchy (`Exceptions.h`) |
| 27 | File Handling & Streams | `Database::loadConfigFromFile()`, CSV report generation in `ReportService` |
| 28 | Composition vs Aggregation | Composition: `Invoice` contains line items; Aggregation: `BookingService` aggregates repositories |
| 29 | Design Patterns | Singleton (`Database`), Factory (`RentalItemFactory`), Repository, RAII |

See [docs/OOP_CONCEPTS.md](docs/OOP_CONCEPTS.md) for full viva defense explanations and code snippets.

---

## REST API Endpoints Reference

| Method | Endpoint | Access | Purpose |
|---|---|---|---|
| `GET` | `/api/health` | Public | Server and database health check |
| `POST` | `/api/login` | Public | Authenticate user & issue session token |
| `POST` | `/api/register` | Public | Customer registration |
| `POST` | `/api/logout` | Authenticated | Terminate active user session |
| `GET` | `/api/profile` | Authenticated | Retrieve logged-in user profile details |
| `PUT` | `/api/profile` | Authenticated | Update user profile information |
| `POST` | `/api/change-password` | Authenticated | Change user account password |
| `GET` | `/api/categories` | Public | List available rental categories |
| `GET` | `/api/items` | Public | Filtered & sorted rental items catalog |
| `GET` | `/api/items/:id` | Public | Detailed specification of a single item |
| `POST` | `/api/items` | Admin Only | Add a new rental item |
| `PUT` | `/api/items/:id` | Admin Only | Update an existing rental item |
| `DELETE` | `/api/items/:id` | Admin Only | Delete an unreserved rental item |
| `GET` | `/api/availability` | Public | Check date availability for an item |
| `GET` | `/api/bookings` | Authenticated | Retrieve customer bookings or all admin bookings |
| `GET` | `/api/bookings/:id` | Authenticated | Retrieve single booking details |
| `POST` | `/api/bookings` | Customer | Create a new rental reservation |
| `PUT` | `/api/bookings/:id/cancel`| Customer/Admin | Cancel an eligible reservation |
| `GET` | `/api/invoices/:booking_id`| Authenticated | Get itemized invoice breakdown |
| `GET` | `/api/payments` | Authenticated | List user payments or all admin transactions |
| `POST` | `/api/payments` | Authenticated | Record a simulated payment transaction |
| `GET` | `/api/returns` | Admin Only | List all check-in return records |
| `POST` | `/api/returns` | Admin Only | Process item check-in, late fee, and refund |
| `GET` | `/api/admin/dashboard` | Admin Only | Audited KPI telemetry & revenue metrics |
| `GET` | `/api/admin/customers` | Admin Only | List all registered customers |
| `PUT` | `/api/admin/customers/:id/status`| Admin Only | Enable or suspend a customer account |
| `GET` | `/api/reports/revenue` | Admin Only | Monthly revenue and earnings metrics |
| `GET` | `/api/reports/categories` | Admin Only | Inventory distribution across categories |
| `GET` | `/api/reports/export/revenue` | Admin Only | Download revenue report as CSV |
| `GET` | `/api/reports/export/bookings`| Admin Only | Download bookings report as CSV |
| `GET` | `/api/reports/export/inventory`| Admin Only| Download inventory report as CSV |

See [docs/API_DOCUMENTATION.md](docs/API_DOCUMENTATION.md) for payload schemas and sample JSON responses.

---

## Security Features

- **No Plaintext Passwords:** Secure cryptographic hashing using SHA-256 with salt support.
- **SQL Injection Prevention:** All SQL queries utilize prepared statements and input sanitization (`mysql_real_escape_string`).
- **Role-Based Access Control (RBAC):** Admin endpoints are protected by token validation; unauthorized customer access returns HTTP 403.
- **Session Protection:** Tokens are cryptographically tied to user IDs and invalidated upon logout.
- **Audit Trails:** Critical administrative actions and financial transactions are recorded in the `activity_logs` table.
- **Git Safety:** Credentials and sensitive files are strictly excluded via `.gitignore`.
