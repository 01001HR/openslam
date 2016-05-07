#ifndef OPENSLAM_SLAM_KEYFRAME_H_
#define OPENSLAM_SLAM_KEYFRAME_H_

#include <openslam/slam/frame.h>
#include <mutex>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP KeyFrame : public Frame
		{
		public:
			KeyFrame(const Frame &frame);
			~KeyFrame();

			/** \brief ������ȣ�ת����ǰ֡������ϵ�£���������������Ӧ��map point��z����ֵ����Ϊ���ֵ
			*/
			float computeSceneMedianDepth();
		public:
			static long unsigned int keyframe_counter_;//!< �����ؼ�֡�ļ���������������֡��Ψһid
			long unsigned int keyframe_id_;//!< �ؼ�֡��Ψһid

		protected:
			std::mutex mutex_pose_;
			std::mutex mutex_features_;

		};

		typedef std::shared_ptr<KeyFrame> KeyFramePtr;
	}
}

#endif // OPENSLAM_SLAM_KEYFRAME_H_
