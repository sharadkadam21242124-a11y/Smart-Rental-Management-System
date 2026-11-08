#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Encapsulation, Static Methods, Demonstration Transaction Log
 * Records demonstration financial transactions (Cash, UPI Demo, Card Demo).
 * ============================================================================
 */
class Payment {
private:
    int m_id;
    std::string m_transactionId;
    int m_bookingId;
    int m_customerId;
    double m_amount;
    std::string m_paymentMethod; // Cash, UPI Demo, Card Demo
    std::string m_paymentType;   // Rental Payment, Security Deposit, Late Fee, Deposit Refund
    std::string m_paymentStatus; // success, pending, failed, refunded
    std::string m_paymentDate;
    std::string m_remarks;

    // Optional cached attributes for UI
    std::string m_customerName;
    std::string m_bookingCode;

public:
    Payment();
    Payment(int id, const std::string& transId, int bookingId, int customerId,
            double amount, const std::string& method,
            const std::string& type = "Rental Payment",
            const std::string& status = "success",
            const std::string& date = "",
            const std::string& remarks = "Demo Transaction Processed Successfully");

    // Static helper to generate unique transaction ID
    static std::string generateTransactionId(const std::string& method);

    // Getters
    int getId() const;
    std::string getTransactionId() const;
    int getBookingId() const;
    int getCustomerId() const;
    double getAmount() const;
    std::string getPaymentMethod() const;
    std::string getPaymentType() const;
    std::string getPaymentStatus() const;
    std::string getPaymentDate() const;
    std::string getRemarks() const;

    std::string getCustomerName() const;
    std::string getBookingCode() const;

    // Setters
    void setId(int id);
    void setTransactionId(const std::string& id);
    void setBookingId(int id);
    void setCustomerId(int id);
    void setAmount(double amt);
    void setPaymentMethod(const std::string& method);
    void setPaymentType(const std::string& type);
    void setPaymentStatus(const std::string& status);
    void setPaymentDate(const std::string& date);
    void setRemarks(const std::string& remarks);

    void setCustomerName(const std::string& name);
    void setBookingCode(const std::string& code);

    std::string toJson() const;
};

#endif // PAYMENT_H
