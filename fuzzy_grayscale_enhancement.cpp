#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: ./fuzzy_grayscale path_to_image" << std::endl;
		return 1;
	}

	// input image
	cv::Mat input = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    // output image
    cv::Mat output = cv::Mat(input.rows, input.cols, CV_8UC1, cv::Scalar(255));
    
    unsigned min_gray = *std::min_element(input.begin<unsigned char>(), input.end<unsigned char>());
    unsigned max_gray = *std::max_element(input.begin<unsigned char>(), input.end<unsigned char>());
    unsigned diff = max_gray - min_gray;

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j <  input.cols; ++j) {
            // fuzzification
            double mem_func = (input.at<unsigned char>(i,j) - min_gray) / double(diff);
            
            // modification of membership function by INT operator
            if (mem_func <= 0.5 and mem_func > 0) {
                mem_func = 2 * pow(mem_func, 2);
            }
            else if (mem_func <= 1 and mem_func > 0.5) {
                mem_func = 1 - 2 * pow(1 - mem_func, 2);
            }

            // defuzzification
            output.at<unsigned char>(i,j) = min_gray + mem_func * diff; 
        }
    }

    // Show and save
	namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	imshow("Display window", output);
	cv::waitKey(0);
	imwrite("fuzzy_grayscale.jpg", output);

    return 0;
}
