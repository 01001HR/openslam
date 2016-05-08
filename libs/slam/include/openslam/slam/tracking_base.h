/*************************************************************************
* �ļ����� tracking
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/9
*
* ˵���� �ο�rpg_svo,orb_slam2
*************************************************************************/
#ifndef OPENSLAM_SLAM_TRACKING_H_
#define OPENSLAM_SLAM_TRACKING_H_
#include <openslam/slam/frame.h>
#include <openslam/slam/map.h>

namespace openslam
{
	namespace slam
	{
		/** �����࣬��Ҫ���ڽ���֡��������֮��Ӧ�������̬�����������ʱ����һ���ؼ�֡������һ����
		*   ��map point�������������ʧ��ִ���ض�λ��
		*/
		class SLAM_IMPEXP TrackingBase
		{
		public:
			/** ����״̬
			*/
			enum TrackingState
			{
				TRACKING_SYSTEM_NOT_READY = -1,//״̬��Ҫ���ڵ���ʻ��Ĺ���
				TRACKING_NO_IMAGES_YET = 0,
				TRACKING_NOT_INITIALIZED = 1,
				TRACKING_OK = 2,
				TRACKING_LOST = 3
			};

			TrackingBase(ORBVocabulary *voc);
			virtual ~TrackingBase();
			/**��ͬ�Ĵ�������ʼ����һ��*/
			virtual void initialize() = 0;

			/**����һ��ͼ�񣬸���Ŀ�Ľӿ�*/
			virtual void addImage(const cv::Mat& img, double timestamp) = 0;
		protected:
			TrackingState tracking_state_;//!<��ʾ���õĸ���״̬
			Map  *map_;
			ORBVocabulary *orb_vocabulary_;
			bool is_only_tracking_;//!< �����true,�����ִֻ�ж�λ
			bool is_vo_;
			cv::Mat motion_model_;

		};
	}
}

#endif // OPENSLAM_SLAM_TRACKING_H_