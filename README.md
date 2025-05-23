# <a href="https://tea-lab.github.io/DOGlove/chapters/the_firmware.html">DOGlove Embedded Firmware</a>
Welcome! This is the official embedded firmware for [DOGlove](https://do-glove.github.io/), a low-cost, open-source haptic+force feedback glove for dexterous manipulation and teleoperation.

[🌐 Project](https://do-glove.github.io/) | 
[📄 Paper](https://arxiv.org/pdf/2502.07730) | 
[🎥 Video](https://www.youtube.com/watch?v=2qO7dbH9zxc) | 
[📚 Documentation](https://tea-lab.github.io/DOGlove/) |
[▶️ 30min工作讲解](https://www.bilibili.com/video/BV19SLizUEfa/)

## Notes for Advanced Developers
To modify the firmware or view pin configurations, open [DOGlove.ioc](./DOGlove.ioc) using [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html).

### Source Code Structure
* [/Core](./Core/)：# Auto-generated by STM32CubeMX, except `main.c` and `stm32f0xx_it.c`
    * [main.c](./Core/Src/main.c)：System initialization, LRA control, and encoder data packaging.
    * [stm32f0xx_it.c](./Core/Src/stm32f0xx_it.c)：SysTick handler used for system-level scheduling.

* [/Drivers](./Drivers/)：STM32 HAL drivers (auto-generated)

* [/Users](./Users/): Custom drivers and application code.
    * [ads1256.c](./Users/ads1256.c): ADC driver for the ADS1256 chip.
    * [drv2605l.c](./Users/drv2605l.c): Haptic driver for the DRV2605L chip.
    * [tca9548a.c](./Users/tca9548a.c): I2C multiplexer driver for the TCA9548A chip.
    * [fsr.c](./Users/fsr.c): Read power supply voltage through the STM32’s internal ADC channel.
    * [delay.c](./Users/delay.c): Precise millisecond delay implementation.
    * [lra_control.c](./Users/lra_control.c): LRA control logic and UART RX event callback.

## License
This repository is released under the MIT license. See [LICENSE](LICENSE) for more details.

## Citation
If you find our work useful, please consider citing:
```console
@article{zhang2025doglove,
  title={DOGlove: Dexterous Manipulation with a Low-Cost Open-Source Haptic Force Feedback Glove},
  author={Zhang, Han and Hu, Songbo and Yuan, Zhecheng and Xu, Huazhe},
  journal={arXiv preprint arXiv:2502.07730},
  year={2025}
}
```