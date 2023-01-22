# ideaCTRPF

## Usage

### Build

```bash
make
```

### Send

#### 1. To install the dependencies, run:

```bash
pip install python-dotenv
```

#### 2. Create a `.env` file in the root directory of the project and add settings according to the following
```
# Send type: 3ds, citra, both
SEND_TO=both

# Send plugin as default.3gx
AS_DEFAULT=true

# Plugin file name to send
PLUGIN_NAME=./EasyCTRPF.3gx

# Target title id
TITLE_ID=0004000000155100

# 3ds hostname
HOSTNAME=192.168.2.100

# sdmc path of citra
CITRA_SDMC_PATH=/home/user/.var/app/org.citra_emu.citra/data/citra-emu/sdmc/
```

#### 3. Run the script:

```bash
python send.py
```
