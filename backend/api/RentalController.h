#ifndef RENTAL_CONTROLLER_H
#define RENTAL_CONTROLLER_H

#include "ApiControllerBase.h"
#include "../services/RentalService.h"
#include "../services/AuthenticationService.h"

class RentalController : public ApiControllerBase {
private:
    RentalService& m_rentalService;
    AuthenticationService& m_authService;

public:
    RentalController(RentalService& rental, AuthenticationService& auth);
    void registerRoutes(httplib::Server& svr);
};

#endif // RENTAL_CONTROLLER_H
