croot
adb push out/target/product/taimen/vendor/bin/hw/thermal_client /data/hidl/
adb shell rm /data/hidl/output.csv
adb shell /data/hidl/thermal_client