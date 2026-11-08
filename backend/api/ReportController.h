#ifndef REPORT_CONTROLLER_H
#define REPORT_CONTROLLER_H

#include "ApiControllerBase.h"
#include "../services/ReportService.h"
#include "../services/AuthenticationService.h"

class ReportController : public ApiControllerBase {
private:
    ReportService& m_reportService;
    AuthenticationService& m_authService;

public:
    ReportController(ReportService& report, AuthenticationService& auth);
    void registerRoutes(httplib::Server& svr);
};

#endif // REPORT_CONTROLLER_H
