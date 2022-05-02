AW9364 [![sample](https://github.com/xingrz/zephyr-aw9364/actions/workflows/sample.yml/badge.svg)](https://github.com/xingrz/zephyr-aw9364/actions/workflows/sample.yml)
==========

Awinic AW9364 LED driver for Zephyr.

## Usage

1. Add this repository as a submodule into your project

    ```sh
    git submodule add https://github.com/xingrz/zephyr-aw9364.git
    ```

2. In your `CMakeLists.txt`:

    ```cmake
    list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_SOURCE_DIR}/zephyr-aw9364/aw9364)
    ```

3. In your `prj.conf`:

    ```conf
    CONFIG_AW9364=y
    ```

## Build sample

```sh
west build -b esp32 sample
```

## License

[Apache-2.0](LICENSE)
