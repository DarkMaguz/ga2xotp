# Google Authenticator to xOTP
Read data from Google Authenticator QR codes

Google Authenticator has a somewhat overly complex way of exporting otherwise simple data.<br>
So I wrote some code to wrap my head around it and now I'm just left puzzled.

## Usage
Don't use...

### Requirements
For Debian Trixie:
```bash
apt install protobuf-compiler liburiparser-dev libzbar-dev libmagick++-dev libssl-dev libcrypto++-dev nlohmann-json3-dev
```

### Build
```bash 
meson setup --reconfigure builddir .
meson compile -C builddir/
```

### Run
```bash 
./builddir/ga2xotp path/to/image.png
```

Output sample:
```json
[
 {
    "algorithm": "SHA1",
    "counter": 0,
    "digits": 1,
    "issuer": "Microsoft",
    "label": "something@curious.onmicrosoft.com",
    "secret": "XXXXXXXXXXXXXXXX",
    "type": "TOTP"
  },
  {
    ...
  }
]
```

## Thanks
Inspiration from [Chris van Marle](https://github.com/qistoph) who wrote [OtpMigration.proto](src/OtpMigration.proto) and is wise(unlike me) to write in Python when simplicity is called for.

### Libraries used:
* [protobuf](https://github.com/google/protobuf/)
* [uriparser](https://uriparser.github.io)
* [zbar](https://github.com/mchehab/zbar)
* [libmagick++](https://www.imagemagick.org/)
* [libssl](https://openssl-library.org)
* [libcrypto++](https://www.cryptopp.com)
* [nlohmann_json](https://nlohmann.github.io/json/)
