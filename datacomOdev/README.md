# Socket Programming Assignment – Data Transmission with Error Detection Methods

This project demonstrates error detection methods used in data communication through a practical socket-based implementation.

## Components

1. **Client 1 (Data Sender)**: Takes user input and generates control information using error detection methods
2. **Server (Intermediate Node + Data Corruptor)**: Receives data, injects errors, and forwards to Client 2
3. **Client 2 (Receiver + Error Checker)**: Receives data, validates it, and reports if corruption was detected

## Error Detection Methods

1. **Parity Bit**: Even parity based on number of 1s in binary representation
2. **2D Parity**: Matrix-based parity with row and column checks
3. **CRC-16**: Cyclic Redundancy Check using polynomial 0x8005
4. **Hamming Code**: Error correction code for 4-bit blocks
5. **Internet Checksum**: 16-bit checksum used in IP protocol

## Error Injection Methods

The server randomly applies one of the following error injection methods:

1. **Bit Flip**: Random bit in binary representation is flipped
2. **Character Substitution**: Random character replaced with another
3. **Character Deletion**: Random character removed
4. **Character Insertion**: Random character inserted
5. **Character Swapping**: Two adjacent characters swapped
6. **Multiple Bit Flips**: 2-5 random bits flipped
7. **Burst Error**: 3-8 consecutive characters corrupted

## Building (Linux/WSL)

### Using Makefile

```bash
make
```

### Manual Compilation

```bash
g++ -std=c++17 -o client1 client1_sender.cpp -lpthread
g++ -std=c++17 -o server server.cpp -lpthread
g++ -std=c++17 -o client2 client2_receiver.cpp -lpthread
```

### Clean Build Files

```bash
make clean
```

## Running

**Important**: Run the components in this order (in separate terminals):

1. **First**, start Client 2 (receiver):
   ```bash
   ./client2
   ```

2. **Second**, start the Server:
   ```bash
   ./server
   ```

3. **Third**, start Client 1 (sender):
   ```bash
   ./client1
   ```

4. In Client 1, enter your data and select an error detection method.

## Example Usage

1. Terminal 1 - Start Client 2:
   ```bash
   ./client2
   # Output: Waiting for server on port 8081...
   ```

2. Terminal 2 - Start Server:
   ```bash
   ./server
   # Output: Waiting for Client 1 on port 8080...
   ```

3. Terminal 3 - Start Client 1:
   ```bash
   ./client1
   # Enter data: HELLO
   # Select method: 3 (CRC-16)
   # Client 1 sends: HELLO|CRC16|87AF
   ```

4. Server receives, corrupts data (e.g., `HEZLO`), and sends to Client 2

5. Client 2 receives and validates:
   ```
   Received Data: HEZLO
   Method: CRC16
   Sent Check Bits: 87AF
   Computed Check Bits: 92B1
   Status: DATA CORRUPTED
   ```

## Packet Format

The packet format is: `DATA|METHOD|CONTROL_INFORMATION`

Example: `HELLO|CRC16|87AF`

## Requirements

- C++17 or higher
- Linux/WSL Ubuntu (or any Linux distribution)
- g++ compiler
- Standard socket libraries (included in Linux)

## Notes

- The server uses random error injection, so results may vary each run
- For 2D Parity, the matrix size is configurable (default: 8 rows)
- CRC-16 uses polynomial 0x8005
- Hamming Code processes data in 4-bit blocks
- Make sure to run components in separate terminal windows/tabs
