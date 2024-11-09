# Secure Confidential System

The Secure Confidential System is a C++ project that provides secure data management through features such as multi-factor authentication (MFA), encrypted data storage, secure FTP transfers, and key rotation. This system is designed for managing sensitive information with high security standards.

## Features

- **MFA Login**: Secure login with Time-Based One-Time Password (TOTP) verification.
- **Password-Protected Locker**: Encrypts and securely stores sensitive data in a file-based locker.
- **Snippet Manager**: Stores and retrieves code snippets with encryption.
- **Log Manager**: Logs events securely, accessible only by authorized users.
- **Data Entry Manager**: Stores and retrieves general-purpose encrypted data entries.
- **Secure FTP Client**: Uploads/downloads encrypted files over FTP.
- **Key Management with Key Rotation**: Supports secure key rotation, re-encrypting all sensitive data.

---

## Prerequisites

To compile and run the Secure Confidential System, ensure you have the following:

1. **C++ Compiler**: GCC (MinGW for Windows) or Visual Studio.
2. **OpenSSL**: For encryption/decryption.
3. **Boost**: For networking support with Boost.Asio.
4. **MySQL Connector/C++**: For database support if needed.
5. **vcpkg (optional)**: Recommended for dependency management on Windows.

### Installing Dependencies on Windows with vcpkg

If using **vcpkg** on Windows, install the necessary libraries with:

```powershell
vcpkg install openssl boost mysql-connector-cpp
```

## Build Instructions

### Windows (Using GCC or MinGW)

1. **Navigate to the Project Directory**:
   ```powershell
   cd SecureConfidentialSystem
   ```

2. **Compile the Project**:
   Replace `<path_to_includes>` and `<path_to_libs>` as needed:
   
   ```powershell
   g++ -std=c++11 -o SecureSystem src/*.cpp -Iinclude -lssl -lcrypto -lmysqlcppconn -lboost_system -lws2_32
   ```

3. **Run the Program**:
   ```powershell
   .\SecureSystem
   ```

### Linux or macOS

1. **Navigate to the Project Directory**:
   ```bash
   cd SecureConfidentialSystem
   ```

2. **Compile the Project**:
   ```bash
   g++ -std=c++11 -o SecureSystem src/*.cpp -Iinclude -lssl -lcrypto -lmysqlcppconn -lboost_system -lpthread
   ```

3. **Run the Program**:
   ```bash
   ./SecureSystem
   ```

---

## Usage Guide

The following sections describe how to use each feature in the Secure Confidential System.

### 1. MFA Login

- **Description**: Provides secure login using a Time-Based One-Time Password (TOTP) system.
- **Usage**: When prompted, enter the TOTP code for access. This code is periodically refreshed for security.

### 2. Password-Protected Locker

- **Description**: A secure data locker that encrypts and protects sensitive information.
- **Usage**: The locker prompts for a password to store or retrieve data. Data remains encrypted and accessible only with the correct password.

### 3. Snippet Manager

- **Description**: Encrypts and securely stores code snippets or configuration details.
- **Usage**: Save a snippet by providing a name and content, then retrieve it by specifying the saved name.

### 4. Log Manager

- **Description**: Logs important actions in a secure format.
- **Usage**: Actions (e.g., login, storing data) are logged automatically. Retrieve logs to view past events securely.

### 5. Data Entry Manager

- **Description**: Securely stores general-purpose data entries.
- **Usage**: Save sensitive data with a unique name and retrieve it using that name.

### 6. Secure FTP Client

- **Description**: Uploads and downloads encrypted files over FTP.
- **Usage**: Specify the server details, file path, and direction (upload/download). The system encrypts files before uploading and decrypts them after downloading.

### 7. Key Management (Key Rotation)

- **Description**: Manages encryption keys and supports key rotation.
- **Usage**: Rotate the encryption key as needed to re-encrypt existing data with a new key, enhancing security over time.

---

## Example Execution Flow

1. **Launch the Program**:
   - Run the executable with `.\\SecureSystem` (Windows) or `./SecureSystem` (Linux/macOS).

2. **Authenticate with MFA**:
   - Enter the TOTP code when prompted.

3. **Store and Retrieve Data in the Locker**:
   - Enter a password to encrypt and store sensitive information in the locker.

4. **Use the Snippet Manager**:
   - Save a code snippet and retrieve it securely.

5. **Log Events with Log Manager**:
   - Perform actions like login and snippet storage, and then view the logs.

6. **Manage Data with Data Entry Manager**:
   - Store and retrieve general-purpose data securely.

7. **Use the Secure FTP Client**:
   - Upload or download encrypted files to/from an FTP server.

8. **Rotate Encryption Key**:
   - Rotate the encryption key to re-encrypt all stored data, ensuring ongoing security.

---

## Practical Benefits

1. **Enhanced Security**: Combines MFA, encryption, and secure key management to ensure data security.
2. **Data Confidentiality**: Ensures that sensitive data remains confidential and is only accessible by authorized users.
3. **Compliance**: Suitable for applications requiring secure logging, data encryption, and key rotation to meet compliance standards.
4. **Versatile Use Cases**: Modular design allows each utility to be used independently or as part of a larger system.
5. **Prevent Unauthorized Access**: MFA and password-protected lockers provide strong access control mechanisms.

This system is ideal for securely managing data in applications that require high security, like financial, healthcare, and confidential personal data management.

---

## License

This project is open-source and available under the MIT License.

--- 

This `README.md` provides everything you need to set up, compile, execute, and use each feature of the Secure Confidential System. Let me know in my email sohom432@gmail.com if you have questions about additional configurations or functionalities!

This project is open sourced and welcome to issues and features!
