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

			/** \brief ��ͼ��ɾ���ؼ�֡
			*/
			void eraseKeyFrame(KeyFramePtr keyframe);

			/** \brief ��ͼ�����map point
			*/
			void addMapPoint(MapPoint* map_point);

			/** \brief ��ͼ��ɾ�� map point
			*/
			void eraseMapPoint(MapPoint* map_point);

			/** \brief ��ͼ�д洢��map point�ĸ���
			*/
			long unsigned int mapPointsInMap();

			/** \brief ��ͼ�д洢�Ĺؼ�֡�ĸ���
			*/
			long unsigned int keyFramesInMap();

			/** \brief �õ���ͼ�е����йؼ�֡
			*/
			std::vector<KeyFramePtr> getAllKeyFrames();

			/** \brief �õ���ͼ�е�����map point
			*/
			std::vector<MapPoint*> getAllMapPoints();
		protected:
			std::set< KeyFramePtr > set_keyframes_;          //!< ��ͼ�д洢�����йؼ�֡
			std::set< MapPoint * > set_map_points_;//!< ��ͼ�д洢��MapPoint
			long unsigned int max_key_frame_id_;//!< ���ڼ�¼��ͼ�йؼ�֡�����id
			std::mutex mutex_map_;//!<map �����õ���
		};
	}
}

#endif // OPENSLAM_SLAM_MAP_H_
