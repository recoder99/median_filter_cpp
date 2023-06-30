// opencv_cpp_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>



class MedianFilter {
    
    cv::Mat image;
    int windowSize;
    int paddingSize;
    cv::Mat paddedImage;

    bool enableLog = false;
    bool verbose = false;
    std::string logPath;
    std::ofstream *logFile;

public:
    //constructor when logging and verbose is not needed
    MedianFilter(cv::Mat image, int windowSize) {
        this->image = image;
        this->windowSize = windowSize;
        
        
        if (!(windowSize >= 3 && windowSize % 2 != 0)) {
            std::cout << "Invalid window size, setting default window size to 3..." << std::endl;
            windowSize = 3;
            this->windowSize = 3;
        }
        this->paddingSize = windowSize - (1 + (windowSize - 1) * 0.5);
    }

    //constructor when logging and verbose must be specified
    MedianFilter(cv::Mat image, int windowSize, bool enableLog, bool verbose, std::string path) {
        this->image = image;
        this->windowSize = windowSize;
        this->enableLog = enableLog;
        this->verbose = verbose;
        this->logPath = path;

        if (!(windowSize >= 3 && windowSize % 2 != 0)) {
            std::cout << "Invalid window size, setting default window size to 3..." << std::endl;
            windowSize = 3;
            this->windowSize = 3;
        }
        this->paddingSize = windowSize - (1 + (windowSize - 1) * 0.5);

        if (enableLog) {
            logFile = new std::ofstream(logPath);
        }
    }

    //part of mergeSort() function, use to merge splitted array
    void merge(int arr[], int p, int q, int r) {

        int i = 0;
        int j = 0;
        int k = p;

        const int n1 = q - p + 1;
        const int n2 = r - q;

        int* P = new int[n1];
        int* Q = new int[n2];

        for (int x = 0; x < n1; x++) {
            P[x] = arr[p + x];
        }

        for (int x = 0; x < n2; x++) {
            Q[x] = arr[q + x + 1];
        }

        while (i < n1 && j < n2) {
            if (P[i] <= Q[j]) {
                arr[k] = P[i];
                i++;
            }
            else {
                arr[k] = Q[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = P[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = Q[j];
            j++;
            k++;
        }

    }

    //A recursive function for sorting pixels using merge sort
    void mergeSort(int arr[], int p, int r) {
        if (enableLog) {
            *logFile << "\nArray: ";
            for (int i = p; i <= r; i++) {
                *logFile << " " << arr[i];
            }
        }
        if (p < r) {
            //get the mid value
            int q = p + (r - p) / 2;
            //split array then recurse
            if (enableLog) *logFile << "\nSplitting Array...";
            mergeSort(arr, p, q);
            if (enableLog) *logFile << "\n";
            mergeSort(arr, q + 1, r);
            //merge two array
            merge(arr, p, q, r);
            if (enableLog) {
                *logFile << "\nMerge: \n";
                for (int i = p; i <= r; i++) {
                    *logFile << " " << arr[i];
                }
            }
        }
        else {
            if (enableLog) *logFile << "\n Cannot be splitted further..\n";
        }
    }

    void startMedianFilter() {
        
        //set padding size
        cv::copyMakeBorder(image, paddedImage, paddingSize, paddingSize, paddingSize, paddingSize, cv::BORDER_CONSTANT, 0);
        int center_start = (windowSize - 1) / 2;

        //for each pixels
        for (int i = 0; i < paddedImage.size().height - windowSize; i++) {
            for (int j = 0; j < paddedImage.size().width - windowSize; j++) {

                if(verbose) std::cout << "\n\n[Pixel location : " << j + paddingSize << " " << i + paddingSize << "]\n";
                if (enableLog) *logFile << "\n\n[Pixel location : " << j + paddingSize << " " << i + paddingSize << "]\n";

                int w = 0;
                int* window = new int[pow(windowSize, 2)];

                //iterate window
                for (int k = 0; k < windowSize; k++) {
                    for (int l = 0; l < windowSize; l++) {
                        window[w] = paddedImage.at<cv::Vec3b>(cv::Point(l + j, k + i))[0];
                        w++;
                    }
                }

                //sort the pixel and get its median
                mergeSort(window, 0, pow(windowSize, 2) - 1);
                int median_value = window[(int)ceil((pow(windowSize, 2) - 1) / 2)];

                //replace the pixel with the median pixel
                paddedImage.at<cv::Vec3b>(cv::Point(j + paddingSize, i + paddingSize))[0] = median_value;
                paddedImage.at<cv::Vec3b>(cv::Point(j + paddingSize, i + paddingSize))[1] = median_value;
                paddedImage.at<cv::Vec3b>(cv::Point(j + paddingSize, i + paddingSize))[2] = median_value;

            }
        }
    }

    cv::Mat getFilteredImage() {
        cv::Mat croppedImage = paddedImage(cv::Rect(paddingSize, paddingSize, image.size().width, image.size().height));
        return croppedImage;
    }
};




int main(int argc, char* argv[])
{
    //Paths for source image and destination
    std::string imagePath;
    std::string imageOutputPath;
    std::string logPath;

    bool verbose = false;
    bool enableLog = false;
    bool preview = false;

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    //std::replace(begin(imagePath), end(imagePath), '\\', '/');
    //std::replace(begin(imageOutputPath), end(imageOutputPath), '\\', '/');

    int windowSize = 3;
    
    //arguments parser
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-i") == 0 ) {
            imagePath = argv[i + 1];
        }
        else if (strcmp(argv[i], "--output") == 0|| strcmp(argv[i], "-o") == 0) {
            imageOutputPath = argv[i + 1];
        }
        else if (strcmp(argv[i], "--window") == 0 || strcmp(argv[i], "-w")  == 0) {
            windowSize = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--enable-preview") == 0) {
            preview = true;
        }
        else  if (strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }
        else  if (strcmp(argv[i], "--log") == 0) {
            enableLog = true;
            logPath = argv[i + 1];
        }
    }

    try {
        image = cv::imread(imagePath, cv::IMREAD_COLOR);
    }
    catch (cv::Exception& e) {
        std::cout << e.msg << std::endl;
        return 0;
    }

    MedianFilter* filteredImage = new MedianFilter(image, windowSize, enableLog, verbose, logPath);

    filteredImage->startMedianFilter();

    if (preview) cv::imshow("Image", filteredImage->getFilteredImage()); cv::waitKey(0);


    try {
        cv::imwrite(imageOutputPath, filteredImage->getFilteredImage());
    }
    catch (cv::Exception& e) {
        std::cout << e.msg << std::endl;
        return 0;
    }
    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
