#include <openslam/slam/tracking_mono.h>
#include <openslam/utils/notify.h>

namespace openslam
{
	namespace slam
	{
		TrackingMono::TrackingMono(const std::string &setting_path_name, ORBVocabulary *voc):
			TrackingBase(voc)
		{
			// �������ļ��е�������ڲ�
			cv::FileStorage file_settings(setting_path_name, cv::FileStorage::READ);
			// ��ʲô��С��ͼƬ�±궨�����Ҫ֪����
			float width = file_settings["Camera.width"];
			float height = file_settings["Camera.height"];
			float fx = file_settings["Camera.fx"];
			float fy = file_settings["Camera.fy"];
			float cx = file_settings["Camera.cx"];
			float cy = file_settings["Camera.cy"];
			
			// �õ�����������
			float k1 = file_settings["Camera.k1"];
			float k2 = file_settings["Camera.k2"];
			float p1 = file_settings["Camera.p1"];
			float p2 = file_settings["Camera.p2"];
			float k3 = file_settings["Camera.k3"];
			cam_ = new MonocularCamera(width, height, fx, fy, cx, cy, k1, k2, p1, p2, k3);

			int is_rgb = file_settings["Camera.RGB"];
			is_rgb_order_ = is_rgb;

			if (is_rgb_order_)
				OPENSLAM_INFO << "- color order: RGB (ignored if grayscale)" << std::endl;
			else
				OPENSLAM_INFO << "- color order: BGR (ignored if grayscale)" << std::endl;

			// ����ORB����ز���
			int feature_num = file_settings["ORBextractor.nFeatures"];
			float scale_factor = file_settings["ORBextractor.scaleFactor"];
			int levels_num = file_settings["ORBextractor.nLevels"];
			int default_fast_threshold = file_settings["ORBextractor.iniThFAST"];
			int min_fast_threshold = file_settings["ORBextractor.minThFAST"];

			extractor_ = new ORBextractor(feature_num, scale_factor, levels_num, default_fast_threshold, min_fast_threshold);

			init_orb_extractor_ = new ORBextractor(2 * feature_num, scale_factor, levels_num, default_fast_threshold, min_fast_threshold);

			OPENSLAM_INFO << std::endl << "ORB Extractor Parameters: " << std::endl;
			OPENSLAM_INFO << "- Number of Features: " << feature_num << std::endl;
			OPENSLAM_INFO << "- Scale Levels: " << levels_num << std::endl;
			OPENSLAM_INFO << "- Scale Factor: " << scale_factor << std::endl;
			OPENSLAM_INFO << "- Initial Fast Threshold: " << default_fast_threshold << std::endl;
			OPENSLAM_INFO << "- Minimum Fast Threshold: " << min_fast_threshold << std::endl;

			stage_ = STAGE_FIRST_FRAME;//��ʼ����ʱ�����ô���֡Ϊ��һ֡
			
		}

		TrackingMono::~TrackingMono()
		{
			delete cam_;
		}

		void TrackingMono::initialize()
		{

		}

		void TrackingMono::addImage(const cv::Mat& img, double timestamp)
		{
			// ��ʼ����֡
			new_frame_ = std::make_shared<Frame>(cam_, img.clone(), timestamp, extractor_,orb_vocabulary_, is_rgb_order_);
			bool res = false;
			if (stage_ == STAGE_DEFAULT_FRAME)
				res = processFrame();
			else if (stage_ == STAGE_SECOND_FRAME)
				res = processSecondFrame();
			else if (stage_ == STAGE_FIRST_FRAME)
				res = processFirstFrame();

		}

		bool TrackingMono::processFirstFrame()
		{
			bool is_success = true;
			is_success = initializer_.addFirstFrame(new_frame_);
			if (is_success)//�����ʼ���ɹ����򽫵�ǰ֡תΪ�ؼ�֡��������BoW
			{
				KeyFramePtr init_keyframe = std::make_shared<KeyFrame>(*new_frame_);
				init_keyframe->computeBoW();
				//����ؼ�֡����ͼ��
				map_.addKeyframe(init_keyframe);
				stage_ = STAGE_SECOND_FRAME;
			}
			return is_success;
		}

		bool TrackingMono::processSecondFrame()
		{
			bool is_success = true;
			is_success = initializer_.addSecondFrame(new_frame_);
			if (is_success)//�����ʼ���ɹ����򽫵�ǰ֡תΪ�ؼ�֡��������BoW
			{
				KeyFramePtr second_keyframe = std::make_shared<KeyFrame>(*new_frame_);
				second_keyframe->computeBoW();
				//����ؼ�֡����ͼ��
				map_.addKeyframe(second_keyframe);
				stage_ = STAGE_DEFAULT_FRAME;
			}
			return true;
		}

		bool TrackingMono::processFrame()
		{
			return true;
		}
	}
}