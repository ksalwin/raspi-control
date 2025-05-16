# raspi-control
The project is designed for remote management of devices connected directly to Raspberry
Pi.

It consists of two parts:
- client application running on PC,
- server application running on Raspberry Pi, with directly connected actuatrs and
sensors.

<div align="center">
  <img src="doc/system_topology.svg" alt="Diagram">
</div>

> [!IMPORTANT]
> This project depends on third-party, external Git repositories, included as Git
submodules in `dependencies/` directory.  
> Before using this project or making any modifications, make sure to review and
understand the licenses of all third-party dependencies located in the `dependencies/`
directory. Each third-party repository may have its own license terms and conditions that
you must comply with.

# Quick Start
- Clone repo: `gh repo clone ksalwin/raspi-control -- --recurse-submodules`
- Build and run client: `sh run.sh`  
`run.sh` will list all possible options.

## SSH Key Setup - connect to Raspberry Pi without typing password every time
To avoid entering your password for every SSH, SCP, or rsync operation:
1. **Generate a key pair** on your PC (if you don't already have one):
```sh
ssh-keygen -t ed25519 -C "<username>@<hostname>"
```
2. **Copy public key to Raspberry Pi**
You will be prompted for your password once.
```sh
ssh-copy-id <raspi_user>@<raspi_ip>
```
After completing these steps, all SSH-based operations will connect without further
password prompts.
