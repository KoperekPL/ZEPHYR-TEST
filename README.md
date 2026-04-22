# Sterowanie kierownicą
Repozytorium jest przeznaczone do oprogramowania sterowania kierownicą za pomocą regulatora PID. Całość trafi na mikrokontroler na płytce RPI PICO. Do komunikacji wartości zadanej kąta wykorzystywany jest UART, a do sprzężenia zwrotnego z położenia kierownicy wejście analogowe.

## Instalacja dependencies, SDK i zephyr OS
Instalacja potrzebnych zależności CMake, python, kompilatory
```
sudo apt update
sudo apt upgrade
sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
```
Środowisko python do instalacji paczek west
```
sudo apt install python3-venv
python3 -m venv ~/zephyr-env
source ~/zephyr-env/bin/activate
```
```
pip install west
```
```
pip install -r zephyr/scripts/requirements.txt
```

Instalacja SDK:
```
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/zephyr-sdk-1.0.1_linux-x86_64.tar.xz
tar xvf zephyr-sdk-1.0.1_linux-x86_64.tar.xz
cd zephyr-sdk-1.0.1
./setup.sh
```

## Init

```bash
west init -m https://github.com/PUT-POWERTRAIN/ZEPHYR-STEROWANIE-KIEROWNICA-PICO --mr main my-workspace
cd my-workspace
west update
```

## Build

```
west build -b <board_name> .
```

W moim przypadku testuję na RPI pico 2W
```
west build -b rpi_pico2/rp2350a/m33/w .
```

## Run

Po zbudowaniu wprowdzaić płytkę w tryb bootsel
```
west flash --runner uf2
```

## Debug
W kodzie można dodawać teraz printk() w celu debugowania kodu i sprawdzenia poprawności działania. Żeby wyświetlić te dane w terminalu po USB:
```
sudo apt install screen
```
```
sudo screen /dev/ttyACM0 115200
```
