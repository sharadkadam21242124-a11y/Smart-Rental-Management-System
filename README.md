# Smart Rental Management System

### A Full-Stack, Multi-Category Rental & Inventory Management Platform

<p align="center">
  <a href="https://smart-rental-management-system-gxri.onrender.com">
    <img src="https://img.shields.io/badge/Live%20Demo-Visit%20Application-46E3B7?style=for-the-badge&logo=render" alt="Live Demo">
  </a>
  <a href="https://github.com/sharadkadam21242124-a11y/Smart-Rental-Management-System">
    <img src="https://img.shields.io/badge/GitHub-Repository-181717?style=for-the-badge&logo=github" alt="GitHub Repository">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B" alt="C++17">
  <img src="https://img.shields.io/badge/MySQL-8.0%2F8.4-4479A1?logo=mysql" alt="MySQL">
  <img src="https://img.shields.io/badge/Docker-Containerized-2496ED?logo=docker" alt="Docker">
  <img src="https://img.shields.io/badge/Render-Deployed-46E3B7?logo=render" alt="Render">
  <img src="https://img.shields.io/badge/Aiven-Cloud%20Database-FF3554?logo=aiven" alt="Aiven">
  <img src="https://img.shields.io/badge/Bootstrap-5.3-7952B3?logo=bootstrap" alt="Bootstrap">
  <img src="https://img.shields.io/badge/Chart.js-4.4-FF6384?logo=chartdotjs" alt="Chart.js">
  <img src="https://img.shields.io/badge/CMake-3.15%2B-064F8C?logo=cmake" alt="CMake">
</p>

A full-stack rental and inventory management platform engineered using **C++17**, an embedded HTTP REST server powered by **cpp-httplib**, and **MySQL with InnoDB**. The application provides a responsive web interface with 28 dedicated frontend pages and supports multiple rental categories through a unified management system.

The platform brings together automotive vehicles, motorcycles, residential properties, and furniture under one application. It provides catalog management, availability validation, reservation scheduling, itemized invoicing, simulated payments, return inspections, settlement calculations, role-based access control, and administrative analytics.

**Live Application:** [https://smart-rental-management-system-gxri.onrender.com](https://smart-rental-management-system-gxri.onrender.com)

---

## Table of Contents

- [Live Deployment](#live-deployment)
- [Project Overview](#project-overview)
- [Key Features](#key-features)
- [Supported Rental Categories](#supported-rental-categories)
- [Technology Stack](#technology-stack)
- [System Architecture](#system-architecture)
- [Application Workflows](#application-workflows)
- [Screenshots](#screenshots)
- [Database Design](#database-design)
- [Security Implementation](#security-implementation)
- [Testing and Verification](#testing-and-verification)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Local Installation](#local-installation)
- [Database Configuration](#database-configuration)
- [Docker Setup](#docker-setup)
- [Cloud Deployment](#cloud-deployment)
- [Running Locally](#running-locally)
- [API Documentation](#api-documentation)
- [Demo Access](#demo-access)
- [Known Limitations](#known-limitations)
- [Future Improvements](#future-improvements)
- [Learning Outcomes](#learning-outcomes)
- [License](#license)

---

## Live Deployment

The application is deployed using a containerized architecture with a managed cloud database.

| Component | Technology | Responsibility |
|---|---|---|
| Application Hosting | Render | Hosts the Dockerized C++ application |
| Containerization | Docker | Packages the application and runtime dependencies |
| Backend | C++17 | Handles business logic and REST API requests |
| HTTP Server | cpp-httplib | Serves API endpoints and frontend assets |
| Cloud Database | Aiven MySQL | Provides managed relational database services |
| Frontend | HTML, CSS, JavaScript | Provides the user interface |
| Database Connectivity | MySQL C API | Connects the backend to MySQL |

**Live URL:** [Open Smart Rental Management System](https://smart-rental-management-system-gxri.onrender.com)

**GitHub Repository:** [View Source Code](https://github.com/sharadkadam21242124-a11y/Smart-Rental-Management-System)

> **Deployment note:** The application is hosted on Render. Depending on the hosting plan, the service may sleep after inactivity and take some time to respond to the first request. Cloud database availability and usage are subject to the provider's plan and limits.

---

## Project Overview

### The Problem

Rental businesses often operate across different product categories, including vehicles, residential properties, motorcycles, and furniture.

Managing these operations independently can create fragmented workflows for inventory tracking, availability checking, reservation management, invoicing, customer records, and return settlements.

A unified system can simplify these operations by providing a centralized platform with consistent business rules and category-specific inventory information.

### The Solution

The Smart Rental Management System provides a centralized platform for managing multiple rental categories through one application.

Its architecture combines a C++ backend, relational database, and responsive frontend to manage the complete rental lifecycle.

The system focuses on:

- Centralized multi-category inventory management.
- Date-based reservation and availability validation.
- Automated rental price calculations.
- Itemized billing and simulated payment records.
- Return inspections and security deposit settlements.
- Customer and administrator workflows.
- Operational analytics and report generation.

### Engineering Highlights

**Native C++ Backend**

Built using C++17, demonstrating object-oriented programming, resource management, exception handling, templates, and database integration.

**Embedded REST Server**

Uses cpp-httplib to handle HTTP requests, expose JSON API endpoints, and serve frontend assets.

**Relational Database Engineering**

Uses MySQL and InnoDB for structured data storage, relational integrity, and transactional operations.

**Concurrency-Aware Reservations**

Implements date-interval validation, synchronization, and database transaction handling to manage conflicting reservations.

**Containerized Deployment**

Uses Docker to package the application and its runtime dependencies for cloud deployment.

**Full-Stack Integration**

Connects the C++ backend, MySQL database, and JavaScript frontend through asynchronous REST API communication.

---

## Key Features

### 1. Multi-Category Inventory Management

Manage different rental categories through a centralized catalog.

- Cars
- Bikes and motorcycles
- Houses and residential properties
- Furniture and equipment

### 2. Dynamic Catalog and Filtering

Search and filter rental items using category-specific attributes such as:

- Rental category
- Daily rental price
- Location
- Fuel type
- Transmission
- Engine capacity
- Seating capacity
- Property area and furnishing
- Furniture material and dimensions

### 3. Booking and Availability Management

- Date-based rental scheduling.
- Availability validation.
- Booking creation and management.
- Reservation status tracking.
- Prevention of overlapping reservations.

The booking overlap condition follows half-open interval logic:

```sql
start_date < existing_end
AND end_date > existing_start
```

### 4. Concurrency-Aware Reservations

Reservation operations use synchronization and database transaction management to reduce the risk of simultaneous double bookings.

The system checks availability before completing a reservation and rejects conflicting requests.

### 5. Itemized Invoicing

Generates rental billing information with:

- Rental duration.
- Daily rental charges.
- Applicable discounts.
- Security deposits.
- Additional charges.
- Total payable amount.

### 6. Simulated Payment Processing

Supports demonstration payment methods:

- Cash
- UPI Demo
- Card Demo

The system records simulated payment transactions and updates relevant booking or invoice statuses.

**Note:** No real banking or external payment gateway transaction is performed.

### 7. Return Inspection and Settlement

Administrators can inspect returned rental items and calculate:

- Overdue rental days.
- Late fees.
- Damage deductions.
- Security deposit adjustments.
- Net refundable deposit amounts.

### 8. Administrative Dashboard

Provides operational information through dashboard visualizations and reports.

- Revenue analytics.
- Inventory distribution.
- Booking statistics.
- Rental management.
- Customer management.
- CSV report exports.

### 9. Role-Based Access Control

Separates application permissions into customer and administrator roles.

Customers access their own rental information, while administrators manage inventory, reservations, customers, returns, and reports.

---

## Supported Rental Categories

The application supports four rental domains with category-specific attributes.

| Category | Primary Attributes | Example Filters |
|---|---|---|
| Cars | Brand, model, year, fuel type, seating, transmission, mileage | Fuel type, transmission, seating |
| Bikes | Brand, model, year, engine displacement, bike type | Engine capacity, motorcycle type |
| Houses | Property type, bedrooms, bathrooms, furnishing, area, parking | BHK, furnishing, property type |
| Furniture | Furniture type, material, color, condition, dimensions | Material, dimensions, assembly |

---

## Technology Stack

### Backend

| Technology | Purpose |
|---|---|
| C++17 | Core backend implementation |
| cpp-httplib | Embedded HTTP server and REST API |
| MySQL C API | Native database connectivity |
| Custom JsonHelper | JSON serialization and deserialization |
| C++ Standard Library | Thread synchronization and resource management |
| CMake | Build configuration |

### Database

| Technology | Purpose |
|---|---|
| MySQL 8.0 / 8.4 | Relational database |
| InnoDB | Transactional storage engine |
| Foreign Keys | Referential integrity |
| Prepared Statements | Parameterized database operations |
| Aiven MySQL | Managed cloud database hosting |

### Frontend

| Technology | Purpose |
|---|---|
| HTML5 | Page structure |
| CSS3 | Responsive styling |
| JavaScript ES6+ | Frontend logic and API communication |
| Bootstrap 5.3 | Responsive UI components |
| Font Awesome 6 | Interface icons |
| Chart.js 4.4 | Dashboard visualizations |

### DevOps and Testing

| Technology | Purpose |
|---|---|
| Docker | Containerization |
| Render | Cloud application hosting |
| Aiven | Managed cloud database |
| CMake | Build system |
| Ninja / Make | Build generation |
| GCC / MinGW-w64 | C++ compilation |
| Python | Integration and automated testing |

---

## System Architecture

The application follows a layered architecture that separates presentation, HTTP handling, business logic, and database operations.

```text
                 +--------------------------------+
                 |        Frontend Client         |
                 | HTML / CSS / JavaScript        |
                 | Bootstrap / Chart.js           |
                 +--------------------------------+
                              |
                         REST API / JSON
                              |
                 +--------------------------------+
                 |         C++17 Backend          |
                 |        cpp-httplib             |
                 +--------------------------------+
                              |
              +---------------+---------------+
              |               |               |
        +-----------+   +-----------+   +-----------+
        | Auth API  |   | Rental API|   | Booking API|
        +-----------+   +-----------+   +-----------+
              |               |               |
              +---------------+---------------+
                              |
                 +--------------------------------+
                 |      Domain Services & Models  |
                 | Billing / Availability /       |
                 | Returns / Business Rules       |
                 +--------------------------------+
                              |
                 +--------------------------------+
                 |        Repository Layer        |
                 | SQL Queries / Transactions     |
                 +--------------------------------+
                              |
                 +--------------------------------+
                 |         MySQL Database         |
                 |             InnoDB             |
                 +--------------------------------+
```

### Architectural Principles

**Layered Separation of Concerns**

Controllers handle HTTP requests and responses, domain services implement business rules, and repositories manage database operations.

**RAII Resource Management**

Resource-management classes help control the lifecycle of database transactions and other resources.

**Decoupled Frontend**

Frontend pages communicate with backend API endpoints through asynchronous JavaScript `fetch()` requests.

**Relational Integrity**

Foreign keys and transactional database operations help maintain consistency across related records.

---

## Application Workflows

### Customer Workflow

```text
Browse Rental Catalog
        |
        v
Select Rental Category
        |
        v
Filter Rental Items
        |
        v
Check Availability
        |
        v
Create Booking
        |
        v
View Invoice
        |
        v
Simulated Payment
        |
        v
View Booking Details
```

### Administrator Workflow

```text
Administrator Login
        |
        v
Review Dashboard
        |
        v
Manage Inventory
        |
        v
Manage Customers
        |
        v
Review Reservations
        |
        v
Inspect Returned Items
        |
        v
Calculate Fees and Refunds
        |
        v
Complete Settlement
        |
        v
Generate Reports
```

### Customer Operations

- Browse rental categories.
- Search and filter rental items.
- Check availability for selected dates.
- Create rental reservations.
- View itemized invoices.
- Make demonstration payments.
- Review bookings and account information.

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

## Screenshots

The application includes responsive frontend pages for customers and administrators.

Add genuine application screenshots to `docs/screenshots/` and display them here.

| Interface | Suggested Screenshot |
|---|---|
| Customer Marketplace | `marketplace.png` |
| Booking Scheduler | `booking.png` |
| Invoice and Payment | `invoicing.png` |
| Administrative Dashboard | `admin-dashboard.png` |
| Return Management | `return-management.png` |

Example:

```markdown
![Customer Marketplace](docs/screenshots/marketplace.png)
```

---

## Database Design

The system uses MySQL with the InnoDB storage engine.

Its relational schema separates authentication, customer information, rental inventory, category-specific attributes, bookings, invoices, payments, returns, and activity logs.

### Relational Schema Summary

The documented schema contains 13 relational tables.

| Table | Purpose |
|---|---|
| `users` | Authentication records, password hashes, roles, and account status |
| `admins` | Administrator profiles |
| `customers` | Customer contact information |
| `rental_items` | Common rental catalog metadata |
| `cars` | Automotive specifications |
| `bikes` | Motorcycle specifications |
| `houses` | Residential property specifications |
| `furniture` | Furniture specifications |
| `bookings` | Reservation dates, pricing, deposits, and status |
| `payments` | Payment references, methods, amounts, and status |
| `invoices` | Itemized billing information |
| `returns` | Return inspections and settlement records |
| `activity_logs` | Important system activity records |

For complete database details, refer to:

[Database Design Documentation](database/DATABASE_DESIGN.md)

---

## Security Implementation

Security-related functionality is implemented across authentication, session management, authorization, database operations, and frontend input handling.

### Authentication and Password Handling

The authentication implementation includes password hashing, cryptographic salt generation, and password verification.

The project has platform-specific cryptographic dependencies. The exact password-hashing algorithm used by each build should be reviewed against the corresponding implementation before describing its security guarantees.

### Session Management

- Session tokens are generated for authenticated users.
- Protected endpoints validate session information.
- Logout invalidates the active session.
- Customer authorization checks restrict access to their own records.

### Authorization and IDOR Protection

- Customer and administrator permissions are separated.
- Protected booking and invoice operations validate ownership.
- Administrative endpoints require administrator authorization.

### Database Security

- Parameterized SQL operations are used where implemented.
- Database credentials are configured separately from application source code.
- Cloud database connectivity uses TLS/SSL where configured.

### Input and Network Handling

- Dynamic frontend content is escaped before DOM insertion using the project's HTML escaping utility.
- CORS handling validates request origins against the configured trusted list.
- API endpoints enforce authentication and authorization where required.

> Security features should be evaluated against the actual implementation and deployment configuration. This project is intended as an educational and portfolio application, not as a certified production security system.

---

## Testing and Verification

The project includes C++ unit tests and Python-based integration and concurrency testing.

### 1. C++ Unit Tests

The standalone C++ test runner covers core business logic and programming concepts.

Run:

```powershell
.\build\run_tests.exe
```

Documented test areas include:

| Test Area | Description |
|---|---|
| Password Handling | Password hashing and verification |
| Polymorphism | Polymorphic rental calculations |
| Operator Overloading | Custom comparison operators |
| Friend Functions | Rental value comparison |
| Templates | Generic repository implementation |
| Return Settlement | Overdue fees and deposit calculations |
| Exception Handling | Error handling and validation |
| Date Validation | Booking interval overlap |
| Billing | Discounts and billing calculations |

### 2. Live Integration and E2E Tests

The Python test runner starts the backend and verifies API behavior and rental workflows.

Run:

```powershell
python tests/run_all_live_tests.py
```

The documented test coverage includes:

- Server health checks.
- Administrator and customer authentication.
- Authorization checks.
- Booking date validation.
- Booking creation.
- Invoice ownership checks.
- Payment authorization.
- Return settlement.
- Inventory availability restoration.
- CORS origin validation.
- Concurrent booking requests.

### 3. Concurrency Testing

The concurrency test script evaluates simultaneous booking requests for the same rental item and overlapping date window.

Previously supplied test results reported:

| HTTP Status | Meaning | Count |
|---|---|---:|
| 201 | Reservation created | 1 |
| 409 | Reservation conflict | 9 |

These results should be retained as verified test results only when the corresponding test run is available.

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
│   ├── cloud_init.sql
│   └── DATABASE_DESIGN.md
│
├── docs/
│   ├── API_DOCUMENTATION.md
│   ├── OOP_CONCEPTS.md
│   └── screenshots/
│
├── frontend/
│   ├── css/
│   │   └── style.css
│   ├── js/
│   │   └── api.js
│   └── *.html
│
├── scripts/
│   └── import_to_aiven.ps1
│
├── tests/
│   ├── test_main.cpp
│   ├── test_api_suite.py
│   ├── test_concurrency_double_booking.py
│   └── run_all_live_tests.py
│
├── Dockerfile
├── docker-entrypoint.sh
├── .dockerignore
├── CMakeLists.txt
└── README.md
```

---

## Prerequisites

The local development environment targets Windows.

| Tool | Recommended Version |
|---|---|
| C++ Compiler | GCC 11+ / MinGW-w64 |
| CMake | 3.15+ |
| Ninja | 1.10+ |
| MySQL Server | 8.0+ |
| MySQL C Client Library | Compatible with installed MySQL version |
| Python | 3.8+ (for automated testing) |
| Docker Desktop | Current supported version |

Verify installations:

```powershell
g++ --version
cmake --version
ninja --version
mysql --version
python --version
docker --version
```

---

## Local Installation

### 1. Clone the Repository

```powershell
git clone https://github.com/sharadkadam21242124-a11y/Smart-Rental-Management-System.git

cd Smart-Rental-Management-System
```

### 2. Verify MySQL Service

```powershell
Get-Service -Name MySQL*
```

If your MySQL service is stopped, start it from an elevated PowerShell terminal:

```powershell
Start-Service -Name MySQL80
```

---

## Database Configuration

The application supports local MySQL and managed cloud MySQL configurations.

### Option A: Local MySQL

Log in:

```powershell
mysql -u root -p
```

Initialize the database:

```sql
SOURCE database/schema.sql;
SOURCE database/seed.sql;
```

Create the local configuration:

```powershell
Copy-Item config/database.example.json config/database.json
```

Update the database credentials in `config/database.json`.

Example:

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

### Option B: Aiven Cloud MySQL

The deployed application uses Aiven-managed MySQL connectivity.

To configure another cloud environment:

1. Create a compatible MySQL service.
2. Initialize the required database schema.
3. Import the required seed or demonstration data.
4. Obtain the database connection details.
5. Configure the application's environment variables.
6. Enable the database TLS/SSL settings required by the provider.

Example environment variables:

```text
DB_HOST=your-mysql-host
DB_PORT=your-mysql-port
DB_NAME=your-database-name
DB_USER=your-database-user
DB_PASSWORD=your-database-password
```

**Never commit real database passwords, connection strings, or private certificates to GitHub.**

---

## Docker Setup

The project includes a multi-stage Dockerfile for building and running the application in a containerized environment.

### 1. Build the Docker Image

From the project root:

```bash
docker build -t smart-rental-management .
```

### 2. Run the Container

Configure the required database variables with your own cloud database credentials:

```powershell
docker run -d `
  --name smart-rental-management `
  -p 8080:8080 `
  -e PORT=8080 `
  -e DB_HOST="your-mysql-host" `
  -e DB_PORT="your-mysql-port" `
  -e DB_NAME="your-database-name" `
  -e DB_USER="your-database-user" `
  -e DB_PASSWORD="your-database-password" `
  smart-rental-management
```

View container logs:

```powershell
docker logs -f smart-rental-management
```

Stop the container:

```powershell
docker stop smart-rental-management
```

Remove the container:

```powershell
docker rm smart-rental-management
```

---

## Cloud Deployment

The application is deployed using the following services:

### Render

Render hosts the Dockerized C++ application.

Deployment workflow:

1. Connect the GitHub repository to Render.
2. Create a Web Service.
3. Select Docker as the runtime.
4. Configure the required environment variables.
5. Deploy the application.
6. Verify the application through its public HTTPS URL.

### Aiven

Aiven provides the managed MySQL database.

Deployment workflow:

1. Create a MySQL service.
2. Initialize the database schema.
3. Import the required application data.
4. Configure secure database connectivity.
5. Add the database connection details to the Render environment configuration.
6. Verify backend database connectivity.

### Production Configuration

The deployment should use environment-based configuration for:

- Application port.
- Database host.
- Database port.
- Database name.
- Database username.
- Database password.
- TLS/SSL configuration.

The deployed service should never rely on credentials committed to the public GitHub repository.

**Live Application:** [https://smart-rental-management-system-gxri.onrender.com](https://smart-rental-management-system-gxri.onrender.com)

---

## Running Locally

Ensure the MySQL service is running and the local database configuration is correct.

Configure the MySQL runtime library path if required:

```powershell
$env:PATH = "C:\Program Files\MySQL\MySQL Server 8.0\lib;$env:PATH"
```

Start the application:

```powershell
.\build\SmartRentalManagement.exe
```

Open the application:

[http://localhost:8080](http://localhost:8080)

Keep the terminal running while using the application.

---

## API Documentation

The backend exposes REST API endpoints under `/api`.

| Method | Endpoint | Access | Description |
|---|---|---|---|
| GET | `/api/health` | Public | Checks application health |
| POST | `/api/login` | Public | Authenticates users |
| POST | `/api/register` | Public | Registers a customer |
| POST | `/api/logout` | Authenticated | Invalidates the active session |
| GET | `/api/items` | Public | Retrieves rental items |
| GET | `/api/items/:id` | Public | Retrieves rental item details |
| GET | `/api/availability` | Public | Checks rental availability |
| GET | `/api/bookings` | Authenticated | Retrieves bookings |
| POST | `/api/bookings` | Customer | Creates a reservation |
| PUT | `/api/bookings/:id/cancel` | Customer / Admin | Cancels an eligible booking |
| GET | `/api/invoices/:id` | Authenticated | Retrieves an invoice |
| POST | `/api/payments` | Authenticated | Records a simulated payment |
| POST | `/api/returns` | Admin | Processes a return settlement |
| GET | `/api/admin/dashboard` | Admin | Retrieves dashboard analytics |
| GET | `/api/reports/export/:type` | Admin | Exports CSV reports |

For detailed request payloads, response structures, and error handling, refer to:

[API Documentation](docs/API_DOCUMENTATION.md)

---

## Demo Access

The application supports customer and administrator workflows.

For security reasons, public administrator passwords and production database credentials are not published in this repository.

Use designated demonstration accounts only when they have been configured for public access.

**Important:** Do not use real personal accounts, production credentials, or sensitive customer information for public demonstrations.

---

## Known Limitations

1. **Simulated Payments:** Payment operations demonstrate transaction workflows without connecting to real banking or payment gateway services.

2. **In-Memory Sessions:** Session storage is maintained by the application process. A distributed deployment would benefit from centralized session storage.

3. **Database Synchronization:** Shared database connection synchronization can limit throughput under high concurrent workloads. A production-scale implementation would benefit from a properly managed connection pool.

4. **Cloud Hosting Constraints:** Application availability and response time depend on the selected hosting plan and provider resource limits.

5. **Platform-Specific Dependencies:** Cryptographic implementation and build dependencies must be verified for each supported operating system.

---

## Future Improvements

Potential enhancements include:

- [ ] Integrate Razorpay or Stripe for real payment processing.
- [ ] Add secure webhook verification for payment events.
- [ ] Implement automated email notifications.
- [ ] Add Redis-backed distributed session storage.
- [ ] Introduce a scalable database connection pool.
- [ ] Improve structured logging and application monitoring.
- [ ] Add CI/CD workflows for automated builds and testing.
- [ ] Add more detailed reporting and analytics.
- [ ] Improve deployment automation and infrastructure configuration.
- [ ] Expand automated security and performance testing.

---

## Learning Outcomes

This project provided practical experience with multiple software engineering concepts.

### C++17 Programming

- Object-oriented programming.
- Polymorphism and inheritance.
- Smart pointers and resource management.
- Templates and operator overloading.
- Exception handling.
- RAII-based programming.

### Database Engineering

- Relational database design.
- MySQL schema implementation.
- Foreign keys and referential integrity.
- Transaction management.
- Prepared SQL statements.
- Data consistency.

### Backend Development

- REST API design using C++.
- HTTP request and response handling.
- Authentication and authorization.
- Business logic separation.
- Thread synchronization.
- Concurrency-aware operations.

### Full-Stack Development

- HTML, CSS, and JavaScript integration.
- Asynchronous API communication.
- Responsive interface development.
- Dashboard visualization using Chart.js.
- Frontend and backend integration.

### Cloud and DevOps

- Docker containerization.
- Cloud application deployment.
- Managed MySQL database integration.
- Environment-based configuration.
- Git and GitHub workflows.

---

## License

A formal open-source license has not yet been added to this repository.

The project is currently shared for portfolio review, educational demonstration, and evaluation purposes. Contact the repository owner for questions regarding reuse or redistribution.

---

<p align="center">
  <strong>Smart Rental Management System</strong>
  <br>
  Built with C++17, MySQL, Docker, and modern web technologies.
  <br>
  <a href="https://smart-rental-management-system-gxri.onrender.com">Live Application</a>
  |
  <a href="https://github.com/sharadkadam21242124-a11y/Smart-Rental-Management-System">GitHub Repository</a>
</p>