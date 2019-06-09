#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#define NUM_CHANNELS 3

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: ./fuzzy_color path_to_image" << std::endl;
		return 1;
	}

	// input image
	cv::Mat input = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    // output image
    cv::Mat output = cv::Mat(input.rows, input.cols, CV_8UC3);

    unsigned max_R, min_R;
    unsigned max_G, min_G;
    unsigned max_B, min_B;
    unsigned R, G, B;
    
    max_R = min_R = input.at<cv::Vec3b>(0, 0)[2];
    max_G = min_G = input.at<cv::Vec3b>(0, 0)[1];
    max_B = min_B = input.at<cv::Vec3b>(0, 0)[0];
        
    for (int i = 1; i < input.rows; i++) {
        for (int j = 1; j <  input.cols; ++j) {
            R = input.at<cv::Vec3b>(i, j)[2];
            G = input.at<cv::Vec3b>(i, j)[1];
            B = input.at<cv::Vec3b>(i, j)[0];

            if (max_R < R) max_R = R;
            else if (min_R > R) min_R = R;
        
            if (max_G < G) max_G = G;
            else if (min_G > G) min_G = G;

            if (max_B < B) max_B = B;
            else if (min_B > B) min_B = B;
            }
    }
 
    std::vector<unsigned> max_RGB {max_R, max_G, max_B};
    std::vector<double> mid_RGB {(max_R - min_R) / 2.0, (max_G - min_G) / 2.0, (max_B - min_B) / 2.0};
    
    std::vector<unsigned> RGB;
    RGB.reserve(3);
    std::vector<unsigned> curr_RGB;
    curr_RGB.reserve(3);

    double Fe = 2.0;
    double Fd;

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; ++j) {
            curr_RGB.resize(0);
            curr_RGB.push_back(input.at<cv::Vec3b>(i, j)[2]); 
            curr_RGB.push_back(input.at<cv::Vec3b>(i, j)[1]);
            curr_RGB.push_back(input.at<cv::Vec3b>(i, j)[0]);
            RGB.resize(0);

            for (int c = 0; c < NUM_CHANNELS; ++c) {
                Fd = (max_RGB[c] - mid_RGB[c]) / 0.41421356237309515;
                // fuzzification
                double mem_func = pow(1 + (max_RGB[c] - curr_RGB[c]) / Fd, -Fe);
                
                // modification of membership function by INT operator
                if (mem_func <= 0.5 and mem_func > 0) {
                    mem_func = 2 * pow(mem_func, 2);
                }
                else if (mem_func <= 1 and mem_func > 0.5) {
                    mem_func = 1 - 2 * pow(1 - mem_func, 2);
                }

                // defuzzification
                RGB.push_back(max_RGB[c] - Fd * (pow(mem_func, -1 / Fe)) + Fd);
            }
            
            cv::Vec3f BGR(RGB[2], RGB[1], RGB[0]);
            output.at<cv::Vec3b>(i, j) = BGR;
        }
    }

    // Show and save
	namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	imshow("Display window", output);
	cv::waitKey(0);
	imwrite("fuzzy_color.jpg", output);

    return 0;
}
