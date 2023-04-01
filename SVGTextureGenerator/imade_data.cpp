#include "image_data.h"
#include <glm/detail/func_geometric.hpp>
#include <cmath>

image_data::image_data()
{

}

void image_data::calculate_neighbor() {

    for (auto& [key1, sample] : this->sample_data) {
        for (auto& [key2, target] : this->sample_data) {
            float phy_distance = glm::distance(sample->position, target->position);
            if (phy_distance < this->neighbor_r) {
                sample->neighborhood[target->index] = phy_distance;
                //B->neighborhood[A->index]=distance;
            }
        }
    }
};


float image_data::calculate_distance(sample* nSo, sample* nS1) {
    float distance = INFINITY;
    float sample_distance = 0;//
    float w;//sample weighted difference
    for (auto& [key1, distance1] : nSo->neighborhood) {
        for (auto& [key2, distance2] : nS1->neighborhood) {

            sample_distance = abs(distance1 - distance2);
            std::vector<int> hs1 = histogram_distribution(this->sample_data[key1].get()->position, cluster_data[this->sample_data[key1].get()->cluster_ID].get());
            std::vector<int> hs2 = histogram_distribution(this->sample_data[key2].get()->position, cluster_data[this->sample_data[key2].get()->cluster_ID].get());
            w = 0;
            for (int i = 0; i < 24; i++) {
                w += pow(hs1[i] - hs2[i], 2.f) / (hs1[i] + hs2[i]);
            }

            sample_distance += 0.5 * w;

            distance = min(distance, sample_distance);
        }
    }
    return distance;
}


std::vector<int> image_data::histogram_distribution(vec2 input, cluster* m_cluster) {
    std::vector<int> output(24, 0);

    for (auto& m_sample : m_cluster->sample_list) {
        vec2 d_v = m_sample - input;
        float distance = glm::distance(m_sample, input);
        int multiplyer = 0;
        if (distance < 1) {
            multiplyer = 0;
        }
        else if (distance < 3) {
            multiplyer = 1;
        }
        else {
            multiplyer = 2;

        }

        if (d_v[0] > 0 && d_v[1] > 0 && d_v[0] > d_v[1]) {
            output[0 + multiplyer * 8] += 1;
        }
        else if (d_v[0] > 0 && d_v[1] > 0 && d_v[0] < d_v[1]) {
            output[1 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1]>0 && d_v[0] < d_v[1]) {
            output[2 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1]>0 && abs(d_v[0]) > abs(d_v[1])) {
            output[3 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1]<0 && abs(d_v[0])>abs(d_v[1])) {
            output[4 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1] < 0 && abs(d_v[0]) < abs(d_v[1])) {
            output[5 + multiplyer * 8] += 1;
        }
        else if (d_v[0] > 0 && d_v[1] < 0 && abs(d_v[0]) < abs(d_v[1])) {
            output[6 + multiplyer * 8] += 1;
        }
        else if (d_v[0] > 0 && d_v[1]<0 && abs(d_v[0])>abs(d_v[1])) {
            output[7 + multiplyer * 8] += 1;
        }

    }

    return output;
}

