#include "ReportService.h"
#include "../utils/JsonHelper.h"
#include <sstream>

ReportService::ReportService(PaymentRepository& pRepo, RentalRepository& rRepo,
                             BookingRepository& bRepo, UserRepository& uRepo)
    : m_payRepo(pRepo), m_rentalRepo(rRepo), m_bookingRepo(bRepo), m_userRepo(uRepo) {}

std::string ReportService::getDashboardStatsJson() {
    FinancialSummary fin = m_payRepo.getFinancialSummary();
    int totalItems = m_rentalRepo.getTotalItemCount();
    int availItems = m_rentalRepo.getAvailableItemCount();
    int rentedItems = m_rentalRepo.getRentedItemCount();
    int totalCustomers = m_userRepo.getTotalCustomerCount();
    int totalBookings = m_bookingRepo.getTotalBookingCount();
    int activeBookings = m_bookingRepo.getActiveBookingCount();
    int pendingBookings = m_bookingRepo.getPendingBookingCount();

    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("total_items", totalItems)
       << JsonHelper::kv("available_items", availItems)
       << JsonHelper::kv("rented_items", rentedItems)
       << JsonHelper::kv("total_customers", totalCustomers)
       << JsonHelper::kv("total_bookings", totalBookings)
       << JsonHelper::kv("active_bookings", activeBookings)
       << JsonHelper::kv("pending_bookings", pendingBookings)
       << JsonHelper::kv("earned_rental_revenue", fin.totalEarnedRentalRevenue)
       << JsonHelper::kv("security_deposits_held", fin.totalDepositsHeld)
       << JsonHelper::kv("security_deposits_refunded", fin.totalDepositsRefunded)
       << JsonHelper::kv("late_fees_collected", fin.totalLateFeesCollected)
       << JsonHelper::kv("damage_charges_collected", fin.totalDamageChargesCollected)
       << JsonHelper::kv("net_revenue", fin.netTotalRevenue)
       << JsonHelper::kv("total_transactions", fin.totalTransactionsCount, false)
       << "}";
    return ss.str();
}

std::string ReportService::getRevenueReportJson() {
    FinancialSummary fin = m_payRepo.getFinancialSummary();
    auto monthly = m_payRepo.getMonthlyRevenueStats();

    std::ostringstream ss;
    ss << "{\"summary\":" << getDashboardStatsJson() << ",\"monthly\":[";
    for (size_t i = 0; i < monthly.size(); ++i) {
        ss << "{"
           << JsonHelper::kvString("month", monthly[i]["month"])
           << JsonHelper::kv("revenue", std::stod(monthly[i]["revenue"]))
           << JsonHelper::kv("bookings", std::stoi(monthly[i]["bookings"]), false)
           << "}";
        if (i + 1 < monthly.size()) ss << ",";
    }
    ss << "]}";
    return ss.str();
}

std::string ReportService::getCategoryReportJson() {
    auto catCounts = m_rentalRepo.getCategoryCounts();
    std::ostringstream ss;
    ss << "[";
    size_t idx = 0;
    for (const auto& pair : catCounts) {
        ss << "{"
           << JsonHelper::kvString("category", pair.first)
           << JsonHelper::kv("count", pair.second, false)
           << "}";
        if (++idx < catCounts.size()) ss << ",";
    }
    ss << "]";
    return ss.str();
}

std::string ReportService::exportRevenueReportCsv() {
    FinancialSummary fin = m_payRepo.getFinancialSummary();
    auto monthly = m_payRepo.getMonthlyRevenueStats();

    std::ostringstream ss;
    ss << "Metric,Amount (INR)\n";
    ss << "Earned Rental Revenue," << fin.totalEarnedRentalRevenue << "\n";
    ss << "Security Deposits Held," << fin.totalDepositsHeld << "\n";
    ss << "Security Deposits Refunded," << fin.totalDepositsRefunded << "\n";
    ss << "Late Fees Collected," << fin.totalLateFeesCollected << "\n";
    ss << "Damage Charges Collected," << fin.totalDamageChargesCollected << "\n";
    ss << "Net Total Revenue," << fin.netTotalRevenue << "\n\n";

    ss << "Month,Revenue (INR),Booking Count\n";
    for (const auto& m : monthly) {
        ss << m.at("month") << "," << m.at("revenue") << "," << m.at("bookings") << "\n";
    }
    return ss.str();
}

std::string ReportService::exportBookingsReportCsv() {
    auto bookings = m_bookingRepo.findAllBookings();
    std::ostringstream ss;
    ss << "Booking Code,Customer,Item,Start Date,End Date,Days,Daily Rate,Rent,Deposit,Total,Status\n";
    for (const auto& b : bookings) {
        ss << b->getBookingCode() << ",\""
           << b->getCustomerName() << "\",\""
           << b->getItemTitle() << "\","
           << b->getStartDate() << ","
           << b->getEndDate() << ","
           << b->getRentalDays() << ","
           << b->getDailyRate() << ","
           << (b->getBaseRent() - b->getDiscountAmount()) << ","
           << b->getSecurityDeposit() << ","
           << b->getTotalAmount() << ","
           << b->getBookingStatus() << "\n";
    }
    return ss.str();
}

std::string ReportService::exportInventoryReportCsv() {
    auto items = m_rentalRepo.findItems();
    std::ostringstream ss;
    ss << "Item ID,Title,Category,Daily Rate,Deposit,Location,Status\n";
    for (const auto& it : items) {
        ss << it->getId() << ",\""
           << it->getTitle() << "\","
           << it->getCategory() << ","
           << it->getDailyRate() << ","
           << it->getSecurityDeposit() << ",\""
           << it->getLocation() << "\","
           << it->getStatus() << "\n";
    }
    return ss.str();
}
