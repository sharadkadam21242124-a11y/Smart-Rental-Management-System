import json
import urllib.request
import urllib.error
import time
import sys

BASE_URL = "http://127.0.0.1:8080/api"

def api_call(path, method="GET", data=None, token=None):
    url = f"{BASE_URL}{path}"
    headers = {"Content-Type": "application/json"}
    if token:
        headers["Authorization"] = f"Bearer {token}"
    body = json.dumps(data).encode("utf-8") if data else None
    req = urllib.request.Request(url, data=body, headers=headers, method=method)
    try:
        with urllib.request.urlopen(req) as resp:
            return resp.status, json.loads(resp.read().decode("utf-8"))
    except urllib.error.HTTPError as e:
        raw = e.read().decode("utf-8")
        try:
            return e.code, json.loads(raw)
        except Exception:
            return e.code, {"error": raw}

def run_e2e_tests():
    print("=== SMART RENTAL MANAGEMENT SYSTEM: END-TO-END SECURITY & API SUITE ===")
    
    # 1. Health check
    status, res = api_call("/health")
    assert status == 200, f"Health check failed: {res}"
    print("[PASS] 1. API Health Check ->", res)

    # 2. Login as Admin
    # Note: admin password in database is password123 (seed hash)
    status, admin_res = api_call("/login", "POST", {
        "email": "admin@smartrental.com",
        "password": "password123"
    })
    assert status == 200 and admin_res.get("success"), f"Admin login failed: {admin_res}"
    admin_token = admin_res["token"]
    assert admin_token.startswith("SRMS-") and len(admin_token) >= 64, f"Invalid CSPRNG token: {admin_token}"
    print(f"[PASS] 2. Admin Login with CSPRNG Token ({len(admin_token)} chars) -> OK")

    # 3. Customer 1 (Aarav Sharma) Login
    status, cust1_res = api_call("/login", "POST", {
        "email": "aarav.sharma@gmail.com",
        "password": "password123"
    })
    assert status == 200 and cust1_res.get("success"), f"Customer 1 login failed: {cust1_res}"
    cust1_token = cust1_res["token"]
    cust1_id = cust1_res["user"]["account_id"]
    print(f"[PASS] 3. Customer 1 Login (Account #{cust1_id}) -> OK")

    # 4. Customer 2 (Priya Patel) Login
    status, cust2_res = api_call("/login", "POST", {
        "email": "priya.patel@gmail.com",
        "password": "password123"
    })
    assert status == 200 and cust2_res.get("success"), f"Customer 2 login failed: {cust2_res}"
    cust2_token = cust2_res["token"]
    cust2_id = cust2_res["user"]["account_id"]
    print(f"[PASS] 4. Customer 2 Login (Account #{cust2_id}) -> OK")

    # 5. Authorization: Non-admin accessing admin endpoint
    status, non_admin_res = api_call("/admin/dashboard", "GET", token=cust1_token)
    assert status == 403, f"Expected 403 Forbidden for non-admin on admin endpoint, got {status}: {non_admin_res}"
    print("[PASS] 5. Admin Endpoint Authorization Barrier (403 Forbidden for Customer) -> OK")

    # 6. Date Validation: Start date in past
    status, past_date_res = api_call("/bookings", "POST", {
        "item_id": 1,
        "start_date": "2020-01-01",
        "end_date": "2020-01-05"
    }, token=cust1_token)
    assert status == 400, f"Expected 400 Bad Request for past dates, got {status}: {past_date_res}"
    print("[PASS] 6. Date Validation: Past Start Date Rejected (400 Bad Request) -> OK")

    # 7. Date Validation: Start date >= end date
    status, inv_dates_res = api_call("/bookings", "POST", {
        "item_id": 1,
        "start_date": "2026-12-10",
        "end_date": "2026-12-05"
    }, token=cust1_token)
    assert status == 400, f"Expected 400 Bad Request for start >= end date, got {status}: {inv_dates_res}"
    print("[PASS] 7. Date Validation: Reversed Dates Rejected (400 Bad Request) -> OK")

    # 8. Valid Booking by Customer 1
    # Use dates in late 2026 to avoid any conflict
    target_start = "2026-12-01"
    target_end   = "2026-12-08"
    status, book_res = api_call("/bookings", "POST", {
        "item_id": 1,
        "start_date": target_start,
        "end_date": target_end
    }, token=cust1_token)
    assert status == 201 and book_res.get("success"), f"Booking creation failed: {book_res}"
    booking_id = book_res["booking"]["id"]
    print(f"[PASS] 8. Customer 1 Created Booking #{booking_id} for Dates {target_start} to {target_end} -> OK")

    # 9. Concurrency & Double Booking Collision Prevention:
    # Customer 2 attempts to book the SAME item for overlapping dates (Dec 03 to Dec 06)
    status, conflict_res = api_call("/bookings", "POST", {
        "item_id": 1,
        "start_date": "2026-12-03",
        "end_date": "2026-12-06"
    }, token=cust2_token)
    assert status == 409, f"Expected 409 Conflict for overlapping double booking, got {status}: {conflict_res}"
    print(f"[PASS] 9. Double Booking Collision Prevented: 409 Conflict returned -> {conflict_res.get('error')}")

    # 10. Invoice Endpoint Security (Unauthenticated access rejected)
    status, unauth_inv = api_call(f"/invoices/{booking_id}", "GET")
    assert status == 401, f"Expected 401 Unauthorized for unauthenticated invoice request, got {status}: {unauth_inv}"
    print("[PASS] 10. Invoice Endpoint Authentication Barrier (401 Unauthorized without token) -> OK")

    # 11. Cross-Customer Invoice Isolation (Customer 2 trying to view Customer 1's invoice)
    status, cross_inv = api_call(f"/invoices/{booking_id}", "GET", token=cust2_token)
    assert status == 403, f"Expected 403 Forbidden for cross-customer invoice view, got {status}: {cross_inv}"
    print("[PASS] 11. Cross-Customer Invoice Isolation (403 Forbidden for unauthorized customer) -> OK")

    # 12. Customer 1 viewing own invoice
    status, own_inv = api_call(f"/invoices/{booking_id}", "GET", token=cust1_token)
    assert status == 200 and own_inv.get("success"), f"Customer 1 viewing own invoice failed: {own_inv}"
    print(f"[PASS] 12. Customer 1 Viewing Own Invoice -> OK (Invoice #{own_inv['invoice']['invoice_number']})")

    # 13. Cross-Customer Payment Isolation:
    # Customer 2 attempting to pay for Customer 1's booking
    status, cross_pay = api_call("/payments", "POST", {
        "booking_id": booking_id,
        "amount": 5000.0,
        "payment_method": "UPI Demo",
        "payment_type": "Rental Payment"
    }, token=cust2_token)
    assert status == 403, f"Expected 403 Forbidden for cross-customer payment, got {status}: {cross_pay}"
    print("[PASS] 13. Cross-Customer Payment Prevention (403 Forbidden for wrong customer) -> OK")

    # 14. Customer 1 Paying for Own Booking
    status, own_pay = api_call("/payments", "POST", {
        "booking_id": booking_id,
        "amount": own_inv["invoice"]["total_amount"],
        "payment_method": "UPI Demo",
        "payment_type": "Rental Payment",
        "remarks": "E2E Automated Test Payment"
    }, token=cust1_token)
    assert status == 201 and own_pay.get("success"), f"Payment failed: {own_pay}"
    assert own_pay.get("is_demo_gateway") is True, "Must identify as simulated demo gateway"
    print(f"[PASS] 14. Customer 1 Payment Recorded -> OK (Txn #{own_pay['payment']['transaction_id']})")

    # 15. Admin Check-in Return Processing & Security Deposit Refund
    status, return_res = api_call("/returns", "POST", {
        "booking_id": booking_id,
        "actual_return_date": "2026-12-08",
        "damage_charges": 0.0,
        "item_condition": "Excellent",
        "notes": "E2E Test Return Settlement"
    }, token=admin_token)
    assert status == 201 and return_res.get("success"), f"Return processing failed: {return_res}"
    print(f"[PASS] 15. Admin Return Settlement Completed -> Refunded ₹{return_res['return']['deposit_refunded']}")

    # 16. Verify Item Availability Restored to Available After Return
    status, item_res = api_call("/items/1", "GET")
    assert status == 200, f"Get item failed: {item_res}"
    assert item_res["item"]["status"] == "available", f"Item status should be available, got {item_res['item']['status']}"
    print("[PASS] 16. Rental Item Status Restored to 'available' -> OK")

    print("\n========================================================")
    print(" ALL 16 LIVE E2E SECURITY & INTEGRATION TESTS PASSED!")
    print("========================================================\n")

if __name__ == "__main__":
    run_e2e_tests()
