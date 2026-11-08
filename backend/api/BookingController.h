#ifndef BOOKING_CONTROLLER_H
#define BOOKING_CONTROLLER_H

#include "ApiControllerBase.h"
#include "../services/BookingService.h"
#include "../services/AuthenticationService.h"

class BookingController : public ApiControllerBase {
private:
    BookingService& m_bookingService;
    AuthenticationService& m_authService;

public:
    BookingController(BookingService& booking, AuthenticationService& auth);
    void registerRoutes(httplib::Server& svr);
};

#endif // BOOKING_CONTROLLER_H
