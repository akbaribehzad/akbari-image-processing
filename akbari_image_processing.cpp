#include "Image.h"
#include "PrintOnScreen.h"
#include <thread>
#include <iostream>
#include <chrono>

// Worker function for threading
void worker(Image* img, Color c, int x1, int y1, int x2, int y2)
{
	img->SetColorPartial(c, x1, y1, x2, y2);
}

int main()
{
	// For testing purposes I have chosen one large file.
	// At first I the color of the entire file is changed and
	// the elapsed time is calculated and printed on the screen.

	// Then the image is divided into 4 slices and each slice is fed to
	// a single thread. The time elaped for executing all four threads
	// is computed and printed on the screen.

	// As it is noted, the execution time difference between the two
	// methods is tremendous.

	// Coordinates of the image
	int x_co1[4] = { 0,0,0,0 };
	int y_co1[4] = { 0,0,0,0 };

	int x_co2[4] = { 0,0,0,0 };
	int y_co2[4] = { 0,0,0,0 };

	//3840 - 2160

	//Slice 1 Coordinates
	x_co1[0] = 0; y_co1[0] = 0;
	x_co2[0] = 959; y_co2[0] = 2159;

	//Slice 2 Coordinates
	x_co1[1] = 960; y_co1[1] = 0;
	x_co2[1] = 1919; y_co2[1] = 2159;

	//Slice 3 Coordinates
	x_co1[2] = 1920; y_co1[2] = 0;
	x_co2[2] = 2879; y_co2[2] = 2159;

	//Slice 4 Coordinates
	x_co1[3] = 2880; y_co1[3] = 0;
	x_co2[3] = 3840; y_co2[3] = 2159;

	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	/////////////////////
	// This section is used to create a bitmap file for testing purposes
	//const int width = 600;
	//const int height = 400;
	//Image MyImage(width, height);

	//for (int y = 0; y < height; y++)
	//{
	//    for (int x = 0; x < width; x++)
	//    {
	//        MyImage.SetColor(Color((float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height), x, y);
	//    }
	//}

	//MyImage.Export("Large.bmp");
	/////////////////////

	Image ReadImage(0, 0);

	ReadImage.Read("3840-2160-sample.bmp");

	// Changing the color of the entire file

	auto w1 = high_resolution_clock::now();
	int x1 = 0, y1 = 0, x2 = 3479, y2 = 2159;
	ReadImage.SetColorPartial(Color(110, 20, 229), x1, y1, x2, y2);
	auto w2 = high_resolution_clock::now();
	auto ms_int_w = duration_cast<milliseconds>(w2 - w1);
	duration<double, std::milli> ms_double_w = (w2 - w1);

	std::cout << "--------- Execution time for main thread ---\n";
	std::cout << ms_int_w.count() << "ms\n";
	std::cout << ms_double_w.count() << "ms\n";
	std::cout << "--------------------------------------------\n";

	ReadImage.Export("3840-2160-sample_copy.bmp");

	// Changing image color using Multithreading

	ReadImage.Read("3840-2160-sample.bmp");

	auto t1 = high_resolution_clock::now();

	std::thread th1(worker, &ReadImage, Color(0, 102, 204), x_co1[0], y_co1[0], x_co2[0], y_co2[0]);
	std::thread th2(worker, &ReadImage, Color(0, 30, 210), x_co1[1], y_co1[1], x_co2[1], y_co2[1]);
	std::thread th3(worker, &ReadImage, Color(50, 100, 250), x_co1[2], y_co1[2], x_co2[2], y_co2[2]);
	std::thread th4(worker, &ReadImage, Color(110, 20, 229), x_co1[3], y_co1[3], x_co2[3], y_co2[3]);

	th1.join();
	th2.join();
	th3.join();
	th4.join();

	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double1 = (t2 - t1);

	std::cout << "\n\n";
	std::cout << "--------- Execution time for Multi-threading ---\n";
	std::cout << ms_double1.count() << "ms\n";
	std::cout << "--------------------------------------------\n";

	ReadImage.Export("3840-2160-sample_copy.bmp");
    

	// The display works fine for small images but it is not 
	// suitable for large files.

	//display("3840-2160-sample_copy.bmp");

	return 0;
}
