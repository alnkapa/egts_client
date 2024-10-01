# EGTS Client Emulator

## Description
The EGTS client emulator is designed for testing and debugging systems that utilize this protocol. The project allows for the emulation of client behavior, sending data to a server and receiving responses, which is useful for developers and testers.

## Supported Services and Sub-Records

### EGTS_AUTH_SERVICE
- **EGTS_SR_RECORD_RESPONSE**: Handling responses.
- **EGTS_SR_TERM_IDENTITY**: Sending terminal identity information.
- **EGTS_SR_MODULE_DATA**: Transmitting data about terminal modules.

### EGTS_TELEDATA_SERVICE
- **EGTS_SR_RECORD_RESPONSE**: Handling responses.
- **EGTS_SR_POS_DATA**: Sending positioning data (GPS).
- **EGTS_SR_EXT_POS_DATA**: Transmitting extended positioning data.

### EGTS_COMMANDS_SERVICE
- **EGTS_SR_RECORD_RESPONSE**: Handling responses.
- **EGTS_SR_COMMAND_DATA**: Handling commands and responses sent to or received from the terminal."

## Command Line Parameters

The emulator supports the following command line parameters:

- -h, --help                  Display help on usage and available parameters.
- -a [<host>]:<port>          Specify the server address to which the client will connect. For example: -a :5000, -a localhost:5000
- -e <IMEI>                   Specify the IMEI that will be used for authentication. For example: -e 863921034878280
- -n <file_path>              Specify the path to the file containing NMEA data. For example: -n nmea.txt

## Building

To build the project, execute the following commands in the terminal:
For Ubuntu/Debian-based systems:

```bash
sudo apt install -y cmake libgtest-dev
mkdir -p build
cd build
cmake ..
cmake --build .

