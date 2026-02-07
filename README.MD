A Windows-based encrypted socket communication application using **Diffie-Hellman key exchange** and **XOR encryption**. Cyber Security , Universitas Gadjah Mada, November 3, 2024

## Features

- **Server-Client Architecture** - TCP socket communication on port 9090
- **Diffie-Hellman Key Exchange** - Secure shared key generation
- **XOR Encryption** - Message encryption/decryption
- **Windows Winsock2** - Native Windows socket library
- **Real-time Chat** - Bidirectional encrypted messaging

## Requirements

- Windows OS
- C++ compiler (g++, MSVC, or Clang)
- Winsock2 library (included in Windows SDK)

## Build & Run

### Compile

```bash
# Compile server
g++ -o server.exe server.cpp crypto.cpp -lws2_32

# Compile client
g++ -o client.exe client.cpp crypto.cpp -lws2_32
```

### Run

**Terminal 1 - Start Server:**
```bash
.\server.exe
```

**Terminal 2 - Start Client:**
```bash
.\client.exe
```

### Usage

1. Server waits for client connection on port 9090
2. Client connects to `127.0.0.1:9090`
3. Diffie-Hellman key exchange occurs automatically
4. Type messages in client terminal - they get encrypted and sent to server
5. Server receives, decrypts, displays, and sends response back

## Security Notes

⚠️ **Educational Purpose Only**
- XOR encryption is weak for production use
- No authentication or signature verification
- For learning Diffie-Hellman & socket programming


---

**Docs created by Claude Haiku 4.5**