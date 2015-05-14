#include <cstdint>
#include <random>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <cstddef>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <cmath>
#include <utility>
#include <list>

#include "common.hpp"
#include "streamops.hpp"

namespace zpogif { namespace detail {

	double color_distance(Rgb x, Rgb y)
	{
		double r = (x.r - y.r);
		double g = (x.g - y.g);
		double b = (x.b - y.b);
		return std::sqrt(r*r + g*g + b*b);
	}

	void clusterize(const std::set<Rgb>& colors, std::map<Rgb, uint8_t>& idxmap, std::vector<Rgb>& color_table)
	{
		if (colors.size() <= 256)
		{
			uint8_t idx = 0;
			for (auto color : colors)
			{
				color_table.push_back(color);
				idxmap[color] = idx;
				idx++;
			}
			return;
		}
		else
		{
			std::set<size_t> random_indices;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, colors.size() - 1);
			while (random_indices.size() != 256)
			{
				random_indices.insert(dis(gen));
			}
			
			std::vector<Rgb> colors_vec;
			std::copy(colors.begin(), colors.end(), std::back_inserter(colors_vec));
			
			for (auto idx : random_indices)
			{
				color_table.push_back(colors_vec[idx]);
			}
			
			std::multimap<uint8_t, Rgb> clusters;
			
			for (int i = 0; i < 20; i++)
			{
				clusters.clear();
				for (auto color : colors)
				{
					uint8_t best_idx;
					Rgb best_cluster;
					double best_distance;
					uint8_t idx = 0;
					for (auto cluster : color_table)
					{
						if (idx == 0)
						{
							best_idx = idx;
							best_cluster = cluster;
							best_distance = color_distance(color, cluster);
						}
						else
						{
							double distance = color_distance(color, cluster);
							if (distance < best_distance)
							{
								best_cluster = cluster;
								best_idx = idx;
								best_distance = distance;
							}
						}
						idx++;
					}
					
					clusters.insert(std::make_pair(best_idx, color));
				}
				
				double error = 0.0;
				uint8_t idx = 0;
				for (auto cluster : color_table)
				{
					unsigned r = 0, g = 0, b = 0;
					auto colors_in_cluster = clusters.equal_range(idx);
					unsigned count = 0;
					for (auto it = colors_in_cluster.first; it != colors_in_cluster.second; ++it)
					{
						r += it->second.r;
						g += it->second.g;
						b += it->second.b;
						count++;
					}
					
					Rgb new_cluster = count == 0 ? cluster : Rgb(r / count, g / count, b / count);
					error += color_distance(new_cluster, cluster);
					color_table[idx] = new_cluster;
					
					idx++;
				}
				
				if (error <= 0.5) break;
			}
		}
	}
}}
