#ifndef PAYMENT_CONTROLLER_H
#define PAYMENT_CONTROLLER_H

#include "ApiControllerBase.h"
#include "../services/PaymentService.h"
#include "../services/BillingService.h"
#include "../services/ReturnService.h"
#include "../services/AuthenticationService.h"

class PaymentController : public ApiControllerBase {
private:
    PaymentService& m_payService;
    BillingService& m_billingService;
    ReturnService& m_returnService;
    AuthenticationService& m_authService;

public:
    PaymentController(PaymentService& pay, BillingService& billing,
                     ReturnService& ret, AuthenticationService& auth);
    void registerRoutes(httplib::Server& svr);
};

#endif // PAYMENT_CONTROLLER_H
