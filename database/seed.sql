-- =====================================================================
-- SMART RENTAL MANAGEMENT SYSTEM - SEED DATA (MySQL 8.0)
-- Indian Market Context: Real vehicle models, Indian metro cities, INR (₹)
-- Passwords: All sample users use password 'Password@123'
-- SHA-256 hash of 'Password@123' = 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f'
-- =====================================================================

USE smart_rental_db;

SET FOREIGN_KEY_CHECKS = 0;

-- ---------------------------------------------------------------------
-- 1. USERS (2 Admins + 10 Customers)
-- ---------------------------------------------------------------------
INSERT INTO users (id, email, password_hash, role, is_active) VALUES
(1, 'admin@smartrental.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'admin', 1),
(2, 'manager@smartrental.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'admin', 1),
(3, 'aarav.sharma@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(4, 'priya.patel@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(5, 'rohan.deshmukh@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(6, 'ananya.iyer@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(7, 'vikram.malhotra@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(8, 'sneha.kulkarni@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(9, 'rahul.verma@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(10, 'pooja.nair@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(11, 'amit.gupta@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1),
(12, 'neha.reddy@gmail.com', 'ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f', 'customer', 1);

-- ---------------------------------------------------------------------
-- 2. ADMINS
-- ---------------------------------------------------------------------
INSERT INTO admins (id, user_id, full_name, phone, department) VALUES
(1, 1, 'Rajesh Singhania', '+91 98201 11223', 'Executive Management'),
(2, 2, 'Kavita Joshi', '+91 98202 33445', 'Operations & Fleet');

-- ---------------------------------------------------------------------
-- 3. CUSTOMERS
-- ---------------------------------------------------------------------
INSERT INTO customers (id, user_id, full_name, phone, address, id_proof_number, driving_license_number) VALUES
(1, 3, 'Aarav Sharma', '+91 98765 43210', 'Flat 402, Green Glen Layout, Bellandur, Bengaluru', 'AADHAAR-890123456789', 'DL-0420180012345'),
(2, 4, 'Priya Patel', '+91 98765 43211', '12, Shanti Sadan, CG Road, Navrangpura, Ahmedabad', 'AADHAAR-901234567890', 'DL-0120190023456'),
(3, 5, 'Rohan Deshmukh', '+91 98765 43212', 'B-14, Mayur Colony, Kothrud, Pune', 'AADHAAR-012345678901', 'DL-1220200034567'),
(4, 6, 'Ananya Iyer', '+91 98765 43213', 'New No 28, 4th Main Road, RA Puram, Chennai', 'AADHAAR-123456789012', 'DL-0720170045678'),
(5, 7, 'Vikram Malhotra', '+91 98765 43214', 'Tower 3, Flat 901, DLF Phase 5, Gurugram', 'AADHAAR-234567890123', 'DL-2620160056789'),
(6, 8, 'Sneha Kulkarni', '+91 98765 43215', 'A-301, Casa Bella, Dombivli East, Mumbai', 'AADHAAR-345678901234', 'DL-0520210067890'),
(7, 9, 'Rahul Verma', '+91 98765 43216', '54/2, Civil Lines, Near High Court, Prayagraj', 'AADHAAR-456789012345', 'DL-7020190078901'),
(8, 10, 'Pooja Nair', '+91 98765 43217', 'Panampilly Nagar, Kochi, Kerala', 'AADHAAR-567890123456', 'DL-4320200089012'),
(9, 11, 'Amit Gupta', '+91 98765 43218', '74/A, Sector 18, Salt Lake City, Kolkata', 'AADHAAR-678901234567', 'DL-0220180090123'),
(10, 12, 'Neha Reddy', '+91 98765 43219', 'Plot 88, Road No 36, Jubilee Hills, Hyderabad', 'AADHAAR-789012345678', 'DL-0920220101234');

-- ---------------------------------------------------------------------
-- 4. RENTAL ITEMS (40 Total: 10 Cars, 10 Bikes, 10 Houses, 10 Furniture)
-- ---------------------------------------------------------------------

-- CARS (Items 1 to 10)
INSERT INTO rental_items (id, title, category, description, daily_rate, security_deposit, location, status, image_url) VALUES
(1, 'Tata Harrier XZA+ Dark Edition', 'Car', 'Premium 5-seater SUV with panoramic sunroof, ventilated leatherette seats and powerful Kryotec 2.0L diesel engine.', 3500.00, 10000.00, 'Bengaluru', 'available', 'https://images.unsplash.com/photo-1549399542-7e3f8b79c341?auto=format&fit=crop&w=800&q=80'),
(2, 'Hyundai Creta SX(O) Turbo', 'Car', 'Versatile modern compact SUV with 10.25-inch infotainment, Bose sound and DCT automatic transmission.', 2800.00, 8000.00, 'Mumbai', 'available', 'https://images.unsplash.com/photo-1503376780353-7e6692767b70?auto=format&fit=crop&w=800&q=80'),
(3, 'Mahindra Thar LX 4x4 Hard Top', 'Car', 'Rugged 4x4 off-roader with convertible styling, touchscreen infotainment, perfect for road trips and hilly terrain.', 3200.00, 12000.00, 'Pune', 'rented', 'https://images.unsplash.com/photo-1533473359331-0135ef1b58bf?auto=format&fit=crop&w=800&q=80'),
(4, 'Maruti Suzuki Swift ZXi+', 'Car', 'Fuel efficient, agile city hatchback with keyless entry, Android Auto/Apple CarPlay and smooth drive.', 1400.00, 4000.00, 'Delhi NCR', 'available', 'https://images.unsplash.com/photo-1541899481282-d53bffe3c35d?auto=format&fit=crop&w=800&q=80'),
(5, 'Toyota Fortuner 4x4 AT', 'Car', 'Imposing 7-seater luxury SUV with bulletproof reliability, 4WD system and unmatched highway road presence.', 5500.00, 20000.00, 'Hyderabad', 'available', 'https://images.unsplash.com/photo-1519641471654-76ce0107ad1b?auto=format&fit=crop&w=800&q=80'),
(6, 'Honda City ZX CVT', 'Car', 'Executive sedan offering class-leading rear seat comfort, sunroof and silky smooth i-VTEC petrol performance.', 2400.00, 7000.00, 'Chennai', 'available', 'https://images.unsplash.com/photo-1552519507-da3b142c6e3d?auto=format&fit=crop&w=800&q=80'),
(7, 'Kia Seltos GTX+ Automatic', 'Car', 'Tech-loaded stylish compact SUV with heads-up display, 360-degree camera and sport tuned suspension.', 2900.00, 8000.00, 'Ahmedabad', 'available', 'https://images.unsplash.com/photo-1508974239320-0a029497e820?auto=format&fit=crop&w=800&q=80'),
(8, 'MG Hector Sharp Pro Petrol CVT', 'Car', 'Internet inside family SUV featuring huge 14-inch HD touchscreen, voice controls and plush lounge seating.', 2700.00, 7500.00, 'Kolkata', 'available', 'https://images.unsplash.com/photo-1550355291-bbee04a92027?auto=format&fit=crop&w=800&q=80'),
(9, 'Mahindra XUV700 AX7 Luxury', 'Car', 'Next-generation flagship 7-seater SUV with ADAS safety features, Alexa integration and twin 10-inch screens.', 3800.00, 12000.00, 'Bengaluru', 'available', 'https://images.unsplash.com/photo-1542282088-72c9c27ed0cd?auto=format&fit=crop&w=800&q=80'),
(10, 'Tata Nexon EV Max Empowered', 'Car', 'Zero-emission electric SUV with 453 km ARAI range, wireless charging, and whisper-quiet city and highway cruising.', 2600.00, 8000.00, 'Mumbai', 'available', 'https://images.unsplash.com/photo-1563720223185-11003d516935?auto=format&fit=crop&w=800&q=80');

-- BIKES (Items 11 to 20)
INSERT INTO rental_items (id, title, category, description, daily_rate, security_deposit, location, status, image_url) VALUES
(11, 'Royal Enfield Classic 350 Reborn', 'Bike', 'Timeless retro cruiser motorcycle with the refined J-series 349cc engine and dual-channel ABS.', 1100.00, 3000.00, 'Goa', 'available', 'https://images.unsplash.com/photo-1558981403-c5f9899a28bc?auto=format&fit=crop&w=800&q=80'),
(12, 'Yamaha YZF R15 V4 Racing Blue', 'Bike', 'Aerodynamic track-bred supersport machine with quick shifter, traction control and upside-down front forks.', 1300.00, 3500.00, 'Pune', 'available', 'https://images.unsplash.com/photo-1568772585407-9361f9bf3a87?auto=format&fit=crop&w=800&q=80'),
(13, 'KTM 390 Duke ABS', 'Bike', 'The quintessential corner rocket with 43.5 PS raw punch, TFT digital console, and slipper clutch.', 1600.00, 5000.00, 'Bengaluru', 'rented', 'https://images.unsplash.com/photo-1599819811279-d5ad9cccf838?auto=format&fit=crop&w=800&q=80'),
(14, 'Honda Activa 6G Premium', 'Bike', 'India’s favorite, ultra-reliable everyday commuter scooter with eSP technology and silent start ACG motor.', 450.00, 1500.00, 'Delhi NCR', 'available', 'https://images.unsplash.com/photo-1609630875171-b1321377ee65?auto=format&fit=crop&w=800&q=80'),
(15, 'Bajaj Pulsar NS200 Dual ABS', 'Bike', 'Aggressive streetfighter naked bike with perimeter frame and triple-spark 200cc liquid-cooled engine.', 850.00, 2500.00, 'Hyderabad', 'available', 'https://images.unsplash.com/photo-1615172282427-9a57ef2d142e?auto=format&fit=crop&w=800&q=80'),
(16, 'TVS Apache RTR 160 4V Special Edition', 'Bike', 'Class-leading 160cc performance with 3 ride modes (Urban, Rain, Sport) and Bluetooth SmartXonnect.', 750.00, 2000.00, 'Chennai', 'available', 'https://images.unsplash.com/photo-1558981420-87aa9dad1c89?auto=format&fit=crop&w=800&q=80'),
(17, 'Suzuki Access 125 Ride Connect', 'Bike', 'Premium comfortable 125cc scooter with retro chrome headlamp, smartphone connectivity, and generous boot space.', 500.00, 1500.00, 'Kolkata', 'available', 'https://images.unsplash.com/photo-1591637333184-19aa84b3e01f?auto=format&fit=crop&w=800&q=80'),
(18, 'Royal Enfield Himalayan 450', 'Bike', 'Purpose-built adventure tourer with Sherpa 450 liquid-cooled motor, Google Maps screen integration and high ground clearance.', 1800.00, 6000.00, 'Chandigarh', 'available', 'https://images.unsplash.com/photo-1571188654248-7a89213915f7?auto=format&fit=crop&w=800&q=80'),
(19, 'Ather 450X Gen 3 Smart EV', 'Bike', 'Futuristic smart electric scooter with warp speed acceleration, Google navigation, and fast home charger.', 650.00, 2000.00, 'Bengaluru', 'available', 'https://images.unsplash.com/photo-1558981806-ec527fa84c39?auto=format&fit=crop&w=800&q=80'),
(20, 'Jawa 42 Bobber Moonstone White', 'Bike', 'Distinctive single-seater bobber with chopped fenders, low-slung stance and bar-end mirrors.', 1250.00, 4000.00, 'Jaipur', 'available', 'https://images.unsplash.com/photo-1558981285-6f0c94958bb6?auto=format&fit=crop&w=800&q=80');

-- HOUSES (Items 21 to 30)
INSERT INTO rental_items (id, title, category, description, daily_rate, security_deposit, location, status, image_url) VALUES
(21, 'Modern 2 BHK Fully Furnished High-Rise', 'House', 'Air-conditioned 2 BHK flat overlooking landscaped gardens with modular kitchen, gym access and covered parking.', 2200.00, 15000.00, 'Indiranagar, Bengaluru', 'available', 'https://images.unsplash.com/photo-1522708323590-d24dbb6b0267?auto=format&fit=crop&w=800&q=80'),
(22, '3 BHK Luxury Sea-Breeze Penthouse', 'House', 'Opulent penthouse with private terrace deck, home theater setup, jacuzzi and Arabian Sea views.', 6500.00, 40000.00, 'Bandra West, Mumbai', 'available', 'https://images.unsplash.com/photo-1502672260266-1c1ef2d93688?auto=format&fit=crop&w=800&q=80'),
(23, 'Cozy 1 BHK Studio Apartment', 'House', 'Compact modern studio designed for IT professionals, fiber optic Wi-Fi, power backup, walking distance to Tech Park.', 1200.00, 8000.00, 'Hinjewadi, Pune', 'rented', 'https://images.unsplash.com/photo-1560448204-e02f11c3d0e2?auto=format&fit=crop&w=800&q=80'),
(24, '4 BHK Heritage Villa with Private Lawn', 'House', 'Sprawling colonial architecture villa with outdoor swimming pool, servant quarters and lush fruit trees.', 7500.00, 50000.00, 'ECR, Chennai', 'available', 'https://images.unsplash.com/photo-1580587771525-78b9dba3b914?auto=format&fit=crop&w=800&q=80'),
(25, '2 BHK Executive Suite Flat', 'House', 'Smart apartment with biometric entry, Italian marble flooring and clubhouse membership in premium gated community.', 2600.00, 18000.00, 'Hitec City, Hyderabad', 'available', 'https://images.unsplash.com/photo-1512917774080-9991f1c4c750?auto=format&fit=crop&w=800&q=80'),
(26, '3 BHK Golf-Course Facing Luxury Condo', 'House', 'Ultra-luxurious apartment on 18th floor with uninterrupted golf-course views and 24/7 concierge.', 4500.00, 30000.00, 'Golf Course Road, Gurugram', 'available', 'https://images.unsplash.com/photo-1600585154340-be6161a56a0c?auto=format&fit=crop&w=800&q=80'),
(27, 'Sunny 1 BHK Garden Terrace Home', 'House', 'Charming rooftop flat with private herb garden, high ceilings, peaceful neighbourhood near trendy cafes.', 1400.00, 9000.00, 'Koramangala, Bengaluru', 'available', 'https://images.unsplash.com/photo-1600596542815-ffad4c1539a9?auto=format&fit=crop&w=800&q=80'),
(28, '3 BHK Royal Sprawling Independent House', 'House', 'Traditional red-brick architecture independent home with marble courtyards, terrace sit-out and two-car garage.', 3200.00, 20000.00, 'Gomti Nagar, Lucknow', 'available', 'https://images.unsplash.com/photo-1600607687939-ce8a6c25118c?auto=format&fit=crop&w=800&q=80'),
(29, '2 BHK Serene Lake-View Residence', 'House', 'Sunlit corner unit facing water body, fully equipped with energy-efficient appliances and 24-hr security.', 2100.00, 12000.00, 'Salt Lake Sector V, Kolkata', 'available', 'https://images.unsplash.com/photo-1600566753376-12c8ab7fb75b?auto=format&fit=crop&w=800&q=80'),
(30, '4 BHK Portuguese Beachfront Holiday Home', 'House', 'Historic restored Goan villa 2 minutes walk from pristine beach with outdoor dining veranda and hammocks.', 8000.00, 45000.00, 'Candolim, North Goa', 'available', 'https://images.unsplash.com/photo-1600585154526-990dced4db0d?auto=format&fit=crop&w=800&q=80');

-- FURNITURE (Items 31 to 40)
INSERT INTO rental_items (id, title, category, description, daily_rate, security_deposit, location, status, image_url) VALUES
(31, 'Urban Living 3-Seater Motorized Recliner', 'Furniture', 'Ergonomic plush genuine leatherette electric recliner with USB charging ports and cup holders.', 300.00, 2000.00, 'Bengaluru', 'available', 'https://images.unsplash.com/photo-1555041469-a586c61ea9bc?auto=format&fit=crop&w=800&q=80'),
(32, 'King Size Solid Sheesham Wood Bed', 'Furniture', 'Crafted from 100% seasoned Indian rosewood with sturdy hydraulic under-bed storage and padded headboard.', 350.00, 2500.00, 'Mumbai', 'available', 'https://images.unsplash.com/photo-1505693416388-ac5ce068fe85?auto=format&fit=crop&w=800&q=80'),
(33, 'Ergonomic High-Back Executive Mesh Chair', 'Furniture', 'Orthopedic lumbar support, 4D adjustable armrests, breathable Korean mesh and synchronized tilting mechanism.', 150.00, 1000.00, 'Pune', 'available', 'https://images.unsplash.com/photo-1580481077195-c3a821a58875?auto=format&fit=crop&w=800&q=80'),
(34, 'Royal 6-Seater Solid Wood Dining Table Set', 'Furniture', 'Classic dark walnut finished dining table with 6 cushioned chairs, heat and scratch resistant top coat.', 280.00, 2000.00, 'Delhi NCR', 'available', 'https://images.unsplash.com/photo-1617806118233-18e1de247200?auto=format&fit=crop&w=800&q=80'),
(35, 'Modern Minimalist Wooden Study Desk', 'Furniture', 'Clean engineered wood work station with cable organizer grommet, stationery drawer and keyboard tray.', 120.00, 800.00, 'Hyderabad', 'rented', 'https://images.unsplash.com/photo-1518455027359-f3f8164ba6bd?auto=format&fit=crop&w=800&q=80'),
(36, 'L-Shaped Sectional Fabric Sofa in Teal Blue', 'Furniture', 'High-density foam 5-seater corner sofa with removable washable covers and accent cushions.', 320.00, 2500.00, 'Chennai', 'available', 'https://images.unsplash.com/photo-1493663284031-b7e3aefcae8e?auto=format&fit=crop&w=800&q=80'),
(37, 'Spacious 4-Door Engineered Wood Wardrobe', 'Furniture', 'Full-length mirror, hanging rods, internal lockable drawer and multiple shelving compartments.', 250.00, 1800.00, 'Ahmedabad', 'available', 'https://images.unsplash.com/photo-1595428774223-ef52624120d2?auto=format&fit=crop&w=800&q=80'),
(38, 'Tempered Glass & Solid Oak Coffee Table', 'Furniture', 'Sleek Scandinavian geometric center table with lower shelf for magazines and decor books.', 90.00, 600.00, 'Kolkata', 'available', 'https://images.unsplash.com/photo-1533090161767-e6ffed986c88?auto=format&fit=crop&w=800&q=80'),
(39, 'Queen Size Platform Bed with Mattress', 'Furniture', 'Low profile platform bed including comfortable 6-inch orthopedic memory foam mattress and waterproof protector.', 300.00, 2200.00, 'Bengaluru', 'available', 'https://images.unsplash.com/photo-1540518614846-7ede433c4ef7?auto=format&fit=crop&w=800&q=80'),
(40, 'Rustic 5-Tier Industrial Display Bookshelf', 'Furniture', 'Matte black metal frame with warm reclaimed pine shelves, anti-tip wall anchors included.', 110.00, 800.00, 'Mumbai', 'available', 'https://images.unsplash.com/photo-1594653114975-d14c2b9a7860?auto=format&fit=crop&w=800&q=80');

-- ---------------------------------------------------------------------
-- 5. CAR DETAILS
-- ---------------------------------------------------------------------
INSERT INTO cars (id, item_id, brand, model, year, fuel_type, seating_capacity, transmission, registration_number, mileage) VALUES
(1, 1, 'Tata', 'Harrier XZA+', 2023, 'Diesel', 5, 'Automatic', 'KA-01-MJ-4512', '16.3 kmpl'),
(2, 2, 'Hyundai', 'Creta SX(O)', 2023, 'Petrol', 5, 'Automatic', 'MH-02-EZ-9871', '17.0 kmpl'),
(3, 3, 'Mahindra', 'Thar LX 4x4', 2022, 'Diesel', 4, 'Manual', 'MH-12-TR-3003', '15.2 kmpl'),
(4, 4, 'Maruti Suzuki', 'Swift ZXi+', 2023, 'Petrol', 5, 'Manual', 'DL-03-CB-7819', '22.5 kmpl'),
(5, 5, 'Toyota', 'Fortuner 4x4', 2024, 'Diesel', 7, 'Automatic', 'TS-09-XY-5555', '14.2 kmpl'),
(6, 6, 'Honda', 'City ZX CVT', 2022, 'Petrol', 5, 'Automatic', 'TN-07-BL-6204', '18.4 kmpl'),
(7, 7, 'Kia', 'Seltos GTX+', 2023, 'Diesel', 5, 'Automatic', 'GJ-01-KR-8120', '18.0 kmpl'),
(8, 8, 'MG', 'Hector Sharp Pro', 2023, 'Petrol', 5, 'Automatic', 'WB-02-AK-1902', '14.5 kmpl'),
(9, 9, 'Mahindra', 'XUV700 AX7', 2024, 'Petrol', 7, 'Automatic', 'KA-03-NV-2024', '13.0 kmpl'),
(10, 10, 'Tata', 'Nexon EV Max', 2023, 'Electric', 5, 'Automatic', 'MH-04-EV-9009', '453 km/charge');

-- ---------------------------------------------------------------------
-- 6. BIKE DETAILS
-- ---------------------------------------------------------------------
INSERT INTO bikes (id, item_id, brand, model, year, engine_capacity_cc, bike_type, registration_number, helmet_included) VALUES
(1, 11, 'Royal Enfield', 'Classic 350', 2023, 349, 'Cruiser', 'GA-03-RE-3501', 1),
(2, 12, 'Yamaha', 'YZF R15 V4', 2023, 155, 'Sports', 'MH-12-R-1504', 1),
(3, 13, 'KTM', '390 Duke', 2022, 373, 'Sports', 'KA-05-KT-3900', 1),
(4, 14, 'Honda', 'Activa 6G', 2023, 109, 'Scooter', 'DL-08-AC-6612', 1),
(5, 15, 'Bajaj', 'Pulsar NS200', 2023, 199, 'Commuter', 'TS-08-NS-2020', 1),
(6, 16, 'TVS', 'Apache RTR 160 4V', 2023, 160, 'Commuter', 'TN-09-AP-4160', 1),
(7, 17, 'Suzuki', 'Access 125', 2023, 124, 'Scooter', 'WB-01-SZ-1250', 1),
(8, 18, 'Royal Enfield', 'Himalayan 450', 2024, 452, 'Adventure', 'CH-01-HM-4509', 1),
(9, 19, 'Ather', '450X Gen 3', 2023, 0, 'Scooter', 'KA-04-AT-4503', 1),
(10, 20, 'Jawa', '42 Bobber', 2023, 334, 'Cruiser', 'RJ-14-JW-4201', 1);

-- ---------------------------------------------------------------------
-- 7. HOUSE DETAILS
-- ---------------------------------------------------------------------
INSERT INTO houses (id, item_id, property_type, bedrooms, bathrooms, furnishing, area_sqft, address_line, parking_available) VALUES
(1, 21, 'Apartment', 2, 2, 'Furnished', 1250, 'Flat 1102, Prestige Palms, 100ft Road, Indiranagar', 1),
(2, 22, 'Penthouse', 3, 3, 'Furnished', 2400, '14th Floor, Sea View Towers, Carter Road, Bandra West', 1),
(3, 23, 'Studio', 1, 1, 'Furnished', 580, 'Unit 304, Megapolis Splendour, Phase 3, Hinjewadi', 1),
(4, 24, 'Villa', 4, 4, 'Furnished', 3800, 'Villa No 12, Golden Sands Enclave, ECR', 1),
(5, 25, 'Apartment', 2, 2, 'Semi-Furnished', 1420, 'A-801, My Home Bhooja, Knowledge City, Hitec City', 1),
(6, 26, 'Apartment', 3, 3, 'Furnished', 2150, 'Tower B-1802, The Camellias, Golf Course Road', 1),
(7, 27, 'Apartment', 1, 1, 'Furnished', 720, 'Top Floor, 5th Cross, 6th Block, Koramangala', 0),
(8, 28, 'Independent House', 3, 3, 'Semi-Furnished', 2200, 'House 45, Vibhuti Khand, Near High Court, Gomti Nagar', 1),
(9, 29, 'Apartment', 2, 2, 'Furnished', 1100, 'Block CF-12, Sector 1, Salt Lake City', 1),
(10, 30, 'Villa', 4, 4, 'Furnished', 3200, 'Casa de Praia, Near Fort Aguada, Candolim Beach Road', 1);

-- ---------------------------------------------------------------------
-- 8. FURNITURE DETAILS
-- ---------------------------------------------------------------------
INSERT INTO furniture (id, item_id, furniture_type, material, color, item_condition, dimensions, assembly_required) VALUES
(1, 31, 'Living Room', 'Top-grain Leatherette', 'Mocha Brown', 'Brand New', '88" W x 38" D x 40" H', 0),
(2, 32, 'Bedroom', 'Solid Sheesham Rosewood', 'Teak Gloss', 'Like New', '76" W x 82" L x 44" H', 1),
(3, 33, 'Office', 'Bionic Mesh & Aluminum', 'Jet Black', 'Brand New', '26" W x 26" D x 48" H', 0),
(4, 34, 'Dining', 'Seasoned Mango Wood', 'Walnut Brown', 'Like New', '66" L x 36" W x 30" H', 1),
(5, 35, 'Office', 'Engineered Oak & Steel', 'Nordic White / Oak', 'Good', '48" W x 24" D x 30" H', 1),
(6, 36, 'Living Room', 'Velvet Chenille Fabric', 'Teal Blue', 'Brand New', '104" W x 65" D x 34" H', 0),
(7, 37, 'Bedroom', 'Melamine Faced Chipboard', 'Dark Wenge', 'Good', '64" W x 22" D x 76" H', 1),
(8, 38, 'Living Room', 'Tempered Glass & Oak', 'Clear / Natural Oak', 'Brand New', '42" L x 24" W x 18" H', 0),
(9, 39, 'Bedroom', 'Pine Wood & Foam', 'Charcoal Grey', 'Like New', '60" W x 78" L x 36" H', 1),
(10, 40, 'Storage', 'Cast Iron & Solid Pine', 'Industrial Black / Pine', 'Like New', '36" W x 14" D x 72" H', 0);

-- ---------------------------------------------------------------------
-- 9. BOOKINGS (Completed, Active, and Confirmed)
-- ---------------------------------------------------------------------
INSERT INTO bookings (id, booking_code, customer_id, item_id, start_date, end_date, rental_days, daily_rate, base_rent, discount_rate, discount_amount, security_deposit, total_amount, booking_status) VALUES
(1, 'BK-2026-001', 1, 1, '2026-10-01', '2026-10-05', 4, 3500.00, 14000.00, 0.00, 0.00, 10000.00, 24000.00, 'completed'),
(2, 'BK-2026-002', 2, 11, '2026-10-10', '2026-10-15', 5, 1100.00, 5500.00, 5.00, 275.00, 3000.00, 8225.00, 'completed'),
(3, 'BK-2026-003', 3, 3, '2026-11-05', '2026-11-12', 7, 3200.00, 22400.00, 10.00, 2240.00, 12000.00, 32160.00, 'active'),
(4, 'BK-2026-004', 4, 13, '2026-11-06', '2026-11-10', 4, 1600.00, 6400.00, 0.00, 0.00, 5000.00, 11400.00, 'active'),
(5, 'BK-2026-005', 5, 23, '2026-11-01', '2026-11-30', 29, 1200.00, 34800.00, 15.00, 5220.00, 8000.00, 37580.00, 'active'),
(6, 'BK-2026-006', 6, 35, '2026-11-04', '2026-11-18', 14, 120.00, 1680.00, 10.00, 168.00, 800.00, 2312.00, 'active'),
(7, 'BK-2026-007', 7, 2, '2026-11-15', '2026-11-20', 5, 2800.00, 14000.00, 5.00, 700.00, 8000.00, 21300.00, 'confirmed'),
(8, 'BK-2026-008', 8, 21, '2026-11-20', '2026-11-25', 5, 2200.00, 11000.00, 5.00, 550.00, 15000.00, 25450.00, 'confirmed');

-- ---------------------------------------------------------------------
-- 10. PAYMENTS (Demo Transactions for Bookings)
-- ---------------------------------------------------------------------
INSERT INTO payments (id, transaction_id, booking_id, customer_id, amount, payment_method, payment_type, payment_status, remarks) VALUES
(1, 'TXN-UPI-982341201', 1, 1, 24000.00, 'UPI Demo', 'Rental Payment', 'success', 'Demo UPI payment via Google Pay'),
(2, 'TXN-CRD-451298302', 2, 2, 8225.00, 'Card Demo', 'Rental Payment', 'success', 'Demo Visa credit card payment'),
(3, 'TXN-UPI-771239843', 3, 3, 32160.00, 'UPI Demo', 'Rental Payment', 'success', 'Demo PhonePe UPI payment'),
(4, 'TXN-CSH-102938474', 4, 4, 11400.00, 'Cash', 'Rental Payment', 'success', 'Direct cash payment received at outlet'),
(5, 'TXN-CRD-998877665', 5, 5, 37580.00, 'Card Demo', 'Rental Payment', 'success', 'Demo Mastercard payment'),
(6, 'TXN-UPI-554433221', 6, 6, 2312.00, 'UPI Demo', 'Rental Payment', 'success', 'Demo Paytm UPI payment'),
(7, 'TXN-UPI-889900112', 7, 7, 21300.00, 'UPI Demo', 'Rental Payment', 'success', 'Advance payment for booking confirmed'),
(8, 'TXN-REF-112233441', 1, 1, 10000.00, 'UPI Demo', 'Deposit Refund', 'refunded', 'Full security deposit refunded upon vehicle check-in');

-- ---------------------------------------------------------------------
-- 11. INVOICES
-- ---------------------------------------------------------------------
INSERT INTO invoices (id, invoice_number, booking_id, issue_date, due_date, subtotal, discount_amount, tax_amount, security_deposit, total_amount, paid_amount, payment_status) VALUES
(1, 'INV-2026-0001', 1, '2026-10-01', '2026-10-01', 14000.00, 0.00, 0.00, 10000.00, 24000.00, 24000.00, 'paid'),
(2, 'INV-2026-0002', 2, '2026-10-10', '2026-10-10', 5500.00, 275.00, 0.00, 3000.00, 8225.00, 8225.00, 'paid'),
(3, 'INV-2026-0003', 3, '2026-11-05', '2026-11-05', 22400.00, 2240.00, 0.00, 12000.00, 32160.00, 32160.00, 'paid'),
(4, 'INV-2026-0004', 4, '2026-11-06', '2026-11-06', 6400.00, 0.00, 0.00, 5000.00, 11400.00, 11400.00, 'paid'),
(5, 'INV-2026-0005', 5, '2026-11-01', '2026-11-01', 34800.00, 5220.00, 0.00, 8000.00, 37580.00, 37580.00, 'paid'),
(6, 'INV-2026-0006', 6, '2026-11-04', '2026-11-04', 1680.00, 168.00, 0.00, 800.00, 2312.00, 2312.00, 'paid'),
(7, 'INV-2026-0007', 7, '2026-11-15', '2026-11-15', 14000.00, 700.00, 0.00, 8000.00, 21300.00, 21300.00, 'paid'),
(8, 'INV-2026-0008', 8, '2026-11-20', '2026-11-20', 11000.00, 550.00, 0.00, 15000.00, 25450.00, 0.00, 'unpaid');

-- ---------------------------------------------------------------------
-- 12. RETURNS (Check-in Audit Records)
-- ---------------------------------------------------------------------
INSERT INTO returns (id, booking_id, return_code, actual_return_date, expected_return_date, overdue_days, daily_late_fee, total_late_fee, damage_charges, item_condition, notes, deposit_refunded, processed_by_admin_id) VALUES
(1, 1, 'RET-2026-0001', '2026-10-05', '2026-10-05', 0, 0.00, 0.00, 0.00, 'Excellent', 'Vehicle returned on time in immaculate condition with full tank fuel.', 10000.00, 1),
(2, 2, 'RET-2026-0002', '2026-10-16', '2026-10-15', 1, 300.00, 300.00, 0.00, 'Good', 'Returned 1 day late due to rain. 1-day late fee of Rs 300 deducted from deposit.', 2700.00, 2);

-- ---------------------------------------------------------------------
-- 13. ACTIVITY LOGS
-- ---------------------------------------------------------------------
INSERT INTO activity_logs (id, user_id, action, details, ip_address) VALUES
(1, 1, 'SYSTEM_INIT', 'Smart Rental Management System database successfully seeded with initial catalog and users.', '127.0.0.1'),
(2, 3, 'USER_LOGIN', 'Aarav Sharma logged in via web portal.', '127.0.0.1'),
(3, 3, 'BOOKING_CREATE', 'Created booking BK-2026-001 for Tata Harrier XZA+.', '127.0.0.1'),
(4, 3, 'PAYMENT_COMPLETE', 'Demo UPI payment TXN-UPI-982341201 verified for BK-2026-001.', '127.0.0.1'),
(5, 1, 'ITEM_RETURN_PROCESS', 'Admin Rajesh processed check-in RET-2026-0001 for Tata Harrier. Deposit refunded.', '127.0.0.1'),
(6, 4, 'USER_LOGIN', 'Priya Patel logged in via web portal.', '127.0.0.1'),
(7, 5, 'BOOKING_CREATE', 'Rohan Deshmukh booked Mahindra Thar 4x4 for off-road tour.', '127.0.0.1'),
(8, 2, 'ADMIN_REPORT_VIEW', 'Admin Kavita viewed monthly revenue and fleet occupancy analytics.', '127.0.0.1');

SET FOREIGN_KEY_CHECKS = 1;
