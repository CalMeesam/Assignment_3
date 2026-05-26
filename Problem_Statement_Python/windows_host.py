import subprocess
import re
from host_info import HostInfo


class WindowsHost(HostInfo):
    """Fetches hardware info on Windows using 'systeminfo' and 'wmic'."""

    def get_hardware_info(self):
        self._get_hostname_and_ip()
        self._get_cpu()
        self._get_memory()
        self._get_disk()

    def _get_cpu(self):
        try:
            result = subprocess.check_output(
                ["wmic", "cpu", "get", "Name"],
                text=True
            )
            lines = [l.strip() for l in result.strip().splitlines() if l.strip()]
            # First line is the header "Name", second is the value
            self.cpu = lines[1] if len(lines) > 1 else "Unavailable"
        except Exception as e:
            self.cpu = f"Error: {e}"

    def _get_memory(self):
        try:
            result = subprocess.check_output(
                ["wmic", "OS", "get", "TotalVisibleMemorySize"],
                text=True
            )
            lines = [l.strip() for l in result.strip().splitlines() if l.strip()]
            if len(lines) > 1:
                total_kb = int(lines[1])
                total_gb = round(total_kb / (1024 ** 2), 2)
                self.memory = f"{total_gb} GB"
            else:
                self.memory = "Unavailable"
        except Exception as e:
            self.memory = f"Error: {e}"

    def _get_disk(self):
        try:
            result = subprocess.check_output(
                ["wmic", "diskdrive", "get", "Size"],
                text=True
            )
            lines = [l.strip() for l in result.strip().splitlines() if l.strip()]
            # Sum all disk sizes
            total_bytes = sum(int(l) for l in lines[1:] if l.isdigit())
            total_gb = round(total_bytes / (1024 ** 3), 2)
            self.disk_size = f"{total_gb} GB"
        except Exception as e:
            self.disk_size = f"Error: {e}"