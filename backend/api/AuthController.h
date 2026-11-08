#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include "ApiControllerBase.h"
#include "../services/AuthenticationService.h"
#include "../database/UserRepository.h"

class AuthController : public ApiControllerBase {
private:
    AuthenticationService& m_authService;
    UserRepository& m_userRepo;

public:
    AuthController(AuthenticationService& auth, UserRepository& userRepo);
    void registerRoutes(httplib::Server& svr);
};

#endif // AUTH_CONTROLLER_H
