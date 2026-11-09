import subprocess
import time
import os
import sys
import urllib.request
import urllib.error

def test_cors():
    print("--- TESTING CORS ORIGIN REJECTION ---")
    base_url = "http://127.0.0.1:8080/api/health"
    
    # 1. Trusted origin OPTIONS preflight
    req = urllib.request.Request(base_url, headers={"Origin": "http://localhost:8080"}, method="OPTIONS")
    with urllib.request.urlopen(req) as resp:
        assert resp.status in (200, 204), f"Expected 200 or 204 for trusted origin OPTIONS, got {resp.status}"
        allow_origin = resp.headers.get("Access-Control-Allow-Origin")
        assert allow_origin == "http://localhost:8080", f"Expected Access-Control-Allow-Origin: http://localhost:8080, got {allow_origin}"
    print(f"[PASS] CORS preflight for trusted origin (http://localhost:8080) -> {resp.status} with header")

    # 2. Untrusted origin OPTIONS preflight (must be rejected with 403)
    req = urllib.request.Request(base_url, headers={"Origin": "http://evil-attacker.com"}, method="OPTIONS")
    try:
        urllib.request.urlopen(req)
        assert False, "Expected 403 HTTPError for untrusted origin preflight"
    except urllib.error.HTTPError as e:
        assert e.code == 403, f"Expected 403 Forbidden, got {e.code}"
        assert e.headers.get("Access-Control-Allow-Origin") is None, "Untrusted origin must receive NO Access-Control-Allow-Origin"
    print("[PASS] CORS preflight for untrusted origin (http://evil-attacker.com) -> 403 Forbidden rejected")

    # 3. Untrusted origin GET request (must omit Access-Control-Allow-Origin)
    req = urllib.request.Request(base_url, headers={"Origin": "http://malicious-site.org"}, method="GET")
    with urllib.request.urlopen(req) as resp:
        assert resp.status == 200
        allow_origin = resp.headers.get("Access-Control-Allow-Origin")
        assert allow_origin is None, f"Expected NO Access-Control-Allow-Origin for untrusted GET, got {allow_origin}"
    print("[PASS] Untrusted origin GET request received NO Access-Control-Allow-Origin header")

def main():
    env = os.environ.copy()
    env["PATH"] = r"C:\mingw64\mingw64\bin;C:\Program Files\MySQL\MySQL Server 8.0\lib;" + env.get("PATH", "")
    
    exe_path = os.path.abspath("./build/SmartRentalManagement.exe")
    print(f"Starting server: {exe_path}")
    proc = subprocess.Popen([exe_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env)
    
    try:
        # Wait for server to come up
        ready = False
        for _ in range(30):
            try:
                with urllib.request.urlopen("http://127.0.0.1:8080/api/health", timeout=1) as resp:
                    if resp.status == 200:
                        ready = True
                        break
            except Exception:
                time.sleep(0.5)
        
        if not ready:
            print("ERROR: Server failed to start within 15 seconds")
            proc.kill()
            out, err = proc.communicate()
            print("Stdout:", out.decode("utf-8", errors="replace"))
            print("Stderr:", err.decode("utf-8", errors="replace"))
            sys.exit(1)
            
        print("Server is UP and ready!\n")
        
        # Test CORS
        test_cors()
        print()

        # Run E2E suite
        import test_api_suite
        test_api_suite.run_e2e_tests()
        print()

        # Run Concurrency Double Booking suite
        import test_concurrency_double_booking
        test_concurrency_double_booking.test_genuine_concurrent_double_booking()
        print()

        print("========================================================")
        print(" ALL TEST SUITES (CORS + E2E + CONCURRENCY) PASSED!")
        print("========================================================")

    finally:
        print("Shutting down server...")
        proc.terminate()
        try:
            proc.wait(timeout=3)
        except subprocess.TimeoutExpired:
            proc.kill()
        print("Server shutdown complete.")

if __name__ == "__main__":
    main()

