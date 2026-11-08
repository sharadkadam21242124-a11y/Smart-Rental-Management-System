#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include "../database/PaymentRepository.h"
#include "../database/RentalRepository.h"
#include "../database/BookingRepository.h"
#include "../database/UserRepository.h"
#include <string>
#include <vector>
#include <map>

class ReportService {
private:
    PaymentRepository& m_payRepo;
    RentalRepository& m_rentalRepo;
    BookingRepository& m_bookingRepo;
    UserRepository& m_userRepo;

public:
    ReportService(PaymentRepository& pRepo, RentalRepository& rRepo,
                  BookingRepository& bRepo, UserRepository& uRepo);

    std::string getDashboardStatsJson();
    std::string getRevenueReportJson();
    std::string getCategoryReportJson();

    // Export Reports to CSV Format
    std::string exportRevenueReportCsv();
    std::string exportBookingsReportCsv();
    std::string exportInventoryReportCsv();
};

#endif // REPORT_SERVICE_H
