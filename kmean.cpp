#include "kmean.h"

using namespace std;
using namespace cv;

const int MAX_ITERATIONS = 500;

double distance(const Vec3b& a, const Vec3b& b) {
    return (a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]) + (a[2] - b[2])*(a[2] - b[2]);
}

Vec3b getNearColor(const vector<Vec3b>& palette, const Vec3b& color) {
    Vec3b nearestColor = palette[0];
    double minDistance = distance(palette[0], color);

    for (const auto& paletteColor : palette) {
        double currentDistance = distance(paletteColor, color);
        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            nearestColor = paletteColor;
        }
    }

    return nearestColor;
}

Vec3b calculateCentroid(const vector<Vec3b>& cluster) {
    int sumB = 0, sumG = 0, sumR = 0;
    for (const auto& color : cluster) {
        sumB += color[0];
        sumG += color[1];
        sumR += color[2];
    }
    size_t count = cluster.size();
    return { static_cast<uchar>(sumB / count), static_cast<uchar>(sumG / count), static_cast<uchar>(sumR / count) };
}

vector<Vec3b> kMeans(const vector<Vec3b>& palette, int k) {
    srand(time(0));

    vector<Vec3b> centroids(k);
    for (int i = 0; i < k; ++i) {
        centroids[i] = palette[rand() % palette.size()];
    }

    vector<int> labels(palette.size());
    bool changed;
    int iterations = 0;

    do {
        changed = false;

        for (size_t i = 0; i < palette.size(); ++i) {
            int bestLabel = 0;
            double bestDistance = numeric_limits<double>::max();

            for (int j = 0; j < k; ++j) {
                double dist = distance(palette[i], centroids[j]);
                if (dist < bestDistance) {
                    bestDistance = dist;
                    bestLabel = j;
                }
            }

            if (labels[i] != bestLabel) {
                labels[i] = bestLabel;
                changed = true;
            }
        }

        vector<vector<Vec3b>> clusters(k);
        for (size_t i = 0; i < palette.size(); ++i) {
            clusters[labels[i]].push_back(palette[i]);
        }

        for (int j = 0; j < k; ++j) {
            if (!clusters[j].empty()) {
                centroids[j] = calculateCentroid(clusters[j]);
            }
        }

        iterations++;

    } while (changed && iterations < MAX_ITERATIONS);

    return centroids;
}
