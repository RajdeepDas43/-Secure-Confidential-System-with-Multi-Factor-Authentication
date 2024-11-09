#ifndef SECURE_SYSTEM_H
#define SECURE_SYSTEM_H

#include "MFA.h"
#include <string>

class SecureSystem {
private:
    MFA mfa;

public:
    void login();
};

#endif // SECURE_SYSTEM_H
