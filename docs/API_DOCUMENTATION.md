# REST API Documentation
## Smart Rental Management System (C++17)

All client-server communication between the frontend (JavaScript) and the backend (C++17 `cpp-httplib` server) uses RESTful conventions over HTTP. All payloads and responses are formatted as standard **JSON**.

- **Default Server Base URL:** `http://localhost:8080`
- **API Prefix:** `/api`
- **Standard Authentication:** Bearer Token via HTTP header:
  ```http
  Authorization: Bearer <base64_or_session_token>
  ```
- **Standard Response Envelope:**
  ```json
  {
    "success": true,
    "message": "Operation completed successfully",
    "data": { ... }
  }
  ```
- **Error Response Envelope:**
  ```json
  {
    "success": false,
    "message": "Descriptive error message",
    "data": null
  }
  ```

---

## Table of Contents

1. [Authentication Endpoints (`/api/auth`)](#1-authentication-endpoints)
2. [Rental Items Endpoints (`/api/items`)](#2-rental-items-endpoints)
3. [Bookings Endpoints (`/api/bookings`)](#3-bookings-endpoints)
4. [Billing & Payments Endpoints (`/api/payments`)](#4-billing--payments-endpoints)
5. [Admin Management Endpoints (`/api/admin`)](#5-admin-management-endpoints)
6. [Reports & Exports Endpoints (`/api/reports`)](#6-reports--exports-endpoints)

---

## 1. Authentication Endpoints

### 1.1 Register Customer
Registers a new customer account in the system with SHA-256 password hashing.

- **Method:** `POST`
- **URL:** `/api/auth/register`
- **Access:** Public
- **Request Body:**
  ```json
  {
    "name": "Kavya Patel",
    "email": "kavya.patel@example.com",
    "phone": "+91 98765 43220",
    "password": "Password@2026",
    "address": "402, Sunrise Towers, SG Highway, Ahmedabad",
    "city": "Ahmedabad",
    "drivingLicense": "GJ01-20230008899",
    "aadhaarNumber": "8877 6655 4433"
  }
  ```
- **Success Response (`201 Created`):**
  ```json
  {
    "success": true,
    "message": "Registration successful. Please log in.",
    "data": {
      "customerId": 11,
      "name": "Kavya Patel",
      "email": "kavya.patel@example.com",
      "role": "customer"
    }
  }
  ```

---

### 1.2 User Login
Authenticates either a Customer or an Administrator using their email and password.

- **Method:** `POST`
- **URL:** `/api/auth/login`
- **Access:** Public
- **Request Body:**
  ```json
  {
    "email": "aarav.sharma@example.com",
    "password": "Customer@2026",
    "role": "customer"
  }
  ```
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "message": "Login successful",
    "data": {
      "token": "dGVzdF90b2tlbl8x",
      "user": {
        "id": 1,
        "name": "Aarav Sharma",
        "email": "aarav.sharma@example.com",
        "phone": "+91 98765 43210",
        "role": "customer"
      }
    }
  }
  ```

---

### 1.3 Get Current User Profile
Retrieves account information for the currently authenticated session.

- **Method:** `GET`
- **URL:** `/api/auth/profile`
- **Headers:** `Authorization: Bearer <token>`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": {
      "id": 1,
      "name": "Aarav Sharma",
      "email": "aarav.sharma@example.com",
      "phone": "+91 98765 43210",
      "address": "B-402, Green Acres, Powai, Mumbai",
      "drivingLicense": "MH02-20210045678",
      "aadhaarNumber": "4532 8765 1098",
      "role": "customer"
    }
  }
  ```

---

### 1.4 Update Profile
Updates editable contact information for the authenticated user.

- **Method:** `PUT`
- **URL:** `/api/auth/profile`
- **Headers:** `Authorization: Bearer <token>`
- **Request Body:**
  ```json
  {
    "name": "Aarav K. Sharma",
    "phone": "+91 98765 99999",
    "address": "A-101, Hiranandani Gardens, Powai, Mumbai"
  }
  ```
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "message": "Profile updated successfully"
  }
  ```

---

### 1.5 Change Password
Updates user credentials with verification of current password.

- **Method:** `POST`
- **URL:** `/api/auth/change-password`
- **Headers:** `Authorization: Bearer <token>`
- **Request Body:**
  ```json
  {
    "currentPassword": "OldPassword@123",
    "newPassword": "NewSecurePassword@2026"
  }
  ```
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "message": "Password updated successfully"
  }
  ```

---

## 2. Rental Items Endpoints

### 2.1 List and Filter Rental Items
Retrieves a paginated list of catalog items with optional filtering and sorting.

- **Method:** `GET`
- **URL:** `/api/items`
- **Query Parameters:**
  - `category` (optional): `cars`, `bikes`, `houses`, `furniture`
  - `location` (optional): Filter by city or region (e.g. `Mumbai`, `Bengaluru`)
  - `minPrice` (optional): Minimum daily rent
  - `maxPrice` (optional): Maximum daily rent
  - `search` (optional): Keyword search for title, brand, or model
  - `sort` (optional): `price_asc`, `price_desc`, `rating_desc`
  - `availableOnly` (optional): `true` or `false`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": [
      {
        "id": 1,
        "title": "Hyundai Creta SX (O)",
        "category": "cars",
        "dailyRate": 2800.0,
        "securityDeposit": 5000.0,
        "location": "Mumbai",
        "imageUrl": "https://images.unsplash.com/photo-1549399542-7e3f8b79c341?w=800",
        "isAvailable": true,
        "rating": 4.8,
        "fuelType": "Diesel",
        "transmission": "Automatic",
        "seatingCapacity": 5
      }
    ]
  }
  ```

---

### 2.2 Get Rental Item Details
Retrieves complete specifications for a single item by its ID.

- **Method:** `GET`
- **URL:** `/api/items/:id`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": {
      "id": 1,
      "title": "Hyundai Creta SX (O)",
      "category": "cars",
      "brand": "Hyundai",
      "model": "Creta SX(O) Turbo 1.5",
      "year": 2024,
      "dailyRate": 2800.0,
      "securityDeposit": 5000.0,
      "location": "Mumbai, Maharashtra",
      "description": "Top-spec turbo diesel SUV with panoramic sunroof and ADAS Level 2.",
      "imageUrl": "https://images.unsplash.com/photo-1549399542-7e3f8b79c341?w=800",
      "isAvailable": true,
      "rating": 4.8,
      "fuelType": "Diesel",
      "seatingCapacity": 5,
      "transmission": "Automatic",
      "hasGPS": true
    }
  }
  ```

---

### 2.3 Get Featured Items
Retrieves high-rated featured items for homepage presentation.

- **Method:** `GET`
- **URL:** `/api/featured`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": [ ... ]
  }
  ```

---

## 3. Bookings Endpoints

### 3.1 Create a Booking
Places a rental reservation with automatic conflict and availability checking.

- **Method:** `POST`
- **URL:** `/api/bookings`
- **Headers:** `Authorization: Bearer <token>`
- **Request Body:**
  ```json
  {
    "itemId": 1,
    "startDate": "2026-12-01",
    "endDate": "2026-12-05",
    "notes": "Pick-up at Mumbai International Airport Terminal 2"
  }
  ```
- **Success Response (`201 Created`):**
  ```json
  {
    "success": true,
    "message": "Booking created successfully",
    "data": {
      "bookingId": 9,
      "itemId": 1,
      "itemTitle": "Hyundai Creta SX (O)",
      "totalDays": 4,
      "dailyRate": 2800.0,
      "grossAmount": 11200.0,
      "securityDeposit": 5000.0,
      "discountAmount": 0.0,
      "netPayable": 16200.0,
      "status": "pending_payment"
    }
  }
  ```

---

### 3.2 List Customer Bookings
Retrieves current and past bookings for the logged-in customer.

- **Method:** `GET`
- **URL:** `/api/bookings/my`
- **Headers:** `Authorization: Bearer <token>`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": [
      {
        "id": 9,
        "itemId": 1,
        "itemTitle": "Hyundai Creta SX (O)",
        "category": "cars",
        "startDate": "2026-12-01",
        "endDate": "2026-12-05",
        "totalDays": 4,
        "netPayable": 16200.0,
        "status": "confirmed",
        "paymentStatus": "paid"
      }
    ]
  }
  ```

---

### 3.3 Cancel a Booking
Cancels a reservation if it has not yet commenced.

- **Method:** `POST`
- **URL:** `/api/bookings/:id/cancel`
- **Headers:** `Authorization: Bearer <token>`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "message": "Booking #9 has been cancelled. Any eligible refund will be processed."
  }
  ```

---

## 4. Billing & Payments Endpoints

### 4.1 Get Booking Invoice
Calculates and generates a breakdown of rental charges, taxes, discounts, and refundable deposits.

- **Method:** `GET`
- **URL:** `/api/invoices/:bookingId`
- **Headers:** `Authorization: Bearer <token>`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": {
      "invoiceNumber": "INV-2026-0009",
      "bookingId": 9,
      "customerName": "Aarav Sharma",
      "itemTitle": "Hyundai Creta SX (O)",
      "dailyRate": 2800.0,
      "totalDays": 4,
      "rentalFee": 11200.0,
      "securityDeposit": 5000.0,
      "discount": 0.0,
      "taxGst": 1344.0,
      "grandTotal": 17544.0,
      "isPaid": false
    }
  }
  ```

---

### 4.2 Record Demonstration Payment
Records a safe demo payment transaction (Cash, UPI Demo, or Credit/Debit Card Demo).

- **Method:** `POST`
- **URL:** `/api/payments`
- **Headers:** `Authorization: Bearer <token>`
- **Request Body:**
  ```json
  {
    "bookingId": 9,
    "amount": 17544.0,
    "paymentMethod": "upi_demo",
    "upiId": "aarav@oksbi"
  }
  ```
- **Success Response (`201 Created`):**
  ```json
  {
    "success": true,
    "message": "Demo payment recorded successfully",
    "data": {
      "transactionId": "TXN-DEMO-2026-8871",
      "bookingId": 9,
      "amount": 17544.0,
      "paymentMethod": "upi_demo",
      "paymentStatus": "completed",
      "timestamp": "2026-11-08T18:00:00Z"
    }
  }
  ```

---

## 5. Admin Management Endpoints

### 5.1 Admin Dashboard Statistics
Provides real-time fleet telemetry, revenue breakdown, and counts.

- **Method:** `GET`
- **URL:** `/api/admin/dashboard`
- **Headers:** `Authorization: Bearer <admin_token>`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": {
      "totalItems": 40,
      "totalCustomers": 10,
      "availableItems": 36,
      "rentedItems": 4,
      "totalBookings": 8,
      "totalRevenue": 142400.0,
      "depositsHeld": 66000.0,
      "recentBookings": [ ... ]
    }
  }
  ```

---

### 5.2 Create New Rental Item
Adds a new item to the fleet under any of the 4 supported categories.

- **Method:** `POST`
- **URL:** `/api/admin/items`
- **Headers:** `Authorization: Bearer <admin_token>`
- **Request Body:**
  ```json
  {
    "title": "Toyota Fortuner Legender 4x4",
    "category": "cars",
    "brand": "Toyota",
    "model": "Fortuner Legender",
    "year": 2024,
    "dailyRate": 6500.0,
    "securityDeposit": 15000.0,
    "location": "Mumbai",
    "description": "Heavy-duty 7-seater 4x4 luxury SUV.",
    "imageUrl": "https://images.unsplash.com/photo-1533473359331-0135ef1b58bf?w=800",
    "fuelType": "Diesel",
    "seatingCapacity": 7,
    "transmission": "Automatic",
    "hasGPS": true
  }
  ```
- **Success Response (`201 Created`):**
  ```json
  {
    "success": true,
    "message": "Rental item added successfully",
    "data": { "id": 41 }
  }
  ```

---

### 5.3 Process Item Return & Settlement
Records inspection condition, calculates late fees or damage penalties, and settles deposit refunds.

- **Method:** `POST`
- **URL:** `/api/admin/returns/process`
- **Headers:** `Authorization: Bearer <admin_token>`
- **Request Body:**
  ```json
  {
    "bookingId": 1,
    "actualReturnDate": "2026-11-09",
    "condition": "good",
    "lateFeeDays": 0,
    "damageFee": 0.0,
    "notes": "Vehicle returned on time in immaculate condition."
  }
  ```
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "message": "Return check-in completed. Security deposit refunded.",
    "data": {
      "returnId": 1,
      "originalDeposit": 5000.0,
      "deductions": 0.0,
      "refundedAmount": 5000.0,
      "status": "completed"
    }
  }
  ```

---

## 6. Reports & Exports Endpoints

### 6.1 System Reports Summary
Aggregates category-wise revenue, customer growth, and booking trends for Chart.js dashboards.

- **Method:** `GET`
- **URL:** `/api/reports/summary`
- **Headers:** `Authorization: Bearer <admin_token>`
- **Success Response (`200 OK`):**
  ```json
  {
    "success": true,
    "data": {
      "categoryRevenue": {
        "cars": 48200.0,
        "bikes": 12800.0,
        "houses": 68000.0,
        "furniture": 13400.0
      },
      "monthlyTrend": [
        { "month": "Jun", "bookings": 12, "revenue": 84000.0 },
        { "month": "Jul", "bookings": 18, "revenue": 124000.0 }
      ]
    }
  }
  ```

---

### 6.2 Export Bookings to CSV
Generates and downloads a CSV report of all historical bookings.

- **Method:** `GET`
- **URL:** `/api/reports/export/bookings`
- **Headers:** `Authorization: Bearer <admin_token>`
- **Content-Type:** `text/csv`
- **Sample Output:**
  ```csv
  BookingID,CustomerName,ItemTitle,Category,StartDate,EndDate,GrossAmount,Status
  1,"Aarav Sharma","Hyundai Creta SX (O)","cars",2026-11-01,2026-11-05,11200.00,"completed"
  2,"Neha Gupta","Royal Enfield Classic 350","bikes",2026-11-02,2026-11-06,3600.00,"confirmed"
  ```
