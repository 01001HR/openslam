#include <openslam/slam/tracking_base.h>
#include <openslam/slam/orb_matcher.h>
#include <openslam/slam/feature.h>

namespace openslam
{
	namespace slam
	{
		TrackingBase::TrackingBase(ORBVocabulary *voc):
			orb_vocabulary_(voc),
			tracking_state_(TRACKING_NO_IMAGES_YET),
			is_only_tracking_(false),
			is_vo_(false),
			is_rgb_order_(false),
			map_(nullptr)
		{
			//������ͼ
			map_ = new Map();
		}
		TrackingBase::~TrackingBase()
		{
			delete map_;
		}

		void TrackingBase::track()
		{
			bool is_ok = true;
			if (!is_only_tracking_)//��Ӿֲ��Ż�
			{
			}
			else// ֻ�Ǹ���
			{
				if (tracking_state_ == TRACKING_LOST)
				{

				}
				else
				{
					if (!is_vo_)
					{
						if (!motion_model_.empty())
						{
							is_ok = trackWithMotionModel();
						}
						else
						{
							is_ok = trackReferenceKeyFrame();
						}
					}
					else
					{

					}
				}

				cur_frame_->ref_keyframe_ = ref_keyframe_;

				if (!is_only_tracking_)
				{
					if (is_ok)
						is_ok = trackLocalMap();
				}
				else
				{
					if (is_ok && !is_vo_)
						is_ok = trackLocalMap();
				}
				//���ٽ���֮��ȷ������״̬
				if (is_ok)
					tracking_state_ = TRACKING_OK;
				else
					tracking_state_ = TRACKING_LOST;

				// ���������ȷ������һ������Ҫȷ����ǰ֡�Ƿ�Ϊ�ؼ�֡
				if (is_ok)
				{
					if (!last_frame_->Tcw_.empty())
					{
						cv::Mat last_Twc = cv::Mat::eye(4, 4, CV_32F);
						last_frame_->getPose().copyTo(last_Twc);
						motion_model_ = cur_frame_->Tcw_*last_Twc;
					}
					else
					{
						motion_model_ = cv::Mat();
					}

					// ɾ��û�ж�Ӧ���mappoint
					for (int i = 0; i < cur_frame_->getKeypointsNum(); i++)
					{
						MapPoint* map_point = cur_frame_->features_[i]->map_point_;
						if (map_point)
						{
							if (map_point->observations() < 1)
							{
								cur_frame_->features_[i]->map_point_ = static_cast<MapPoint*>(NULL);
							}
						}
					}
				}
			}
		}

		bool TrackingBase::trackWithMotionModel()
		{
			ORBmatcher matcher(options_.track_motion_model_matcher_nnratio, true);
			return true;
		}

		bool TrackingBase::trackReferenceKeyFrame()
		{
			return true;
		}

		bool TrackingBase::trackLocalMap()
		{
			return true;
		}
	}
}