//
// Created by brucknem on 04.03.21.
//

#ifndef CAMERASTABILIZATION_OBJECTSLOADING_HPP
#define CAMERASTABILIZATION_OBJECTSLOADING_HPP

#include <string>
#include <utility>
#include "yaml-cpp/yaml.h"

#include "WorldObject.hpp"

namespace static_calibration {
	namespace calibration {

		/**
		 * Loads the given YAML file.
		 *
		 * @param filename
		 * @return
		 */
		YAML::Node loadYAML(const std::string &filename);

		/**
		 * Loads the world positions of the objects from a YAML object.
		 *
		 * @param opendriveObjects
		 * @return
		 */
		std::vector<WorldObject>
		loadObjects(YAML::Node opendriveObjects, const YAML::Node &imageObjects, const Eigen::Vector2i &
		imageSize = {-1, -1});

		/**
		 * Loads the world positions of the objects from a YAML file.
		 *
		 * @param opendriveObjectsFile
		 * @return
		 */
		std::vector<WorldObject> loadObjects(const std::string &opendriveObjectsFile, const std::string
		&imageObjectsFile, const Eigen::Vector2i &imageSize = {-1, -1});
	}
}

#endif //CAMERASTABILIZATION_OBJECTSLOADING_HPP
