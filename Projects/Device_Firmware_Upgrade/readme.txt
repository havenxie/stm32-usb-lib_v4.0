APP部分可借用stm32-iap-app_lite（https://github.com/havenxie/stm32-iap-app_lite）要注意应该选择相同的芯片型号
/**
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Description of the USB Device_Firmware_Upgrade Demo.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


Example description
===================
This Demo presents the implementation of a device firmware upgrade (DFU) capability
in the STM32F10xxx, STM32F37xxx, STM32F30xxx and STM32L15xxx microcontrollers. It follows the DFU class 
specification defined by the USB Implementers Forum for reprogramming an application 
through USB-FS-Device. 
The DFU principle is particularly well suited to USB-FS-Device applications that 
need to be reprogrammed in the field.

More details about this Demo implementation is given in the User manual 
"UM0424 STM32F10xxx USB development kit", available for download from the ST
microcontrollers website: www.st.com/stm32


Directory contents
==================
 + \inc: contains the Demo firmware header files
 + \EWARM: contains preconfigured project for EWARM toolchain
 + \MDK-ARM: contains preconfigured project for MDK-ARM toolchain           
 + \src: contains the Demo firmware source files


Hardware environment
====================
This example runs these STMicroelectronics evaluation boards and can be easily tailored to any other hardware: 

- STM3210B-EVAL
- STM3210E-EVAL
- STM32L152-EVAL
- STM32F373C_EVAL
- STM32F303C_EVAL
- STM32L152D-EVAL

To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

Note: After each device reset, hold down the Key push-button on the STM3210B-EVAL 
      or STM3210E-EVAL or STM32L152D-EVAL or STM32F373C-EVAL evaluation boards or the Up push button 
      on STM32L152-EVAL evaluation board to enter the DFU mode.

  - STM3210B-EVAL Set-up 
     - Jumper JP1 (USB disconnect) should be connected in position 2-3.

  - STM3210E-EVAL Set-up 
     - Jumper JP14 (USB disconnect) should be connected in position 2-3.

  - STM32L152-EVAL Set-up 
     - None.
     - Note that the JoyStick Up push button is used to enter DFU mode.
 
  - STM32L152D-EVAL Set-up 
     - None.
     
  - STM32F373C-EVAL Set-up 
     - None.

  - STM32F303C-EVAL Set-up 
     - None.
     
How to use it
=============
 + EWARM
    - Open the DFU.eww workspace.
    - In the workspace toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the DFU.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices  
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)  

 

************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE******
