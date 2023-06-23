// opencv_cpp_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

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


void mergeSort(int arr[], int p, int r) {
    std::cout << "\nArray: ";
    for (int i = p; i <= r; i++) {
        std::cout << " " << arr[i];
    }
    if (p < r) {
        //get the mid value
        int q = p + (r - p) / 2;
        //split array then recurse
        std::cout << "\nSplitting Array...";
        mergeSort(arr, p, q);
        std::cout << "\n";
        mergeSort(arr, q + 1, r);
        //merge two array
        merge(arr, p, q, r);
        std::cout << "\nMerge: \n";
        for (int i = p; i <= r; i++) {
            std::cout << " " << arr[i];
        }
    }
    else {
        std::cout << "\n Cannot be splitted further..\n";
    }
}

void medianFilter(cv::Mat image, int windowSize) {
    cv::Mat paddedImage;
    //set padding size
    int paddingSize = windowSize - (1 + (windowSize - 1) * 0.5);
    cv::copyMakeBorder(image, paddedImage, paddingSize, paddingSize, paddingSize, paddingSize, cv::BORDER_CONSTANT, 0);
    cv::imshow("Filtered Image", paddedImage);
    int center_start = (windowSize - 1) / 2;

    //for each pixels
    for (int i = 0; i < paddedImage.size().height - windowSize; i++) {
        for (int j = 0; j < paddedImage.size().width - windowSize; j++) {

            std::cout << "\n\n[Pixel location : " << j + paddingSize << " " << i + paddingSize << "]\n";

            int w = 0;
            int* window = new int[pow(windowSize, 2)];

            //iterate window
            for (int k = 0; k < windowSize; k++) {
                for (int l = 0; l < windowSize; l++) {
                    window[w] = paddedImage.at<cv::Vec3b>(cv::Point(l + j, k + i))[0];
                    w++;
                }
            }
            mergeSort(window, 0, pow(windowSize, 2) - 1);

            int median_value = window[(int)ceil((pow(windowSize, 2) - 1) / 2)];

            paddedImage.at<cv::Vec3b>(cv::Point(j + paddingSize, i + paddingSize))[0] = median_value;
            paddedImage.at<cv::Vec3b>(cv::Point(j + paddingSize, i + paddingSize))[1] = median_value;
            paddedImage.at<cv::Vec3b>(cv::Point(j + paddingSize, i + paddingSize))[2] = median_value;

        }
    }

    cv::imshow("Filtered Image", paddedImage);
    cv::waitKey(0);


}



int main(int argc, char* argv[])
{
    std::string imagePath = "/Users/roel/Documents/fruit_noise_debug.jpg";
    std::string imageOutputPath;
    int windowSize;

    for (int i = 0; i < argc; i++) {
        if (argv[i] == "--input" || argv[i] == "-i") {
            imagePath = argv[i + 1];
        }
        else if (argv[i] == "--output" || argv[i] == "-o") {
            imageOutputPath = argv[i + 1];
        }
        else if (argv[i] == "--window" || argv[i] == "-w") {
            windowSize = (int)argv[i + 1];
        }
    }
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    medianFilter(image, 3);

    cv::imshow("Image", image);
    cv::waitKey(0);

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
