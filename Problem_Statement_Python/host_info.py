from abc import ABC, abstractmethod
import json
import socket


class HostInfo(ABC):
    """Abstract base class for hardware information."""

    def __init__(self):
        self.hostname  = None
        self.memory    = None
        self.cpu       = None
        self.ip        = None
        self.disk_size = None

    def _get_hostname_and_ip(self):
        """Common method to fetch hostname and IP (works on both OS)."""
        self.hostname = socket.gethostname()
        try:
            self.ip = socket.gethostbyname(self.hostname)
        except socket.gaierror:
            self.ip = "Unavailable"

    @abstractmethod
    def get_hardware_info(self):
        """Must be implemented by subclasses to populate hardware attributes."""
        pass

    def display_hardware_info(self):
        """Displays hardware info as formatted JSON."""
        info = {
            "hostname":  self.hostname,
            "ip":        self.ip,
            "cpu":       self.cpu,
            "memory":    self.memory,
            "disk_size": self.disk_size
        }
        print(json.dumps(info, indent=4))