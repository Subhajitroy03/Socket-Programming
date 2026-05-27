# VSFTPD (Very Secure FTP Daemon) Beginner Study Guide

A comprehensive, lab-tested reference manual for installing, configuring, managing, and validating an operational FTP Server using `vsftpd` over a local loopback interface.

---

## 1. Installations

To set up an operational environment on Ubuntu, Debian, or WSL2, run the following commands sequentially:

* **`sudo apt update`** *Refreshes package repositories. This is a crucial first step to ensure your package manager fetches the most up-to-date dependencies.*
* **`sudo apt install vsftpd`** *Installs the actual FTP server daemon software that runs in the background.*
* **`sudo apt install net-tools`** *(Optional) Installs legacy utilities like `ifconfig` to quickly view your internal IP architecture.*
* **`sudo apt install inetutils-ftp -y`** *Installs the interactive command-line FTP client tool used to establish outbound connections to servers.*

---

## 2. Basic Server Management Commands

These system utility commands control the lifecycle of the background service daemon via `systemctl`:

* **`sudo systemctl start vsftpd`** Starts the background service daemon thread.
* **`sudo systemctl status vsftpd`** Displays the active state of the service. Look for a bright green `active (running)` string to verify health.
* **`sudo systemctl stop vsftpd`** Shuts down the FTP server engine immediately.
* **`sudo systemctl restart vsftpd`** Stops and immediately spins up the daemon process. **Mandatory** after making any structural modifications to `/etc/vsftpd.conf`.
* **`sudo systemctl enable vsftpd`** Configures the system init layer to start the FTP service automatically whenever the Linux machine boots up.
* **`sudo systemctl disable vsftpd`** Prevents the FTP server from launching automatically on system startup.
* **`vsftpd -v`** Prints out the current version number of the `vsftpd` binary compiled on your machine.

---

## 3. Client Connection Step

To log in and navigate your server space internally, run:

1. **`ftp <IP Address>`** *Initiates a standard network connection. Use `127.0.0.1` (local loopback) for testing purposes.*
2. Provide a valid Linux system username (e.g., `subha`) and password when prompted to complete the authentication loop.

---

## 4. Interactive Client Commands

Once connected and authenticated, your shell prompt transitions to `ftp>`. Run these primary commands to handle file management.

### Crucial Environmental Preparation
Run these two commands immediately upon logging in to prevent data payload corruption and annoying confirmation prompt loops:
* **`binary`** *Description:* Changes the data stream configuration from 7-bit ASCII to 8-bit parsing. **Mandatory** for images, compiled socket binaries, PDFs, or ZIP archives to prevent transport-layer corruption.  
  *Return Output:* `200 Switching to Binary mode.`
* **`prompt`** *Description:* Toggles interactive mode off. Prevents the server from pausing to ask "Y/N" before downloading or uploading individual files during bulk operations.  
  *Return Output:* `Interactive mode off.`

### A) RECEIVING FILES (Downloading: Server &rarr; Local Computer)

* **`pwd`** *Description:* Prints the current working directory path you are standing in on the **remote server**. Preventative tool against blind transfers.  
  *Real-World Example:* ```text
  ftp> pwd
  257 "/home/subha" is the current directory.
  ```
* **`get <filename>`** *Description:* Requests exactly **one** specific file from the server's active path and downloads it to your local system path.  
  *Real-World Example:* ```text
  ftp> get socket_prog.pdf
  local: socket_prog.pdf remote: socket_prog.pdf
  200 PORT command successful. Consider using PASV.
  150 Opening BINARY mode data connection for socket_prog.pdf (124530 bytes).
  226 Transfer complete.
  124530 bytes received in 0.05 secs (2.49 MB/s)
  ```
* **`mget <pattern>`** *Description:* Downloads **multiple** files concurrently using a wildcard pattern (`*`).  
  *Real-World Example:* ```text
  ftp> mget *.pdf
  200 PORT command successful. Consider using PASV.
  150 Opening BINARY mode data connection for day1_notes.pdf.
  226 Transfer complete.
  ```
* **`lcd <path/to/local/dir>`** *Description:* "Local Change Directory". This modifies your positioning on your **local computer** where files land. It leaves your remote directory pointer untouched.  
  *Real-World Example:* ```text
  ftp> lcd /mnt/c/Users/roysu/Downloads/Socket-Programming
  Local directory now /mnt/c/Users/roysu/Downloads/Socket-Programming
  ```

### B) SENDING FILES (Uploading: Local Computer &rarr; Server)

* **`!ls`** *Description:* The exclamation mark (`!`) serves as an escape character. It temporarily breaks out of the FTP environment to run an immediate directory listing directly on your **local machine** shell so you can verify source file locations.  
  *Real-World Example:* ```text
  ftp> !ls
  README.md   checksum   day1   parity   push.sh   test_code.c
  ```
* **`put <filename>`** *Description:* Pinpoints exactly **one** target file on your local disk path and transmits a copy straight up into the server's working space.  
  *Real-World Example:* ```text
  ftp> put test_code.c
  local: test_code.c remote: test_code.c
  200 PORT command successful. Consider using PASV.
  150 Ok to send data.
  226 Transfer complete.
  412 bytes sent in 0.00 secs (121.2 KB/s)
  ```
* **`mput <pattern>`** *Description:* Uploads an entire batch of files matching a specific syntax character string to the server simultaneously.  
  *Real-World Example:* ```text
  ftp> mput *.c
  200 PORT command successful. Consider using PASV.
  150 Ok to send data.
  226 Transfer complete.
  ```

### Closing the Session
* **`bye`** (or `quit` / `exit`)  
  *Description:* Terminates the TCP state transaction cleanly, logs your profile session out, and reverts the prompt cleanly back to your Linux shell.  
  *Real-World Example:* ```text
  ftp> bye
  221 Goodbye.
  subha@Subhajit:/mnt/c/Users/roysu/Downloads/Socket-Programming$
  ```

---

## 5. Troubleshooting & Configuration Overrides

If you hit a **`550 Permission denied`** error, it means file adjustments are being blocked. Open your configuration template using the graphical editor Pluma via:

```bash
sudo pluma /etc/vsftpd.conf
```

Clear out structural inconsistencies and replace the contents with this validated, clean, lab-optimized configuration mapping block:

### Validated `/etc/vsftpd.conf` Blueprint
```text
# Standalone IPv4 Server Settings
listen=YES
listen_ipv6=NO

# Access & Login Controls
anonymous_enable=NO
local_enable=YES

# Read/Write Permissions
write_enable=YES
local_umask=022
dirmessage_enable=YES
use_localtime=YES
xferlog_enable=YES

# Data Connection Settings
connect_from_port_20=YES

# System Directories & Authentication
secure_chroot_dir=/var/run/vsftpd/empty
pam_service_name=vsftpd
rsa_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem
rsa_private_key_file=/etc/ssl/private/ssl-cert-snakeoil.key
ssl_enable=NO
```

---

## 6. Critical Engineering Insights

### Where Lab Assumptions Can Go Wrong
1. **The Active vs. Passive Mode Fallacy:** The server explicitly returns the line `200 PORT command successful. Consider using PASV.` In a loopback test adapter link (`127.0.0.1`), **Active Mode (`PORT`)** runs flawlessly. However, assuming this translates seamlessly to a network exposure layer is incorrect. Real production architectures require setting up **Passive Mode (`PASV`)** port arrays so that data payload sockets don't get rejected by incoming NAT router firewalls.
2. **The Local Path Blindspot:** The `put` command will fail instantly if you initiate the `ftp` tool from a folder that doesn't physically contain your source code files. The client maps its local read buffer to your exact directory location right before you ran the initial connection string.

### Categorization of Information
* **Facts:** FTP status numbers such as `257`, `150`, and `226` are structural execution confirmations mapped out explicitly under strict internet RFC engineering protocols.
* **Guesses/Assumptions:** This study guide assumes your training environment profile runs inside an isolated, unexposed sandbox environment where relaxing security toggles like `chroot` creates zero data liability issues.
* **Opinions:** When debugging stateful protocols like FTP, completely dropping complex user isolation barriers like `chroot_local_user` is the single most practical choice for academic labs. It completely isolates your learning scope to transport layer socket mechanics without losing hours to file system isolation blocks.
