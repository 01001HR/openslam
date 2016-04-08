/*************************************************************************
* �ļ����� frame
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/9
*
* ˵���� �ο�rpg_svo,orb_slam2
*************************************************************************/
#ifndef OPENSLAM_SLAM_FRAME_H_
#define OPENSLAM_SLAM_FRAME_H_
#include <openslam/slam/link_pragmas.h>
#include <vector>
#include <opencv2/opencv.hpp>

#include <openslam/slam/pinhole_camera.h>
#include <openslam/slam/orb_extractor.h>

namespace openslam
{
	namespace slam
	{
		class Feature;

		typedef std::vector<Feature> Features;

		class SLAM_IMPEXP Frame
		{
		public:
			/** ֡�Ĺ��죬������Ӧ�����������Ӧԭʼ֡����Ӧ��ʱ����
			*/
			Frame(PinholeCamera* cam, const cv::Mat& img, double timestamp,ORBextractor* extractor);
			~Frame();

		private:
			/** \brief ��ͼ�����ORB�������
			*/
			void extractORB(const cv::Mat &image);

		public:
			static long unsigned int     frame_counter_;//!< ����֡�ļ���������������֡��Ψһid
			long unsigned int            id_;           //!< ֡��id
			double                       timestamp_;    //!< ֡��ʱ���
			PinholeCamera*               cam_;          //!< ���ģ��
			cv::Mat                      img_;          //!< ֡��Ӧ��ԭʼͼ��
			float                        scale_factor_; //!< ��Ӧ������ͼ��ĳ߶�����
			int                          levels_num_;   //!< ��Ӧ�������Ĳ���
			Features                     features_;     //!< ֡��Ӧ������
			int                          keypoints_num_;//!< ������ĸ���
			ORBextractor*                extractor_;    //!< ��������ȡ�ŵ�֡��
		};
	}
}

#endif // OPENSLAM_SLAM_FRAME_H_
