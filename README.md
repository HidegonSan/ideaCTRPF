# ideaCTRPF

## Usage

### Build

```bash
$ make
```

### Send

#### 1. To install the dependencies, run:

```bash
$ pip install python-dotenv
```

#### 2. Create a `.env` file in the root directory of the project and add the following:

```
HOSTNAME=your_hostname
TITLE_ID=your_title_id
```

For example:

```
HOSTNAME=192.168.1.10
TITLE_ID=0004000000155100
```

#### 3. Run the script:

```bash
$ python send.py
```
