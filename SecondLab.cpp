#include "kmean.h" 

using namespace std;
using namespace cv;

void saveImage(Mat* image, string name) {
    imwrite("D:/MEPHI/KG/SecondLab/SecondLab/result/" + name, *image);
}

Mat loadImage(string name) {
    Mat image = imread("D:/MEPHI/KG/SecondLab/SecondLab/img/" + name);
    if (image.empty()) {
        cout << "Can not load image " << name << endl;
    }
    return image;
}

void showImage(const Mat& image) {
    imshow("Display window", image);
}

vector<Vec3b> getPaletteImage(const Mat& image) {
    vector<Vec3b> imagePalette;

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            Vec3b color = image.at<Vec3b>(y, x);
            imagePalette.push_back(color);
        }
    }

    return imagePalette;
}


vector<int> getError(const Vec3b color, const Vec3b newColor) {
    return {
        color[0] - newColor[0],
        color[1] - newColor[1],
        color[2] - newColor[2],
    };
}


Mat FloydSteinberg(const Mat& image, const vector<Vec3b> newPalette) {
    Mat result = Mat::zeros(image.size(), CV_8UC3);
    Mat tempImage = image.clone();

    float errorRatio[2][3] = {
        {0, 0, 7 / 16},
        {3 / 16, 5 / 16, 1 / 16},
    };

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            Vec3b color = tempImage.at<Vec3b>(y, x);
            Vec3b newColor = getNearColor(newPalette, color);
            vector<int> error = getError(color, newColor);

            for (int j = 0; j < 2; ++j) {
                for (int i = 0; i <= 2; ++i) {
                    int neighbourY = y + j;
                    int neighbourX = x + i - 1;
                    if (neighbourY >= 0 && neighbourY < image.rows && neighbourX >= 0 && neighbourX < image.cols) {

                        Vec3b neighbourColor = tempImage.at<Vec3b>(neighbourY, neighbourX);
                        for (int c = 0; c < 3; c++) {
                            tempImage.at<Vec3b>(neighbourY, neighbourX)[c] = saturate_cast<uchar>(neighbourColor[c] + error[c] * errorRatio[j][i]);
                        }

                    }
                }
            }


            result.at<Vec3b>(y, x) = newColor;
        }
    }

    return result;
}


int main() {

    vector<string> pictures = { "football", "dog" };

    for (int j = 0; j < 2; ++j) {
        Mat image = loadImage(pictures[j] + ".jpg");

        Mat smallImage;
        resize(image, smallImage, Size(100, 100));

        vector<Vec3b> imagePalette = getPaletteImage(smallImage);

        for (int n = 1; n <= 4; n++) {
            int k = pow(2, n);

            vector<Vec3b> newPalette = kMeans(imagePalette, k);

            Mat newImage = FloydSteinberg(image, newPalette);

            String name = pictures[j] + to_string(n) + "bpp.jpg";

            saveImage(&newImage, name);
        }
    }

    waitKey(0);

    return 0;
}

