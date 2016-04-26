#include <openslam/slam/orb_matcher.h>
#include <openslam/slam/frame.h>
#include <openslam/slam/feature.h>

namespace openslam
{
	namespace slam
	{
		// ����ȷ������������������������ֵ
		const int ORBmatcher::TH_HIGH = 100;
		// ����ȷ���������������������С��ֵ
		const int ORBmatcher::TH_LOW = 50;
		const int ORBmatcher::HISTO_LENGTH = 30;

		ORBmatcher::ORBmatcher(float nnratio, bool check_orientation):
			nn_ratio_(nnratio),
			is_check_orientation_(check_orientation)
		{

		}

		ORBmatcher::~ORBmatcher(){}

		int ORBmatcher::searchForInitialization(FramePtr ref_frame, FramePtr cur_frame,
			std::vector<cv::Point2f> &prev_matched, std::vector<int> &matches_ref_cur, int window_size)
		{
			int nmatches = 0;
			int keypoints_num = ref_frame->getKeypointsNum();
			matches_ref_cur = std::vector<int>(keypoints_num, -1);
			std::vector<int> rot_hist[HISTO_LENGTH];
			for (int i = 0; i < HISTO_LENGTH; i++)
				rot_hist[i].reserve(500);
			// ����ֱ��ͼ�ı�������
			const float factor = 1.0f / HISTO_LENGTH;

			std::vector<int> matched_distance(keypoints_num, (std::numeric_limits<int>::max)());
			std::vector<int> matches_cur_ref(keypoints_num, -1);

			for (size_t i1 = 0, iend1 = keypoints_num; i1 < iend1; i1++)
			{
				cv::KeyPoint kp1 = ref_frame->features_[i1]->undistored_keypoint_;
				int level1 = kp1.octave;
				if (level1 > 0)//ֻ������ԭʼͼ��
					continue;
				// �ڵ�ǰ֡�в��ҿ���ƥ�������������
				std::vector<size_t> keypoint_indices = cur_frame->getFeaturesInArea(prev_matched[i1].x, prev_matched[i1].y, window_size, level1, level1);

				if (keypoint_indices.empty())
					continue;
				// ��ȡ��Ӧ�ο�֡����������
				cv::Mat cur_des = ref_frame->features_[i1]->descriptor_;

				int best_dist = (std::numeric_limits<int>::max)();
				int best_dist2 = (std::numeric_limits<int>::max)();
				int best_index2 = -1;
				// �Ե�ǰ֡�п��ܵ���������б���
				for (std::vector<size_t>::iterator vit = keypoint_indices.begin(); vit != keypoint_indices.end(); vit++)
				{
					size_t i2 = *vit;
					// ��Ӧ��ǰ֡����������
					cv::Mat ref_des = cur_frame->features_[i2]->descriptor_;

					int dist = descriptorDistance(cur_des, ref_des);

					if (matched_distance[i2] <= dist)
						continue;
					// �ҵ���С��ǰ��������
					if (dist < best_dist)
					{
						best_dist2 = best_dist;
						best_dist = dist;
						best_index2 = i2;
					}
					else if (dist < best_dist2)
					{
						best_dist2 = dist;
					}
				}
				// ȷ����С����С����ֵ
				if (best_dist <= TH_LOW)
				{
					// ��ȷ������С�������nn_ratio_Ҫ������С���룬��Ҫȷ����ƥ��Ƚ�³��
					if (best_dist < (float)best_dist2*nn_ratio_)
					{
						// ����Ѿ�ƥ�䣬��˵����ǰ�����Ѿ��й���Ӧ����ͻ���������Ӧ���Ƴ���ƥ��
						if (matches_cur_ref[best_index2] >= 0)
						{
							matches_ref_cur[matches_cur_ref[best_index2]] = -1;
							nmatches--;
						}
						// ��¼ƥ��
						matches_ref_cur[i1] = best_index2;
						matches_cur_ref[best_index2] = i1;
						matched_distance[best_index2] = best_dist;
						nmatches++;

						if (is_check_orientation_)
						{
							float rot = ref_frame->features_[i1]->undistored_keypoint_.angle - cur_frame->features_[best_index2]->undistored_keypoint_.angle;
							if (rot < 0.0)
								rot += 360.0f;
							int bin = round(rot*factor);
							if (bin == HISTO_LENGTH)
								bin = 0;
							assert(bin >= 0 && bin < HISTO_LENGTH);
							rot_hist[bin].push_back(i1);//�õ�ֱ��ͼ
						}
					}
				}

			}

			if (is_check_orientation_)
			{
				int ind1 = -1;
				int ind2 = -1;
				int ind3 = -1;

				computeThreeMaxima(rot_hist, HISTO_LENGTH, ind1, ind2, ind3);

				for (int i = 0; i < HISTO_LENGTH; i++)
				{
					// �Կ��ܵ�һ�µķ���Ͳ��迼��
					if (i == ind1 || i == ind2 || i == ind3)
						continue;
					// ��ʣ�·���һ�µ�ƥ������޳�
					for (size_t j = 0, jend = rot_hist[i].size(); j < jend; j++)
					{
						int idx1 = rot_hist[i][j];
						if (matches_ref_cur[idx1] >= 0)
						{
							matches_ref_cur[idx1] = -1;
							nmatches--;
						}
					}
				}

			}

			//���� prev matched
			for (size_t i1 = 0, iend1 = matches_ref_cur.size(); i1 < iend1; i1++)
				if (matches_ref_cur[i1] >= 0)
					prev_matched[i1] = cur_frame->features_[matches_ref_cur[i1]]->undistored_keypoint_.pt;

			return nmatches;

		}

		void ORBmatcher::computeThreeMaxima(std::vector<int>* histo, const int L, int &ind1, int &ind2, int &ind3)
		{
			int max1 = 0;
			int max2 = 0;
			int max3 = 0;

			for (int i = 0; i < L; i++)
			{
				const int s = histo[i].size();
				if (s > max1)
				{
					max3 = max2;
					max2 = max1;
					max1 = s;
					ind3 = ind2;
					ind2 = ind1;
					ind1 = i;
				}
				else if (s > max2)
				{
					max3 = max2;
					max2 = s;
					ind3 = ind2;
					ind2 = i;
				}
				else if (s > max3)
				{
					max3 = s;
					ind3 = i;
				}
			}
			// ����˸߶�����߶����̫��Ͳ��迼�ǣ�������ֵΪ-1
			if (max2 < 0.1f*(float)max1)
			{
				ind2 = -1;
				ind3 = -1;
			}
			else if (max3 < 0.1f*(float)max1)
			{
				ind3 = -1;
			}
		}

		int ORBmatcher::descriptorDistance(const cv::Mat &a, const cv::Mat &b)
		{
			//TODO:???��������д��˽�
			const int *pa = a.ptr<int32_t>();
			const int *pb = b.ptr<int32_t>();

			int dist = 0;

			for (int i = 0; i < 8; i++, pa++, pb++)
			{
				unsigned  int v = *pa ^ *pb;
				v = v - ((v >> 1) & 0x55555555);
				v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
				dist += (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
			}

			return dist;
		}

	}
}