import platform
from windows_host import WindowsHost
from linux_host import LinuxHost


def main():
    os_type = platform.system().lower()
    print(f"Detected OS: {platform.system()}\n")

    if os_type == "windows":
        host = WindowsHost()
    elif os_type == "linux":
        host = LinuxHost()
    else:
        raise EnvironmentError(f"Unsupported OS: {platform.system()}")

    host.get_hardware_info()
    host.display_hardware_info()


if __name__ == "__main__":
    main()