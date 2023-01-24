# ideaCTRPF

## Usage

### Requirements

```
devkitARM
libctrpf
3gxtool
```

### Build

```bash
git clone https://github.com/kani537/ideaCTRPF.git
cd ./ideaCTRPF
make
```

### Send

#### 1. To install the dependencies, run

```bash
pip install python-dotenv
```

#### 2. Create a `.env` file in the root directory of the project and add settings according to the following

```env
# Plugin file name to send
PLUGIN_NAME=ideaCTRPF.3gx

# Target title id
TITLE_ID=0004000000155100

# 3ds hostname
HOSTNAME=192.168.2.10

# Send type (Optional): 3ds(default), citra, both
SEND_TO=3ds

# Send plugin as default.3gx (Optional): true, false(default)
AS_DEFAULT=false

# sdmc path of citra (Optional)
CITRA_SDMC_PATH=/home/user/.var/app/org.citra_emu.citra/data/citra-emu/sdmc/
```

#### 3. Run the script

```bash
python send.py
```
