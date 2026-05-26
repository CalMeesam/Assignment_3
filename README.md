# Assignment 3 - Shriya Sharma

This repository contains three coding assignments implemented in Python, C++, and Java, demonstrating object-oriented programming, file handling, and exception management.

---

## Table of Contents
- [Q1 - Get Hardware Info (Python)](#q1---get-hardware-info-python)
- [Q2 - Filter Inventory Data (C++)](#q2---filter-inventory-data-c)
- [Q3 - Log File Parsing (Java)](#q3---log-file-parsing-java)
- [Technologies Used](#technologies-used)

---

## Q1 - Get Hardware Info (Python)

### Problem Statement
A Python program that displays real-time hardware information of the system using object-oriented programming principles.

### Approach
- A parent abstract class `HostInfo` is created with common attributes: `hostname`, `memory`, `cpu`, `ip`, and `disk_size`
- An abstract method `get_hardware_info()` is defined in the parent class and must be implemented by every child class
- Two child classes `LinuxHost` and `WindowsHost` inherit from `HostInfo` and implement `get_hardware_info()` using OS-specific commands
- `LinuxHost` uses commands like `free`, `lscpu`, and `df` to fetch hardware details
- `WindowsHost` uses `systeminfo` and `wmic` commands to fetch hardware details
- A common method `display_hardware_info()` in the parent class displays all collected info in JSON format
- The main script detects the current OS using `platform.system()` and instantiates the appropriate child class automatically

### Project Structure
```
q1_hardware_info/
├── host_info.py       # Abstract parent class
├── linux_host.py      # Child class for Linux
├── windows_host.py    # Child class for Windows
└── main.py            # Entry point
```

### How to Run
```bash
cd q1_hardware_info
python main.py
```

### Sample Output
```json
{
    "hostname": "DESKTOP-ABC123",
    "ip": "192.168.1.10",
    "memory": "8 GB",
    "cpu": "Intel Core i5",
    "disk_size": "512 GB"
}
```

---

## Q2 - Filter Inventory Data (C++)

### Problem Statement
A C++ program that reads inventory data from a JSON file and filters it based on given criteria using object-oriented design.

### Approach
- A `Host` struct stores all details of a single machine: `ip`, `os`, `memory`, `cpu`, and `disk`
- The JSON file is parsed line by line without any external library, extracting values for each known key
- Numeric values like `"4GB"` and `"3.8Ghz"` are handled by stripping unit characters and comparing only the numeric part
- An `InventoryFilter` class handles all filtering logic with separate private methods for each filter type
- The program supports four filter criteria: `Memory`, `CPU`, `Linux`, and `Windows`
- If filter criteria is `Memory` or `CPU`, the host with the maximum value is displayed
- If filter criteria is `Linux` or `Windows`, all hosts matching that OS are displayed
- Exceptions are raised if filter criteria is missing or invalid

### Project Structure
```
q2_inventory_filter/
├── main.cpp           # Main program with all classes
└── inventory.json     # Input inventory data file
```

### How to Run
```bash
cd q2_inventory_filter
g++ -o inventory main.cpp -std=c++17

.\inventory.exe Memory
.\inventory.exe CPU
.\inventory.exe Linux
.\inventory.exe Windows
```

### Filter Criteria

| Criteria | Description                          |
|----------|--------------------------------------|
| Memory   | Displays host with maximum RAM       |
| CPU      | Displays host with maximum CPU speed |
| Linux    | Displays all hosts running Linux     |
| Windows  | Displays all hosts running Windows   |

---

## Q3 - Log File Parsing (Java)

### Problem Statement
A Java program that parses a log file and returns the most recent log entries based on log type and count.

### Approach
- The program accepts three inputs: file path, number of lines to display, and log types to filter
- Number of lines defaults to `10` and log type defaults to `"error"` if not provided
- The log file is read and scanned from bottom to top so that the most recent logs are picked first
- Each log line starts with a type in square brackets like `[INFO]`, `[DEBUG]`, `[ERROR]`, `[WARNING]`
- The program extracts the type from inside the brackets and matches it against the requested types
- Multiple log types can be provided as comma-separated values like `"info,debug"`
- Exceptions are raised if the file path is invalid or if an invalid log type is provided
- Results are reversed before display so output appears in chronological order

### Project Structure
```
q3_log_parser/
├── LogParser.java                  # Main Java program
└── Log_19_10_17_11_42_01.log      # Sample log file
```

### How to Run
```bash
cd Assignment3-Shriya
javac q3_log_parser/LogParser.java
java q3_log_parser.LogParser q3_log_parser/Log_19_10_17_11_42_01.log 10 info
```

### Input Parameters

| Parameter  | Required | Default | Description                          |
|------------|----------|---------|--------------------------------------|
| File Path  | Yes      | None    | Path to the log file                 |
| Num Lines  | No       | 10      | Number of recent logs to display     |
| Log Types  | No       | error   | Comma-separated log types to filter  |

### Valid Log Types
`error` &nbsp; `warning` &nbsp; `info` &nbsp; `debug`

---

## Technologies Used

| Task | Language | Key Concepts                                        |
|------|----------|-----------------------------------------------------|
| Q1   | Python   | OOP, Abstract Classes, Inheritance, OS Commands     |
| Q2   | C++      | OOP, Structs, File Parsing, Exception Handling      |
| Q3   | Java     | File I/O, Collections, Exception Handling           |
