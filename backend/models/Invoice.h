#ifndef INVOICE_H
#define INVOICE_H

#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Encapsulation & Billing Calculations
 * Invoice generates comprehensive customer bill breakdown.
 * ============================================================================
 */
class Invoice {
private:
    int m_id;
    std::string m_invoiceNumber;
    int m_bookingId;
    std::string m_issueDate;
    std::string m_dueDate;
    double m_subtotal;
    double m_discountAmount;
    double m_taxAmount;
    double m_securityDeposit;
    double m_totalAmount;
    double m_paidAmount;
    std::string m_paymentStatus; // paid, partial, unpaid
    std::string m_createdAt;

    // Optional metadata
    std::string m_bookingCode;
    std::string m_customerName;
    std::string m_itemTitle;

public:
    Invoice();
    Invoice(int id, const std::string& invNum, int bookingId,
            const std::string& issueDate, const std::string& dueDate,
            double subtotal, double discount, double tax,
            double deposit, double total, double paid = 0.0,
            const std::string& status = "unpaid",
            const std::string& createdAt = "");

    static std::string generateInvoiceNumber();

    int getId() const;
    std::string getInvoiceNumber() const;
    int getBookingId() const;
    std::string getIssueDate() const;
    std::string getDueDate() const;
    double getSubtotal() const;
    double getDiscountAmount() const;
    double getTaxAmount() const;
    double getSecurityDeposit() const;
    double getTotalAmount() const;
    double getPaidAmount() const;
    std::string getPaymentStatus() const;
    std::string getCreatedAt() const;

    std::string getBookingCode() const;
    std::string getCustomerName() const;
    std::string getItemTitle() const;

    void setId(int id);
    void setInvoiceNumber(const std::string& num);
    void setBookingId(int id);
    void setIssueDate(const std::string& d);
    void setDueDate(const std::string& d);
    void setSubtotal(double amt);
    void setDiscountAmount(double amt);
    void setTaxAmount(double amt);
    void setSecurityDeposit(double amt);
    void setTotalAmount(double amt);
    void setPaidAmount(double amt);
    void setPaymentStatus(const std::string& status);
    void setCreatedAt(const std::string& created);

    void setBookingCode(const std::string& code);
    void setCustomerName(const std::string& name);
    void setItemTitle(const std::string& title);

    std::string toJson() const;
};

#endif // INVOICE_H
