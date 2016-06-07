# Cryptography Library for nRF51

- Uses AES-ECB hardware peripheral
- Includes target test app that runs the EAX test vectors
- Includes host test app that verifies the test vectors from NIST
  800-38B, EAX papes, NIST 800-38A F5.1 (requires OpenSSL)

## Usage

1. Edit `Makefile`, special note to edit the path to the NRF-SDK
   (tested against SDK 10.0)
   
2. Edit main.c:

>      #define SUCCESS_LED 10
>      #define ERROR_LED 12
>      #define LED_ACTIVE LOW

3. `make`: 

	Output files in `_build` directory. Program to nRF51 dev. board to
    run tests on target.

4. `make test`: 

	Fakes ECB peripheral using openssl, then runs tests on host.
