#ifndef OPENSLAM_SLAM_MAP_POINT_H_
#define OPENSLAM_SLAM_MAP_POINT_H_

#include <openslam/slam/link_pragmas.h>
#include <list>
#include <memory>
#include <mutex>
#include <opencv2/core/core.hpp>

namespace openslam
{
	namespace slam
	{
		class Feature;

		class SLAM_IMPEXP MapPoint
		{
		public:
			MapPoint(const cv::Mat &pos);
			~MapPoint();
			/**��ӹ۲⵽�Ķ�Ӧ����*/
			void addFeatureRef(Feature* ftr);

			/** \brief �����ͬһ��mappoint���ж�Ӧ�����������������ӽ��д���Ѱ�����������
			*/
			void computeDistinctiveDescriptors();
		public:
			long unsigned int id_;                                   //!< ��Ψһ��id
			static long unsigned int map_point_counter_;             //!< ������ļ�������������Ψһ��id
			cv::Mat world_position_;                                 //!<����������ϵ�е�λ��
			cv::Mat normal_vector_;                                  //!<ƽ���۲ⷽ��
			std::list<Feature*>   obs_;                              //!< ��Ӧ����������
			int obs_num_;                                            //!< �۲쵽��map point�Ĺؼ�֡�ĸ���
			std::mutex mutex_features_;//!<��������д���п���
			cv::Mat descriptor_;//!<�õ���map point ��Ӧ����������ӣ��������ؼ�֡�Ըõ�����������С��
		};

	}
}

#endif // OPENSLAM_SLAM_MAP_POINT_H_
