#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include "ApiControllerBase.h"
#include "../database/UserRepository.h"
#include "../services/ReportService.h"
#include "../services/AuthenticationService.h"

class AdminController : public ApiControllerBase {
private:
    UserRepository& m_userRepo;
    ReportService& m_reportService;
    AuthenticationService& m_authService;

public:
    AdminController(UserRepository& userRepo, ReportService& report, AuthenticationService& auth);
    void registerRoutes(httplib::Server& svr);
};

#endif // ADMIN_CONTROLLER_H
