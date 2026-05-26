import subprocess
import re
from host_info import HostInfo


class LinuxHost(HostInfo):
    """Fetches hardware info on Linux using shell commands."""

    def get_hardware_info(self):
        self._get_hostname_and_ip()
        self._get_cpu()
        self._get_memory()
        self._get_disk()

    def _get_cpu(self):
        try:
            result = subprocess.check_output(
                ["grep", "-m", "1", "model name", "/proc/cpuinfo"],
                text=True
            )
            self.cpu = result.split(":")[1].strip()
        except Exception as e:
            self.cpu = f"Error: {e}"

    def _get_memory(self):
        try:
            result = subprocess.check_output(
                ["grep", "MemTotal", "/proc/meminfo"],
                text=True
            )
            mem_kb = int(re.search(r"\d+", result).group())
            mem_gb = round(mem_kb / (1024 ** 2), 2)
            self.memory = f"{mem_gb} GB"
        except Exception as e:
            self.memory = f"Error: {e}"

    def _get_disk(self):
        try:
            result = subprocess.check_output(
                ["df", "-BG", "--total"],
                text=True
            )
            # Last line is the total row
            total_line = result.strip().splitlines()[-1]
            total_gb = total_line.split()[1].replace("G", "")
            self.disk_size = f"{total_gb} GB"
        except Exception as e:
            self.disk_size = f"Error: {e}"