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
#include <memory>
#include <opencv2/opencv.hpp>

#include <openslam/slam/pinhole_camera.h>
#include <openslam/slam/orb_extractor.h>

namespace openslam
{
	namespace slam
	{
		//Ŀǰͨ��������Ϊ�̶�ֵ�����ڽ��в�������
#define FRAME_GRID_ROWS 48
#define FRAME_GRID_COLS 64
		class Feature;

		typedef std::vector<Feature *> Features;

		class SLAM_IMPEXP Frame
		{
		public:
			/** ֡�Ĺ��죬������Ӧ�����������Ӧԭʼ֡����Ӧ��ʱ����
			*/
			Frame(PinholeCamera* cam, const cv::Mat& img, double timestamp,ORBextractor* extractor);
			~Frame();

			/** ��ͼ�����������
			*/
			void addFeature(Feature* ftr);

			/** �õ��ؼ��������
			*/
			int getKeypointsNum(){ return keypoints_num_; }

			/** \brief ���������Ƶ�ĳ�������ڣ��������������ƥ��
			*/
			std::vector<size_t> getFeaturesInArea(const float &x, const float  &y, const float  &r, const int min_level = -1, const int max_level = -1) const;


		private:
			/** \brief ��ͼ�����ORB�������
			*/
			void extractORB(const cv::Mat &image);

			/** \brief ����ͼ��ı߽磬����ͼ�����
			*/
			void computeImageBounds(const cv::Mat &image);

			/** \brief ÿ��ͼ��֡�����ʱ����ã����ڽ���������䵽�������Լӿ�����ƥ�䡣
			*/
			void assignFeaturesToGrid();

			/** �������������ڵ�Ԫ�񣬲��ڸ����ڷ���false
			*/
			bool posInGrid(const cv::KeyPoint &kp, int &pos_x, int &pos_y);

		public:
			static long unsigned int     frame_counter_;//!< ����֡�ļ���������������֡��Ψһid
			long unsigned int            id_;           //!< ֡��id
			double                       timestamp_;    //!< ֡��ʱ���
			PinholeCamera*               cam_;          //!< ���ģ��
			cv::Mat                      img_;          //!< ֡��Ӧ��ԭʼͼ��
			float                        scale_factor_; //!< ��Ӧ������ͼ��ĳ߶�����
			int                          levels_num_;   //!< ��Ӧ�������Ĳ���
			Features                     features_;     //!< ֡��Ӧ������
			cv::Mat                      T_f_w_;        //!< ����������ϵ(w)orldת�����������ϵ(f)rame�����Ա任Rt
			ORBextractor*                extractor_;    //!< ��������ȡ�ŵ�֡��


			// �����ͼ��߽���Ҫ���ǻ���ͼ��			
			static float                 min_bound_x_;//!<�����������С�߽�x������
			static float                 max_bound_x_;//!<������������߽�x������
			static float                 min_bound_y_;//!<�����������С�߽�y������
			static float                 max_bound_y_;//!<������������߽�y������
			static float                 grid_element_height_;//!<����֮����ӵ�����
			static float                 grid_element_width_;//!<����֮����ӵĿ��
			static bool                  is_initial_computations_;//!<����һ�μ���ı�ʶ��ture��ʾ�Ѿ��������
			std::vector<std::size_t>     grid_[FRAME_GRID_COLS][FRAME_GRID_ROWS];//!<һ��ͼ�����ĸ���,����洢�����������

		private:
			int                          keypoints_num_;//!< ������ĸ���
		};

		typedef std::shared_ptr<Frame> FramePtr;
	}
}

#endif // OPENSLAM_SLAM_FRAME_H_
