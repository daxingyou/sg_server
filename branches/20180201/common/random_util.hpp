#ifndef __COMMON_RANDOM_UTIL_HPP__
#define __COMMON_RANDOM_UTIL_HPP__

#include <vector>
#include <algorithm>

static unsigned int seedp = time(NULL);

struct odds_t
{
	uint32_t upNum;
	uint32_t downNum;
};


struct random_util_t
{
	static bool random_from_vec(const std::vector<uint32_t>& from_vec, const std::vector<uint32_t>& exclude_vec, uint32_t& out_val)
	{
		std::vector<uint32_t> tmp_from_vec(from_vec.begin(), from_vec.end());
		for (auto exclude_val : exclude_vec)
		{
			std::vector<uint32_t>::iterator it = std::find(tmp_from_vec.begin(), tmp_from_vec.end(), exclude_val);
			if (it != tmp_from_vec.end())
			{
				tmp_from_vec.erase(it);
			}
		}
		if (tmp_from_vec.empty())
		{
			return false;
		}
		int rand_index = rand() % tmp_from_vec.size();
		out_val = tmp_from_vec[rand_index];
		return true;
	}

    template<typename T>
    static bool random_from_vec(const std::vector<T>& from_vec, int32_t random_num, std::vector<T>& to_vec)
    {
        std::vector<T> tmp_from_vec(from_vec.begin(), from_vec.end());
        to_vec.clear();
        int32_t from_vec_size = tmp_from_vec.size();
        if (from_vec_size <= 0 || random_num <= 0)
        {
            return false;
        }
        int32_t tmp_num = random_num;
        if (from_vec_size < tmp_num)
        {
            tmp_num = from_vec_size;
        }

        for (uint32_t i = 0; i < tmp_num; ++i)
        {
            int random_index = rand() % from_vec_size;
            to_vec.push_back(tmp_from_vec[random_index]);
            tmp_from_vec[random_index] = tmp_from_vec[from_vec_size-1];
            --from_vec_size;
        }

        return true;
    }


    static bool random_from_int_range(int32_t min_num, int32_t max_num, int32_t random_num, std::vector<int32_t>& to_vec, int32_t* exclude_num=NULL)
    {
        if (min_num < 0 || max_num <= 0 || random_num <= 0)
        {
            return false;
        }
        if (min_num > max_num)
        {
            return false;
        }
        if (min_num == max_num)
        {
            to_vec.push_back(min_num);
            return true;
        }
        if (max_num - min_num <= random_num)
        {
            for (int32_t i = min_num; i < max_num; ++i)
            {
                to_vec.push_back(i);
            }
        }
        else
        {
            int32_t try_times = 0;
            std::set<int32_t> tmp_set;
            while (tmp_set.size() < (uint32_t)random_num)
            {
                int32_t tmp_num = (rand() % (max_num - min_num + 1)) + min_num;

                if (NULL != exclude_num)
                {
                    if (tmp_num == *exclude_num)
                    {
                        continue;
                    }
                }

                tmp_set.insert(tmp_num);

                try_times += 1;
                if (try_times >= 1000)
                {
                    break;
                }
            }
            std::set<int32_t>::iterator itr = tmp_set.begin();
            std::set<int32_t>::iterator itr_end = tmp_set.end();
            for (; itr != itr_end; ++itr)
            {
                to_vec.push_back(*itr);
            }
        }
        return true;
    }

	//随机产生min~max之间的数字，包裹min和max
	static int randBetween(int min, int max)
	{
		if (min == max)
		{
			return min;
		}
		else if (min > max)
		{
			return -1;
		}
        else
        {
            return min + (int)(((double)max - (double)min + 1.0) * rand_r(&seedp) / (RAND_MAX + 1.0));
        }
	}

	// 随机产生min~max之间的数字，包裹min 不包含max
	static int randMin(int min, int max)
	{
		if (min >= max)
		{
			return -1;
		}
		else
		{
			return min + (int)(((double)max - (double)min) * rand_r(&seedp) / (RAND_MAX + 1.0));
		}
	}

	//获取几分之的几率
	static bool selectByOdds(const unsigned int upNum, const unsigned int downNum)
	{
		unsigned int m_rand;
		if (downNum < 1) return false;
		if (upNum < 1) return false;
		if (upNum > downNum - 1) return true;
		m_rand = 1 + (unsigned int)((double)downNum * rand_r(&seedp) / (RAND_MAX + 1.0));
		return (m_rand <= upNum);
	}

	//获取几分之几的几率
	static bool selectByt_Odds(const odds_t &odds)
	{
		return selectByOdds(odds.upNum, odds.downNum);
	}

	//获取百分之的几率
	static bool selectByPercent(const uint32_t percent)
	{
		return selectByOdds(percent, 100);
	}

	//获取千分之的几率
	static bool selectByThousand(const uint32_t thousand)
	{
		return selectByOdds(thousand, 1000);
	}

	//获取万分之的几率
	static bool selectByTenTh(const uint32_t tenth)
	{
		if (tenth >= 10000)
		{
			return true;
		}

		return selectByOdds(tenth, 10000);
	}

	//获取十万分之的几率
	static bool selectByLakh(const uint32_t lakh)
	{
		return selectByOdds(lakh, 100000);
	}

	//获取亿分之之的几率
	static bool selectByOneHM(const uint32_t lakh)
	{
		return selectByOdds(lakh, 100000000);
	}

};

#endif
