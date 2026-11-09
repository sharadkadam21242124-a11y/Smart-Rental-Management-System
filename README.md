# Smart Rental Management System

### A Full-Stack Multi-Category Rental & Inventory Management Platform

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![MySQL](https://img.shields.io/badge/MySQL-8.0_InnoDB-4479A1?logo=mysql)](https://www.mysql.com/)
[![Bootstrap](https://img.shields.io/badge/Bootstrap-5.3-7952B3?logo=bootstrap)](https://getbootstrap.com/)
[![Chart.js](https://img.shields.io/badge/Chart.js-4.4-FF6384?logo=chartdotjs)](https://www.chartjs.org/)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-064F8C?logo=cmake)](https://cmake.org/)

A full-stack rental and inventory management platform engineered using modern **C++17**, featuring an embedded multi-threaded HTTP REST server (`cpp-httplib`), **MySQL 8.0 with InnoDB**, and a responsive web application comprising 28 dedicated interface pages.

The system unifies multi-domain rental operations — spanning automotive vehicles, motorcycles, residential properties, and office furniture — into a single platform with booking scheduling, itemized invoicing, return inspection settlements, role-based access control, and administrative analytics.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Key Features](#key-features)
- [Supported Rental Categories](#supported-rental-categories)
- [Technology Stack](#technology-stack)
- [System Architecture](#system-architecture)
- [Application Workflow](#application-workflow)
- [Screenshots and Visual Interface Tour](#screenshots-and-visual-interface-tour)
- [Database Design](#database-design)
- [Security Implementation](#security-implementation)
- [Testing and Verification](#testing-and-verification)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Installation and Setup](#installation-and-setup)
- [Database Configuration](#database-configuration)
- [Build Instructions](#build-instructions)
- [How to Run](#how-to-run)
- [Demo Credentials](#demo-credentials)
- [API Documentation](#api-documentation)
- [Known Limitations](#known-limitations)
- [Future Improvements](#future-improvements)
- [Learning Outcomes](#learning-outcomes)
- [License](#license)

---

## Project Overview

Many rental businesses operate across distinct product domains, such as fleet vehicles, event equipment, residential properties, and furniture. However, standard management software often isolates these domains into separate products.

The **Smart Rental Management System** addresses this challenge by bringing multiple rental categories together under one platform. Its data architecture supports category-specific specifications while standardizing booking calendars, pricing calculations, itemized billing, and return settlements.

### Why This Project Is Technically Interesting

- **Native C++ Backend:** The core backend is built in C++17 rather than relying on managed runtime environments such as Node.js or Python. It demonstrates modern C++ concepts, smart pointers, RAII-based resource management, and direct integration with the MySQL C API.

- **Embedded Web Server:** Uses `cpp-httplib` to serve JSON REST API endpoints and static frontend pages directly through the compiled application on port 8080.

- **Concurrency-Safe Reservations:** Uses date-interval validation, database transactions, and thread synchronization to prevent overlapping reservations.

- **Zero-Build Frontend:** Uses HTML5, CSS3, Vanilla JavaScript, and Bootstrap 5.3 without requiring a Node.js or npm compilation workflow.

- **Relational Database Design:** Uses MySQL 8.0 with InnoDB, foreign keys, and transactional operations to maintain data consistency.

---

## Key Features

- **Dynamic Catalog and Filtering:** Search and filter rental items by category, daily rate, location, transmission, fuel type, engine capacity, square footage, and material dimensions.

- **Mathematical Interval Scheduling:** Validates booking date windows against existing reservations using half-open interval overlap logic:

  `start_date < existing_end AND end_date > existing_start`

- **Concurrency-Safe Reservations:** Synchronizes reservation creation through an RAII `TransactionGuard` and mutex locks to prevent simultaneous double-booking collisions.

- **Itemized Billing and Discounts:** Generates detailed billing statements with duration-based discounts and explicit separation of security deposit liabilities from rental charges.

- **Simulated Payment Gateway:** Supports demonstration payments through Cash, UPI Demo, and Card Demo methods, generating transaction references and updating booking states.

- **Return Inspection and Settlement:** Provides an administrative check-in workflow that calculates overdue days, late fees, damage deductions, and net security deposit refunds.

- **Administrative Analytics:** Provides a dashboard with Chart.js visualizations for revenue and inventory distribution, along with CSV exports for revenue, bookings, and inventory.

- **Role-Based Access Control:** Separates public browsing, customer self-service, and administrator operations through authorization checks.

---

## Supported Rental Categories

The system models four rental domains, each with category-specific relational attributes.

| Category | Primary Attributes | Domain-Specific Filters |
|---|---|---|
| Cars | Brand, model, year, fuel type, seating capacity, transmission, registration number, mileage | Transmission, fuel type, seating capacity |
| Bikes | Brand, model, year, engine displacement, bike type, registration number, helmet included | Engine capacity, motorcycle type |
| Houses | Property type, bedrooms, bathrooms, furnishing status, area, address, parking | BHK count, furnishing, property type |
| Furniture | Furniture type, material, color, condition, dimensions, assembly requirement | Material, dimensions, assembly requirement |

---

## Technology Stack

### Backend

| Technology | Purpose |
|---|---|
| C++17 | Core backend implementation |
| `cpp-httplib` | Embedded HTTP server and REST API |
| Custom `JsonHelper` | JSON serialization, deserialization, and string escaping |
| Windows CNG API | Cryptographic random generation and PBKDF2 key derivation |
| C++ Standard Library | Thread synchronization and resource management |

### Database

| Technology | Purpose |
|---|---|
| MySQL Server 8.0 | Relational database |
| InnoDB | Storage engine, foreign keys, and transactions |
| MySQL C API | Native database connectivity |
| Prepared Statements | Parameterized SQL operations |

### Frontend

| Technology | Purpose |
|---|---|
| HTML5 | Page structure |
| CSS3 | Responsive styling |
| Vanilla JavaScript (ES6+) | Frontend logic and asynchronous API requests |
| Bootstrap 5.3 | Responsive UI components |
| Font Awesome 6 | Interface icons |
| Chart.js 4.4 | Dashboard data visualization |

### Build and Testing

| Technology | Purpose |
|---|---|
| CMake 3.15+ | Build configuration |
| Ninja or Make | Build generation |
| GCC 11+ / MinGW-w64 | C++ compilation |
| C++ Test Runner | Unit testing |
| Python 3.8+ | Automated integration and E2E testing |

---

## System Architecture

The application follows a layered architecture that separates HTTP handling, business logic, database operations, and frontend presentation.

```text
                  +---------------------------------------+
                  |         Frontend Client                |
                  | HTML5 / CSS3 / JavaScript / Chart.js   |
                  +---------------------------------------+
                                |
                         REST API / JSON
                                |
                  +---------------------------------------+
                  |          C++17 Backend                |
                  |       cpp-httplib :8080               |
                  +---------------------------------------+
                                |
             +------------------+------------------+
             |                  |                  |
       +-----------+      +-----------+      +-----------+
       |  Auth API |      | Rental API|      | Booking API|
       +-----------+      +-----------+      +-----------+
                                |
                  +---------------------------------------+
                  |       Domain Services & Models       |
                  | Billing / Availability / Returns     |
                  +---------------------------------------+
                                |
                  +---------------------------------------+
                  |           Repository Layer            |
                  | SQL Statements / Transactions        |
                  +---------------------------------------+
                                |
                  +---------------------------------------+
                  |        MySQL 8.0 InnoDB               |
                  |     Relational Database               |
                  +---------------------------------------+
```

### Architectural Principles

1. **Layered Separation of Concerns:** Controllers handle HTTP requests and responses, domain services implement business rules, and repositories manage database operations.

2. **RAII Resource Management:** The `TransactionGuard` class manages transaction completion and rollback behavior during database operations.

3. **Decoupled Frontend:** Frontend pages communicate with `/api/` endpoints using asynchronous JavaScript `fetch()` requests.

4. **Relational Integrity:** MySQL foreign keys and InnoDB transactions help maintain consistency across related records.

---

## Application Workflow

The system provides separate workflows for customers and administrators.

```text
       CUSTOMER WORKFLOW                 ADMINISTRATOR WORKFLOW

       Browse Catalog                    Review Dashboard
             |                                  |
       Filter Rental Items                Manage Inventory
             |                                  |
       Check Availability                 Manage Customers
             |                                  |
       Create Booking                     Review Reservations
             |                                  |
       Receive Invoice                    Inspect Returned Items
             |                                  |
       Simulate Payment                   Calculate Fees & Refunds
             |                                  |
       View Booking Details               Complete Settlement
```

### Customer Operations

- Browse available rental categories.
- Search and filter rental items.
- Check availability for selected dates.
- Create rental reservations.
- View itemized invoices.
- Make demonstration payments.
- Review personal bookings and account information.

### Administrator Operations

- Monitor dashboard analytics.
- Manage rental inventory.
- Manage customer records.
- Review and manage reservations.
- Inspect returned rental items.
- Calculate overdue and damage charges.
- Process security deposit settlements.
- Export administrative reports.

---

## Screenshots and Visual Interface Tour

The application provides responsive frontend views served through the embedded C++ HTTP server.

The following table identifies important interfaces that can be demonstrated through screenshots.

| Interface | Route / File | Description |
|---|---|---|
| Customer Marketplace | `/index.html`, `/cars.html` | Catalog browsing, rental categories, search, and filters |
| Reservation Scheduler | `/booking.html` | Date selection, availability validation, and rental calculations |
| Itemized Invoicing | `/billing.html`, `/payment.html` | Invoice breakdown and simulated payment workflow |
| Administrative Dashboard | `/admin-dashboard.html` | Operational statistics and Chart.js visualizations |
| Return Management | `/return-management.html` | Return inspection, overdue fees, damage assessment, and deposit settlement |

### Adding Application Screenshots

To make the repository more visually informative, capture screenshots while the application is running locally.

Store screenshots in:

```text
docs/
└── screenshots/
    ├── marketplace.png
    ├── booking.png
    ├── invoicing.png
    ├── admin-dashboard.png
    └── return-management.png
```

Once the screenshots are added, they can be displayed in this section using standard Markdown image links.

For example:

```markdown
![Customer Marketplace](docs/screenshots/marketplace.png)
```

---

## Database Design

The relational database is implemented using MySQL 8.0 and the InnoDB storage engine.

The schema uses separate tables for authentication, customers, rental inventory, category-specific attributes, bookings, invoices, payments, returns, and activity logs.

### Entity Relationship Overview

```text
+---------------+        1:N        +-------------------+
|     users     | ----------------> |     bookings      |
+---------------+                   +-------------------+
       |                                      |
      1:1                                     1:1
       |                                      |
+---------------+                   +-------------------+
|   customers   |                   |     invoices      |
+---------------+                   +-------------------+
                                              |
                                             1:N
                                              |
                                    +-------------------+
                                    |     payments      |
                                    +-------------------+
                                              |
                                             1:1
                                              |
                                    +-------------------+
                                    |      returns      |
                                    +-------------------+

+---------------+        1:N
|  rental_items | ----------------> Category Subtype Tables
+---------------+                    cars
                                     bikes
                                     houses
                                     furniture
```

### Relational Schema Summary

The project database contains 13 relational tables according to the supplied schema documentation.

| Table | Purpose |
|---|---|
| `users` | Authentication records, password hashes, roles, and account status |
| `admins` | Administrator profiles and departmental information |
| `customers` | Customer contact information and addresses |
| `rental_items` | Common rental catalog metadata |
| `cars` | Automotive specifications |
| `bikes` | Motorcycle specifications |
| `houses` | Residential property specifications |
| `furniture` | Furniture specifications |
| `bookings` | Reservation dates, pricing, discounts, deposits, and status |
| `payments` | Payment transaction references, methods, amounts, and status |
| `invoices` | Itemized billing information |
| `returns` | Return inspection and settlement records |
| `activity_logs` | Records of important system actions |

For the complete entity relationship diagram and field specifications, see:

[Database Design Documentation](database/DATABASE_DESIGN.md)

---

## Security Implementation

Security is implemented across authentication, session handling, authorization, database operations, and frontend input rendering.

### 1. Password Hashing and Key Derivation

- Passwords are hashed using PBKDF2-HMAC-SHA256 with 600,000 iterations and unique 16-byte cryptographically secure salts.
- Key derivation and salt generation use the Windows Cryptography API: Next Generation.
- Relevant functions include `BCryptDeriveKeyPBKDF2` and `BCryptGenRandom`.
- Password verification uses constant-time comparison through `PasswordHasher::constantTimeCompare`.
- A migration mechanism upgrades legacy password hashes following successful authentication.

### 2. Session Management and IDOR Protection

- Sessions use cryptographically secure random tokens containing 256 bits of random data.
- Tokens use the format `SRMS-<64-hex-characters>`.
- A 24-hour session expiration period is enforced on protected endpoints.
- Active tokens are invalidated upon logout.
- Customer authorization checks restrict access to their own bookings, invoices, and payments.

### 3. Concurrency Safety and Double-Booking Prevention

- Booking requests use an exclusive `std::recursive_mutex` and an RAII `TransactionGuard`.
- Availability is checked inside a database transaction.
- Overlapping booking requests are rejected with HTTP 409 Conflict when a conflicting reservation exists.

### 4. Input Sanitization and Network Hardening

- Database operations use SQL input handling and parameterized statements where implemented.
- The `escapeHtml` utility escapes dynamic user input before rendering it into DOM elements.
- CORS handling validates request origins against an explicitly trusted list.
- Untrusted preflight `OPTIONS` requests are rejected with HTTP 403 Forbidden.
- Responses omit `Access-Control-Allow-Origin` when the requesting origin is not trusted.

---

## Testing and Verification

The project includes standalone C++ unit tests and Python-based live integration and concurrency test scripts.

### 1. Standalone C++ Unit Test Suite

The C++ test runner covers business logic and core programming concepts without requiring a live database.

Run:

```powershell
$env:PATH = "C:\Program Files\MySQL\MySQL Server 8.0\lib;$env:PATH"
.\build\run_tests.exe
```

The supplied test report identifies coverage for:

| Test Area | Description |
|---|---|
| Password Hashing | PBKDF2-HMAC-SHA256 password hashing |
| Polymorphism | Polymorphic rental calculations and factory pattern |
| Operator Overloading | Custom comparison operators |
| Friend Functions | Rental value comparison and stream insertion |
| Templates | Generic repository implementation |
| Return Settlement | Overdue days, late fees, and deposit refunds |
| Exception Handling | Error handling and input validation |
| Date Validation | Booking interval overlap detection |
| Billing | Tiered discounts and billing calculations |

The previously supplied test output reports **9/9 tests passed**.

### 2. Live Automated E2E and Concurrency Tests

The Python test runner starts the C++ server, checks API behavior, executes the booking lifecycle, and tests concurrent reservation requests.

Run:

```powershell
python tests/run_all_live_tests.py
```

The supplied test report covers:

- Server health check.
- Administrator and customer authentication.
- Authorization barriers for administrator endpoints.
- Booking date validation.
- Booking creation.
- Double-booking collision prevention.
- Invoice authentication and ownership checks.
- Payment authorization.
- Return settlement.
- Rental item availability restoration.
- CORS origin validation.
- Concurrent booking requests.

### 3. Concurrency Test

The supplied concurrency test report describes 10 simultaneous booking requests for the same rental item and overlapping date window.

Reported results:

| HTTP Status | Meaning | Count |
|---|---|---:|
| 201 | Reservation created | 1 |
| 409 | Reservation conflict | 9 |

This demonstrates the intended behavior: only one reservation succeeds while conflicting requests are rejected.

---

## Project Structure

```text
SmartRentalManagement/
│
├── backend/
│   ├── api/
│   │   ├── ApiControllerBase.h
│   │   ├── AuthController.cpp
│   │   ├── AuthController.h
│   │   ├── RentalController.cpp
│   │   ├── RentalController.h
│   │   ├── BookingController.cpp
│   │   ├── BookingController.h
│   │   ├── PaymentController.cpp
│   │   ├── PaymentController.h
│   │   ├── AdminController.cpp
│   │   ├── AdminController.h
│   │   ├── ReportController.cpp
│   │   ├── ReportController.h
│   │   └── httplib.h
│   │
│   ├── database/
│   │   ├── Database.cpp
│   │   ├── Database.h
│   │   ├── UserRepository.cpp
│   │   ├── UserRepository.h
│   │   ├── RentalRepository.cpp
│   │   ├── RentalRepository.h
│   │   ├── BookingRepository.cpp
│   │   ├── BookingRepository.h
│   │   ├── PaymentRepository.cpp
│   │   ├── PaymentRepository.h
│   │   └── TemplateRepository.h
│   │
│   ├── models/
│   ├── services/
│   ├── utils/
│   └── main.cpp
│
├── config/
│   └── database.example.json
│
├── database/
│   ├── schema.sql
│   ├── seed.sql
│   └── DATABASE_DESIGN.md
│
├── docs/
│   ├── API_DOCUMENTATION.md
│   └── OOP_CONCEPTS.md
│
├── frontend/
│   ├── css/
│   │   └── style.css
│   ├── js/
│   │   └── api.js
│   └── *.html
│
├── tests/
│   ├── test_main.cpp
│   ├── test_api_suite.py
│   ├── test_concurrency_double_booking.py
│   └── run_all_live_tests.py
│
├── CMakeLists.txt
└── README.md
```

---

## Prerequisites

The documented build environment targets Windows.

Ensure the following software is installed:

| Tool | Recommended Version | Verification Command |
|---|---|---|
| C++ Compiler | GCC 11+ / MinGW-w64 | `g++ --version` |
| CMake | 3.15+ | `cmake --version` |
| Ninja | 1.10+ | `ninja --version` |
| MySQL Server | 8.0 | `mysql --version` |
| MySQL C Client Library | Compatible with MySQL 8.0 | Verify installed headers and libraries |
| Python (optional) | 3.8+ | `python --version` |

The MySQL headers and libraries are expected in locations such as:

```text
C:\Program Files\MySQL\MySQL Server 8.0\include
C:\Program Files\MySQL\MySQL Server 8.0\lib
```

---

## Installation and Setup

### 1. Clone the Repository

Open Windows PowerShell and run:

```powershell
git clone https://github.com/sharadkadam21242124-a11y/Smart-Rental-Management-System.git

cd Smart-Rental-Management-System
```

### 2. Verify MySQL Service Status

Check whether the MySQL service is running:

```powershell
Get-Service -Name MySQL*
```

If the service is stopped, start it from an elevated PowerShell terminal:

```powershell
Start-Service -Name MySQL80
```

---

## Database Configuration

### 1. Initialize the Database

Log in to your local MySQL server:

```powershell
mysql -u root -p
```

After entering the MySQL password, execute the initialization scripts from the project root directory:

```sql
SOURCE database/schema.sql;

SOURCE database/seed.sql;
```

The schema script creates the database structure, and the seed script inserts initial demonstration data.

### 2. Configure Database Credentials

Copy the example configuration file:

```powershell
Copy-Item config/database.example.json config/database.json
```

Open `config/database.json` and update the credentials for your local MySQL environment.

Example configuration:

```json
{
  "host": "127.0.0.1",
  "port": 3306,
  "database": "smart_rental_db",
  "username": "rental_user",
  "password": "YOUR_LOCAL_PASSWORD_HERE",
  "connectionPoolSize": 5,
  "connect_timeout_seconds": 10
}
```

**Security Note:** The local `config/database.json` file should be excluded through `.gitignore`. Never commit real database credentials or passwords to GitHub.

---

## Build Instructions

Execute these commands in Windows PowerShell from the project root directory.

### 1. Configure Environment Paths

Update the paths according to your local MinGW and MySQL installations:

```powershell
$env:PATH = "C:\mingw64\mingw64\bin;C:\Program Files\MySQL\MySQL Server 8.0\lib;$env:PATH"
```

### 2. Configure CMake

```powershell
cmake -B build -S . -G "Ninja"
```

### 3. Compile the Application

```powershell
cmake --build build
```

The build is expected to generate:

| Executable | Purpose |
|---|---|
| `build/SmartRentalManagement.exe` | Backend server and frontend web host |
| `build/run_tests.exe` | Standalone C++ unit test runner |

---

## How to Run

Ensure the MySQL client library, including `libmysql.dll`, is accessible through your environment's `PATH`.

From the project root directory, execute:

```powershell
$env:PATH = "C:\Program Files\MySQL\MySQL Server 8.0\lib;$env:PATH"

.\build\SmartRentalManagement.exe
```

The application should connect to MySQL, serve static frontend assets from `./frontend`, and listen on port 8080.

Open your browser and visit:

**[http://localhost:8080](http://localhost:8080)**

Keep the terminal running while using the application.

---

## Demo Credentials

The database seed script is documented as initializing the following demonstration accounts:

| Role | Email / Username | Password |
|---|---|---|
| Administrator | `admin@smartrental.com` | `Password@123` |
| Customer | `aarav.sharma@gmail.com` | `Password@123` |
| Customer | `priya.patel@gmail.com` | `Password@123` |

### Access Scope

- **Administrator:** Fleet inventory, customer management, booking controls, return settlements, and revenue reports.
- **Customer:** Catalog browsing, booking scheduling, invoice review, simulated payments, and profile management.

**Security Warning:** These credentials are intended only for local development and demonstration. Replace or remove demonstration accounts before deploying the application in any publicly accessible environment.

---

## API Documentation

The backend exposes JSON REST API endpoints under the `/api` route.

The following table summarizes key endpoints:

| Method | Endpoint | Access | Description |
|---|---|---|---|
| `GET` | `/api/health` | Public | Checks system and database connectivity |
| `POST` | `/api/login` | Public | Authenticates users and returns a session token |
| `POST` | `/api/register` | Public | Registers a customer account |
| `POST` | `/api/logout` | Authenticated | Invalidates the active session |
| `GET` | `/api/items` | Public | Retrieves rental items with filters |
| `GET` | `/api/items/:id` | Public | Retrieves details for a rental item |
| `GET` | `/api/availability` | Public | Checks rental availability for a date window |
| `GET` | `/api/bookings` | Authenticated | Retrieves customer bookings or all bookings for administrators |
| `POST` | `/api/bookings` | Customer | Creates a booking reservation |
| `PUT` | `/api/bookings/:id/cancel` | Customer / Admin | Cancels an eligible reservation |
| `GET` | `/api/invoices/:id` | Authenticated | Retrieves an itemized invoice |
| `POST` | `/api/payments` | Authenticated | Records a simulated payment |
| `POST` | `/api/returns` | Admin | Processes a rental return settlement |
| `GET` | `/api/admin/dashboard` | Admin | Retrieves dashboard statistics |
| `GET` | `/api/reports/export/:type` | Admin | Exports administrative CSV reports |

For complete request payloads, response structures, and error codes, refer to:

[API Documentation](docs/API_DOCUMENTATION.md)

---

## Known Limitations

The current implementation has the following documented limitations:

1. **Simulated Payment Processing:** Payment handling is intended for demonstration. It records payment methods and transaction references without communicating with external banking gateways.

2. **Windows CNG Cryptography:** Password hashing and cryptographic random generation rely on the Windows Cryptography API. Cross-platform support would require an abstraction using a portable cryptography library such as OpenSSL or libsodium.

3. **In-Memory Session Store:** Active sessions are stored in server process memory. Multiple backend instances would require a shared session store, such as Redis.

4. **Single Connection Synchronization:** Database operations share a connection handle protected through `std::recursive_mutex`. Higher-concurrency workloads would benefit from a properly implemented multi-connection pool.

---

## Future Improvements

Potential enhancements for future development include:

- [ ] **Cross-Platform Cryptography:** Implement a portable cryptography provider for Linux and macOS.
- [ ] **Real Payment Gateway Integration:** Integrate payment providers such as Razorpay or Stripe with secure webhook handling.
- [ ] **Containerization:** Add Docker and Docker Compose configurations for the backend and MySQL service.
- [ ] **Automated Email Notifications:** Send booking confirmations, invoices, and return settlement notifications.
- [ ] **Distributed Session Storage:** Add Redis-backed session persistence for multi-instance deployments.
- [ ] **Enhanced Monitoring:** Add structured logging, performance metrics, and centralized error monitoring.
- [ ] **Deployment Automation:** Introduce CI workflows for automated builds and testing.

---

## Learning Outcomes

This project provides practical exposure to several software engineering concepts.

### C++17 Programming

- Modern C++ standard library features.
- Object-oriented programming and polymorphism.
- Smart pointers and resource management.
- Templates and operator overloading.
- Exception handling and custom exceptions.

### Database Engineering

- Relational database design and normalization.
- MySQL schema implementation.
- Foreign key relationships and constraints.
- Transaction management and consistency.
- SQL queries and prepared statements.

### Backend Development

- REST API design using C++.
- HTTP request and response handling.
- Authentication and authorization.
- Business logic separation.
- Thread synchronization and concurrency handling.

### Full-Stack Development

- HTML, CSS, and JavaScript integration.
- Asynchronous frontend API communication.
- Responsive interface development.
- Dashboard visualization with Chart.js.

### Software Engineering Practices

- Layered architecture.
- RAII-based resource management.
- Unit and integration testing.
- Security-conscious application design.
- Git and GitHub project management.

---

## License

A formal open-source license has not yet been added to this repository.

The project is currently provided for evaluation, portfolio review, and educational demonstration purposes. For reuse, redistribution, or commercial inquiries, contact the repository owner or open an issue.

---

**Developed as a software engineering project demonstrating modern C++ backend development, relational database management, REST API design, and full-stack application integration.**