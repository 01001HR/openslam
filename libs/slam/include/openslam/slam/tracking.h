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

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP Tracking
		{
		public:
			/** ����״̬
			*/
			enum TrackingState
			{
				TRACKING_SYSTEM_NOT_READY = -1,
				TRACKING_NO_IMAGES_YET = 0,
				TRACKING_NOT_INITIALIZED = 1,
				TRACKING_OK = 2,
				TRACKING_LOST = 3
			};
		};
	}
}

#endif // OPENSLAM_SLAM_TRACKING_H_