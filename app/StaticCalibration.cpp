//
// Created by brucknem on 02.02.21.
//

#include <thread>
#include "StaticCalibration/objects/ObjectsLoading.hpp"
#include "StaticCalibration/CameraPoseEstimation.hpp"

#include "glog/logging.h"
#include <boost/filesystem.hpp>

#include "StaticCalibration/utils/CommandLineParser.hpp"
#include "StaticCalibration/utils/Formatters.hpp"

int main(int argc, char const *argv[]) {
    srandom(time(nullptr));
    auto parsedOptions = static_calibration::utils::parseCommandLine(argc, argv);

//    auto origin = static_calibration::calibration::getOrigin(parsedOptions.objectsFile, "4007847");
    auto objects = static_calibration::calibration::loadObjects(parsedOptions.objectsFile, parsedOptions.pixelsFile);

    static_calibration::calibration::CameraPoseEstimation estimator;
    estimator.addWorldObjects(objects);
    estimator.guessIntrinsics(parsedOptions.focalLength, parsedOptions.focalLengthRatio, parsedOptions.principalPoint,
                              parsedOptions.skew);
//    estimator.fixIntrinsics(true);

    google::InitGoogleLogging("Static Calibration");
    estimator.estimate(parsedOptions.logEstimationProgress);

    if (!estimator.hasFoundValidSolution()) {
        std::cout << "Couldn't find valid solution! "
                     "This may happen due to initializations. "
                     "If it persists after rerunning please check if the input data is correct." << std::endl;
        exit(EXIT_FAILURE);
    }

    auto outFilename = boost::filesystem::path(
            parsedOptions.measurementPointName + "_" + parsedOptions.cameraName + ".launch");
    std::ofstream outFile;
    outFile.open(outFilename.string());

//    std::cout << estimator.getTranslation() << std::endl;
//    estimator.guessTranslation(estimator.getTranslation() - origin);
//    std::cout << estimator.getTranslation() << std::endl;
    auto rosXML = static_calibration::utils::toROSXML(estimator, parsedOptions.measurementPointName,
                                                      parsedOptions.cameraName);
    std::cout << rosXML << std::endl;
    outFile << rosXML;
    outFile.close();

    return EXIT_SUCCESS;
}
