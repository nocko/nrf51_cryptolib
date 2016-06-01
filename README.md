# CMAC-AES128 for nRF51

- Uses AES-ECB hardware peripheral
- Includes target test app that runs the NIST 800-38B Examples
- Includes host test app that checks the NIST 800-38B Examples
  (requires OpenSSL)


## Usage

1. Edit `Makefile`, special note to edit the path to the NRF-SDK
   (tested against SDK 10.0)
   
2. Edit main.c:

>      #define ERROR_LED_PIN 12
>      #define ERROR_LED_ACTIVE LOW

3. `make`: 

	Output files in `_build` directory. Program to nRF51 dev. board to
    run tests on target.

4. `make test`: 

	Fakes AES128 peripheral using openssl, then runs tests on host.
