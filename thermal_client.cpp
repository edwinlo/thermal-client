#define LOG_TAG "THERMAL_CLIENT"
#include <android/hardware/thermal/1.0/IThermal.h>
#include <android/hardware/thermal/1.0/types.h>
#include <log/log.h>

#include <ctime>
#include <fstream>

#define _THERMAL_CLIENT_GLOBAL_TIMER_STOP 1810     //30 min
#define _THERMAL_CLIENT_TRIAL_TIMER_STOP 60        //1 min

using android::hardware::thermal::V1_0::CoolingDevice;
using android::hardware::thermal::V1_0::CpuUsage;
using android::hardware::thermal::V1_0::IThermal;
using android::hardware::thermal::V1_0::Temperature;    
using android::hardware::thermal::V1_0::ThermalStatus;
using android::hardware::thermal::V1_0::ThermalStatusCode;

using android::hardware::hidl_vec;

using android::sp;

static std::ofstream outputFile;

int main() {

    sp<IThermal> service = IThermal::getService();

    if (service == nullptr) {
        ALOGE("Unable to get IThermal service..");
        return -1;
    }

    // Initialize Timer
    int globalStart = std::clock();
    int trialStart = std::clock();
    int currentMin = 0;
    double trialDiff, globalDiff;

    outputFile.open("/data/hidl/output.csv");

    // Generate headers (from hidl_vec<Temp>)
    outputFile << "Time, CPU0, CPU1, CPU2, CPU3, CPU4, CPU5, CPU6, CPU7, GPU, Battery, Skin\n";
    
    do {
        trialDiff = (std::clock() - trialStart) / (double) (CLOCKS_PER_SEC);
        
        if (trialDiff > _THERMAL_CLIENT_TRIAL_TIMER_STOP){
            
            // Get temperatures (assume no errors)
            service->getTemperatures([trialStart](const ThermalStatus &status, const hidl_vec<Temperature> &temperatures){
                if (status.code != ThermalStatusCode::SUCCESS) {
                    ALOGE("Something went wrong. Status code was not SUCCESS");
                    ALOGE("Debug message: %s", status.debugMessage.c_str());
                }
                std::string trailingChar = ",";
                outputFile << std::to_string((trialStart) / (double) (CLOCKS_PER_SEC));
                for (auto t : temperatures){
                    outputFile << trailingChar << t.currentValue;
                }
                outputFile << "\n";
                outputFile.flush();
            });

            trialStart = std::clock();
            currentMin++;
            globalDiff = (std::clock() - globalStart) / (double) (CLOCKS_PER_SEC);
            ALOGW("Current Time: %d", currentMin);
        }

    } while (globalDiff < _THERMAL_CLIENT_GLOBAL_TIMER_STOP);

    outputFile.close();

    return 0;
}