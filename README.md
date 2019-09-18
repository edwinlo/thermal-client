# thermal-client

Simple Android HIDL client that tracks the device temmperature via IThermal service.
This client was intended to be compiled within a AOSP project (https://source.android.com/).

## Instructions
1. Copy the directory of the thermal-client anywhere in the AOSP repo.
2. cd to that directory and rebuild the files (mm)
3. Push the thermal_client bin onto Android device. (adb push to /data/hidl)
4. Run using `adb shell PATH/TO/thermal_client`

## Example

```
# This was tested on the Pixel 2 XL (Taimen) AOSP
adb push out/target/product/taimen/vendor/bin/hw/thermal_client /data/hidl/
adb shell rm /data/hidl/output.csv
adb shell /data/hidl/thermal_client
```