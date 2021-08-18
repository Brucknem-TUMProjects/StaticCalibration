//
// Created by brucknem on 17.08.21.
//
#include <opencv2/opencv.hpp>

#include <StaticCalibration/utils/CommandLineParser.hpp>
#include <StaticCalibration/objects/DataSet.hpp>
#include <StaticCalibration/utils/RenderUtils.hpp>
#include <StaticCalibration/objects/ImageObject.hpp>

int main(int argc, char const *argv[]) {
    auto parsedOptions = static_calibration::utils::parseCommandLine(argc, argv);

    auto dataSet = static_calibration::objects::DataSet(parsedOptions.objectsFile, parsedOptions.pixelsFile,
                                                        parsedOptions.mappingFile);

    cv::Mat evaluationFrame = cv::imread(parsedOptions.evaluationBackgroundFrame);
    evaluationFrame = static_calibration::utils::addAlphaChannel(evaluationFrame);
    cv::Mat finalFrame;

    const char *windowName = "Calibrate By Hand";
    cv::namedWindow(windowName);

    int trackbarShowIds = 1;
    cv::createTrackbar("Show IDs", windowName, &trackbarShowIds, 1);

    std::vector<int> translation;
    std::vector<int> rotation;

    auto intrinsics = parsedOptions.intrinsics;

    for (int i = 0; i < 3; ++i) {
        translation.emplace_back(parsedOptions.translation[i] * 10 + 10000);
        rotation.emplace_back(parsedOptions.rotation[i] * 10 + 1800);
    }
    translation[2] = parsedOptions.translation[2] * 10 + 500;

    cv::createTrackbar("T [X]", windowName, &(translation[0]), 20000);
    cv::createTrackbar("T [Y]", windowName, &(translation[1]), 20000);
    cv::createTrackbar("T [Z]", windowName, &(translation[2]), 1000);

    cv::createTrackbar("R [X]", windowName, &(rotation[0]), 3600);
    cv::createTrackbar("R [Y]", windowName, &(rotation[1]), 3600);
    cv::createTrackbar("R [Z]", windowName, &(rotation[2]), 3600);

    while ((char) cv::waitKey(1) != 'q') {
        finalFrame = evaluationFrame * 0.5;

        Eigen::Vector3d t{(translation[0] - 10000) / 10., (translation[1] - 10000) / 10., (translation[2] - 500) / 10.};
        Eigen::Vector3d r{(rotation[0] - 1800) / 10., (rotation[1] - 1800) / 10., (rotation[2] - 1800) / 10.};

        static_calibration::utils::render(finalFrame, dataSet.getWorldObjects(), t, r, intrinsics, trackbarShowIds);
        static_calibration::utils::render(finalFrame, dataSet.getImageObjects(), trackbarShowIds);
        static_calibration::utils::renderText(finalFrame, t, r, 0);

        cv::imshow(windowName, finalFrame);
    }
}
