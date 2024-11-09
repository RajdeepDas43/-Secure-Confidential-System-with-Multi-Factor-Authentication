#ifndef MFA_H
#define MFA_H

#include <string>

class MFA {
private:
    std::string totpSecretFile;
    std::string totpSecret;
    
    std::string generateRandomSecret(int length = 32);
    void generateTOTPSecret();
    void loadTOTPSecret();

public:
    MFA();
    bool verifyTOTP(const std::string &userInput);
};

#endif // MFA_H
