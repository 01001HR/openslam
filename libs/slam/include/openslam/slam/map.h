#ifndef OPENSLAM_SLAM_MAP_H_
#define OPENSLAM_SLAM_MAP_H_
#include <openslam/slam/link_pragmas.h>
#include <mutex>
#include <openslam/slam/keyframe.h>
#include <openslam/slam/map_point.h>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP Map
		{
		public:
			Map();
			~Map();
			/**����ͼ�����һ���µĹؼ�֡*/
			void addKeyframe(KeyFramePtr new_keyframe);

		protected:
			std::list< KeyFramePtr > list_keyframes_;          //!< ��ͼ�д洢�����йؼ�֡
			std::list< MapPoint * > list_map_points_;//!< ��ͼ�д洢��MapPoint
			long unsigned int max_key_frame_id_;//!< ���ڼ�¼��ͼ�йؼ�֡�����id
			std::mutex mutex_map_;//!<map �����õ���
		};
	}
}

#endif // OPENSLAM_SLAM_MAP_H_
