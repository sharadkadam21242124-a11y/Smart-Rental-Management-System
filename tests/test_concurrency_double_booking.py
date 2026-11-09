import json
import urllib.request
import urllib.error
import concurrent.futures
import time
import random
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

def test_genuine_concurrent_double_booking():
    print("--- CONCURRENCY AUDIT: TESTING SIMULTANEOUS DOUBLE-BOOKINGS ---")
    
    # 1. Login as customer
    status, cust_res = api_call("/login", "POST", {
        "email": "aarav.sharma@gmail.com",
        "password": "password123"
    })
    assert status == 200, f"Customer login failed: {cust_res}"
    token = cust_res["token"]

    # Use item 4 (Executive Chair) for a completely fresh 2027 window
    test_item_id = 4
    test_start = "2027-02-10"
    test_end   = "2027-02-15"

    print(f"Submitting 10 SIMULTANEOUS concurrent requests to book Item #{test_item_id} [{test_start} to {test_end}]...")

    results = []
    
    def fire_booking(req_index):
        return api_call("/bookings", "POST", {
            "item_id": test_item_id,
            "start_date": test_start,
            "end_date": test_end
        }, token=token)

    # Launch 10 worker threads concurrently
    with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
        futures = [executor.submit(fire_booking, i) for i in range(10)]
        for f in concurrent.futures.as_completed(futures):
            results.append(f.result())

    status_codes = [status for status, _ in results]
    success_count = status_codes.count(201)
    conflict_count = status_codes.count(409)

    print(f"Results across 10 concurrent threads: {status_codes}")
    print(f"Success count (201): {success_count} | Conflict count (409): {conflict_count}")

    # ACID GUARANTEE: Exactly 1 request succeeds, all other 9 must be rejected with 409 Conflict
    assert success_count == 1, f"ACID Violation! Expected exactly 1 successful booking, but got {success_count}"
    assert conflict_count == 9, f"Expected 9 conflict responses (409), but got {conflict_count}"

    # Find the successful booking ID to cleanly release it
    successful_booking = next(res["booking"] for status, res in results if status == 201)
    booking_id = successful_booking["id"]
    print(f"[PASS] ACID Concurrency Check PASSED: Exactly 1 reservation created (#{booking_id}), 9 conflicts prevented.")

    # Clean up: cancel the test booking to release item
    cancel_status, cancel_res = api_call(f"/bookings/{booking_id}/cancel", "PUT", token=token)
    assert cancel_status == 200, f"Cleanup cancellation failed: {cancel_res}"
    print(f"[PASS] Cleaned up test booking #{booking_id}; Item #{test_item_id} released.")

if __name__ == "__main__":
    test_genuine_concurrent_double_booking()
