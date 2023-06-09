# Operation Systems (OSs) Course Assignment 5 (Bonus)

### For Computer Science B.S.c Ariel University

**By Roy Simanovich and Linor Ronen**

## Description


## Requirements
* Linux machine (Ubuntu 22.04 LTS preferable)
* GNU C Compiler
* OpenSSL crypto library
* Bzip2 Compression library
* Make

## Building
```
# Cloning the repo to local machine.
git clone https://github.com/RoySenpai/opsys_hw5_bonus.git

# Install OpenSSL crypto library
sudo apt install libssl-dev

# Bzip2 Compression library
sudo apt install libzip-dev

# Building all the necessary files & the main programs.
make all
```

## Running
```
# Run the mail server
./mail_server

# Run the mail client
./mail_client
```