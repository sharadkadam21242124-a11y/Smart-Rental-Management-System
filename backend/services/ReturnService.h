#ifndef RETURN_SERVICE_H
#define RETURN_SERVICE_H

#include "../database/PaymentRepository.h"
#include <memory>
#include <vector>
#include <string>

class ReturnService {
private:
    PaymentRepository& m_payRepo;

public:
    explicit ReturnService(PaymentRepository& repo);

    std::shared_ptr<RentalReturn> processCheckIn(
        int bookingId,
        const std::string& actualReturnDate,
        double damageCharges = 0.0,
        const std::string& condition = "Good",
        const std::string& notes = "",
        int adminId = 1
    );

    std::shared_ptr<RentalReturn> getReturnByBookingId(int bookingId);
    std::vector<std::shared_ptr<RentalReturn>> getAllReturns();
};

#endif // RETURN_SERVICE_H
