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
