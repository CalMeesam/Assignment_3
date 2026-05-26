#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>

using namespace std;

/**
 * Represents a single host/machine in the inventory.
 * Stores all hardware and OS details for one IP address.
 */
struct Host {
    string ip;
    string os;
    string memory;   // e.g. "4GB"
    string cpu;      // e.g. "3.8Ghz"
    string disk;     // e.g. "150GB"

    // Display host info in a readable format
    void display() const {
        cout << "  IP      : " << ip     << endl;
        cout << "  OS      : " << os     << endl;
        cout << "  Memory  : " << memory << endl;
        cout << "  CPU     : " << cpu    << endl;
        cout << "  Disk    : " << disk   << endl;
        cout << "  --------" << endl;
    }
};

/**
 * Strips quotes and whitespace from a JSON string value.
 * Example: "  \"Windows\"  " becomes "Windows"
 */
string cleanValue(const string& val) {
    string result = val;
    size_t start = result.find_first_not_of(" \t\r\n\"");
    size_t end   = result.find_last_not_of(" \t\r\n\",");
    if (start == string::npos) return "";
    return result.substr(start, end - start + 1);
}

/**
 * Extracts numeric part from strings like "4GB", "3.8Ghz", "150GB".
 * Used to compare values numerically.
 * "4GB" becomes 4.0,  "3.8Ghz" becomes 3.8
 */
double extractNumber(const string& val) {
    string numStr = "";
    for (char c : val) {
        if (isdigit(c) || c == '.') {
            numStr += c;
        }
    }
    if (numStr.empty()) return 0.0;
    return stod(numStr);
}

/**
 * Reads inventory.json line by line and parses all hosts.
 * Returns a vector of Host objects.
 */
vector<Host> loadInventory(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("ERROR: Cannot open file -> " + filePath);
    }

    vector<Host> hosts;
    Host current;
    bool insideHost = false;

    string line;
    while (getline(file, line)) {
        if (line.find("\"ip\"") != string::npos) {
            size_t colon = line.find(":");
            current.ip = cleanValue(line.substr(colon + 1));
            insideHost = true;
        }
        else if (insideHost && line.find("\"os\"") != string::npos) {
            size_t colon = line.find(":");
            current.os = cleanValue(line.substr(colon + 1));
        }
        else if (insideHost && line.find("\"memory\"") != string::npos) {
            size_t colon = line.find(":");
            current.memory = cleanValue(line.substr(colon + 1));
        }
        else if (insideHost && line.find("\"cpu\"") != string::npos) {
            size_t colon = line.find(":");
            current.cpu = cleanValue(line.substr(colon + 1));
        }
        else if (insideHost && line.find("\"disk\"") != string::npos) {
            size_t colon = line.find(":");
            current.disk = cleanValue(line.substr(colon + 1));

            // disk is last field — save host and reset
            hosts.push_back(current);
            current = Host();
            insideHost = false;
        }
    }

    if (hosts.empty()) {
        throw runtime_error("ERROR: No hosts found in file.");
    }

    return hosts;
}

/**
 * InventoryFilter class handles all filtering logic.
 * Supports: Memory, CPU, Linux, Windows
 */
class InventoryFilter {
private:
    vector<Host> hosts;

public:
    // Constructor loads inventory from JSON file
    InventoryFilter(const string& filePath) {
        hosts = loadInventory(filePath);
        cout << "Loaded " << hosts.size() << " host(s) from inventory.\n\n";
    }

    /**
     * Apply filter based on user criteria.
     * Valid values: Memory, CPU, Linux, Windows (case-insensitive)
     */
    void filter(const string& criteria) {
        // Convert to lowercase for case-insensitive comparison
        string c = criteria;
        transform(c.begin(), c.end(), c.begin(), ::tolower);

        if (c == "memory") {
            filterByMaxMemory();
        } else if (c == "cpu") {
            filterByMaxCPU();
        } else if (c == "linux") {
            filterByOS("Linux");
        } else if (c == "windows") {
            filterByOS("Windows");
        } else {
            // Invalid criteria — raise exception as per assignment
            throw invalid_argument(
                "ERROR: Invalid filter criteria -> '" + criteria + "'\n"
                "Valid options are: Memory, CPU, Linux, Windows"
            );
        }
    }

private:
    /**
     * Find and display host with highest RAM.
     * Compares numeric part of memory field (16 from "16GB")
     */
    void filterByMaxMemory() {
        Host* best = nullptr;
        double maxVal = -1;
        for (Host& h : hosts) {
            double val = extractNumber(h.memory);
            if (val > maxVal) {
                maxVal = val;
                best = &h;
            }
        }
        cout << "=== Host with MAX Memory (" << best->memory << ") ===" << endl;
        best->display();
    }

    /**
     * Find and display host with highest CPU speed.
     * Compares numeric part of cpu field (3.8 from "3.8Ghz")
     */
    void filterByMaxCPU() {
        Host* best = nullptr;
        double maxVal = -1;
        for (Host& h : hosts) {
            double val = extractNumber(h.cpu);
            if (val > maxVal) {
                maxVal = val;
                best = &h;
            }
        }
        cout << "=== Host with MAX CPU (" << best->cpu << ") ===" << endl;
        best->display();
    }

    /**
     * Display all hosts matching given OS type.
     */
    void filterByOS(const string& osType) {
        cout << "=== Hosts with OS = " << osType << " ===" << endl;
        bool found = false;
        for (const Host& h : hosts) {
            if (h.os == osType) {
                h.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No hosts found with OS: " << osType << endl;
        }
    }
};

/**
 * Entry point.
 * Usage: ./inventory <filter_criteria>
 * Example: ./inventory Memory
 *          ./inventory Linux
 */
int main(int argc, char* argv[]) {
    try {
        // Filter criteria is mandatory
        if (argc < 2) {
            throw invalid_argument(
                "ERROR: Filter criteria is required!\n"
                "Usage: inventory <Memory|CPU|Linux|Windows>"
            );
        }

        string criteria = argv[1];

        // Create filter object — loads JSON on construction
        InventoryFilter inv("inventory.json");

        // Apply the filter
        inv.filter(criteria);

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}