#ifndef RENTAL_RETURN_H
#define RENTAL_RETURN_H

#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Encapsulation & Business Rules Implementation
 * RentalReturn handles item inspection, late fee calculation, damage assessment,
 * and security deposit refund/deduction settlements.
 * ============================================================================
 */
class RentalReturn {
private:
    int m_id;
    int m_bookingId;
    std::string m_returnCode;
    std::string m_actualReturnDate;
    std::string m_expectedReturnDate;
    int m_overdueDays;
    double m_dailyLateFee;
    double m_totalLateFee;
    double m_damageCharges;
    std::string m_itemCondition; // Excellent, Good, Fair, Damaged
    std::string m_notes;
    double m_depositRefunded;
    int m_processedByAdminId;
    std::string m_createdAt;

    // Optional metadata
    std::string m_bookingCode;
    std::string m_itemTitle;
    std::string m_customerName;

public:
    RentalReturn();
    RentalReturn(int id, int bookingId, const std::string& code,
                 const std::string& actualDate, const std::string& expectedDate,
                 int overdueDays, double dailyLateFee, double totalLateFee,
                 double damageCharges, const std::string& condition = "Good",
                 const std::string& notes = "", double depositRefunded = 0.0,
                 int processedByAdminId = 1, const std::string& createdAt = "");

    static std::string generateReturnCode();

    // Business Logic Method: Computes late fee and net refund
    static void computeSettlement(double deposit, int overdueDays, double dailyLateRate,
                                  double damageCharges, double& outLateFee, double& outRefund);

    int getId() const;
    int getBookingId() const;
    std::string getReturnCode() const;
    std::string getActualReturnDate() const;
    std::string getExpectedReturnDate() const;
    int getOverdueDays() const;
    double getDailyLateFee() const;
    double getTotalLateFee() const;
    double getDamageCharges() const;
    std::string getItemCondition() const;
    std::string getNotes() const;
    double getDepositRefunded() const;
    int getProcessedByAdminId() const;
    std::string getCreatedAt() const;

    std::string getBookingCode() const;
    std::string getItemTitle() const;
    std::string getCustomerName() const;

    void setId(int id);
    void setBookingId(int id);
    void setReturnCode(const std::string& code);
    void setActualReturnDate(const std::string& d);
    void setExpectedReturnDate(const std::string& d);
    void setOverdueDays(int days);
    void setDailyLateFee(double fee);
    void setTotalLateFee(double fee);
    void setDamageCharges(double charges);
    void setItemCondition(const std::string& condition);
    void setNotes(const std::string& notes);
    void setDepositRefunded(double refund);
    void setProcessedByAdminId(int adminId);
    void setCreatedAt(const std::string& created);

    void setBookingCode(const std::string& code);
    void setItemTitle(const std::string& title);
    void setCustomerName(const std::string& name);

    std::string toJson() const;
};

#endif // RENTAL_RETURN_H
