#include <openslam/slam/system.h>
#include <openslam/utils/notify.h>
#include <openslam/utils/timer.h>

namespace openslam
{
	namespace slam
	{
		System::System(const std::string &voc_file_name, const std::string &settings_file_name
			, const Sensor sensor, const bool is_use_viewer) :
			input_sensor_(sensor)
		{
			if (input_sensor_ == SENSOR_MONOCULAR)
				OPENSLAM_INFO << "Monocular" << std::endl;
			else if (input_sensor_ == SENSOR_STEREO)
				OPENSLAM_INFO << "Stereo" << std::endl;
			else if (input_sensor_ == SENSOR_RGBD)
				OPENSLAM_INFO << "RGB-D" << std::endl;
			//����ORB�Ĵʻ��
			utils::Timer timer;
			std::cout << std::endl << "Loading ORB Vocabulary. This could take a while..." << std::endl;

			orb_vocabulary_ = new ORBVocabulary();
			bool is_voc_load = false; // ���Ϊ�˵���ʻ���ʱ��죬��ֱ�ӵ����������
			std::string suffix = ".txt";
			std::size_t index = voc_file_name.find(suffix, voc_file_name.size() - suffix.size());
			if (index != std::string::npos)
				is_voc_load = orb_vocabulary_->loadFromTextFile(voc_file_name);
			else
				is_voc_load = orb_vocabulary_->loadFromBinaryFile(voc_file_name);
			if (!is_voc_load)
			{
				OPENSLAM_FATAL << "Wrong path to vocabulary. " << std::endl;
				OPENSLAM_FATAL << "Falied to open at: " << voc_file_name << std::endl;
				exit(-1);
			}
			OPENSLAM_INFO << "Vocabulary loaded in " << timer.Stop() << "s" << std::endl;
		}
		System::~System()
		{

		}
	}
}