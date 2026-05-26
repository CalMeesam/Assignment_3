import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

public class LogFileParser {

    private static final Set<String> VALID_LOG_TYPES =
        Set.of("error", "warning", "info", "debug");

    public static List<String> parseLogs(String filePath,
                                          int numLines,
                                          String logTypes) throws IOException {

        // Validate file path
        Path path = Paths.get(filePath);
        if (!Files.exists(path) || !Files.isRegularFile(path)) {
            throw new IllegalArgumentException("Invalid file path: " + filePath);
        }

        // Validate and parse log types
        Set<String> requestedTypes = Arrays.stream(logTypes.split(","))
            .map(String::trim)
            .map(String::toLowerCase)
            .collect(Collectors.toCollection(LinkedHashSet::new));

        for (String type : requestedTypes) {
            if (!VALID_LOG_TYPES.contains(type)) {
                throw new IllegalArgumentException(
                    "Invalid log type: '" + type + "'. Valid types are: " + VALID_LOG_TYPES
                );
            }
        }

        // Read all lines
        List<String> allLines = Files.readAllLines(path);

        // Group into log entries
        List<String> logEntries = groupIntoLogEntries(allLines);

        // Filter from end (most recent first)
        LinkedList<String> result = new LinkedList<>();
        for (int i = logEntries.size() - 1; i >= 0 && result.size() < numLines; i--) {
            String entry = logEntries.get(i);
            String detectedType = detectLogType(entry);
            if (detectedType != null && requestedTypes.contains(detectedType)) {
                result.addFirst(entry);
            }
        }

        return result;
    }

    /**
     * Detects log type from a line, supporting multiple formats:
     *   [INFO], [ERROR], [DEBUG], [WARNING]  -> bracket format
     *   INFO:, ERROR:, DEBUG:, WARNING:      -> colon format
     *   info, error, debug, warning          -> plain format
     */
    private static String detectLogType(String line) {
        if (line == null || line.isBlank()) return null;
        String trimmed = line.trim().toLowerCase();

        for (String type : VALID_LOG_TYPES) {
            if (trimmed.startsWith("[" + type + "]") ||  // [INFO], [ERROR]
                trimmed.startsWith(type + ":") ||         // INFO:, ERROR:
                trimmed.startsWith(type + " ")) {         // info message...
                return type;
            }
        }
        return null;
    }

    /**
     * Groups raw lines into complete log entries.
     * A new entry starts when a line begins with a known log keyword (any format).
     */
    private static List<String> groupIntoLogEntries(List<String> lines) {
        List<String> entries = new ArrayList<>();
        StringBuilder current = new StringBuilder();

        for (String line : lines) {
            if (detectLogType(line) != null) {
                if (current.length() > 0) {
                    entries.add(current.toString().trim());
                }
                current = new StringBuilder(line);
            } else {
                if (current.length() > 0) {
                    current.append("\n").append(line);
                }
            }
        }

        if (current.length() > 0) {
            entries.add(current.toString().trim());
        }

        return entries;
    }

    public static void main(String[] args) {
        String filePath  = "application.log";
        int numLines     = 10;
        String logTypes  = "error";

        if (args.length >= 1) filePath = args[0];
        if (args.length >= 2) numLines = Integer.parseInt(args[1]);
        if (args.length >= 3) logTypes = args[2];

        try {
            List<String> results = parseLogs(filePath, numLines, logTypes);

            System.out.println("=== Matched " + results.size() + " log entry/entries ===\n");
            results.forEach(entry -> System.out.println(entry + "\n" + "-".repeat(60)));

        } catch (IllegalArgumentException e) {
            System.err.println("[VALIDATION ERROR] " + e.getMessage());
        } catch (IOException e) {
            System.err.println("[IO ERROR] " + e.getMessage());
        }
    }
}