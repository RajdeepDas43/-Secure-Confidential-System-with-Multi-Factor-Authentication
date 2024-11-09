class SecureSystem {
private:
    MFA mfa;

public:
    void login() {
        std::string password;
        std::string totpCode;

        std::cout << "Enter password: ";
        std::cin >> password;

        if (password != "securepassword") {
            std::cerr << "Invalid password.\n";
            return;
        }

        std::cout << "Enter TOTP code: ";
        std::cin >> totpCode;

        if (!mfa.verifyTOTP(totpCode)) {
            std::cerr << "Invalid TOTP code.\n";
            return;
        }

        std::cout << "Login successful.\n";
    }
};

int main() {
    SecureSystem system;
    system.login();
    return 0;
}
