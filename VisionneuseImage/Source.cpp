#include <iostream>
#include <windows.h>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const string baseURL = "Images/raw";
const int nbImages = 14;

const int largeur = 640;
const int hauteur = 480;

const int tailleImageBandeau = largeur / nbImages;

vector<Mat> loadImages(const string& path, const int nbImages)
{
	vector<Mat> images;
	for (int i = 0; i < nbImages; i++)
	{
		string filename = path + to_string(i+1) + ".jpg";
		Mat image = imread(filename);
		images.push_back(image);
	}
	return images;
}

vector<Mat> getImagesResized(const vector<Mat>& imagesToResize) {
	vector<Mat> images;
	for (Mat image : imagesToResize) {
		double ratio = 0.0;
		if (image.cols > image.rows) {
			ratio = (double)largeur / image.cols;
		}
		else {
			ratio = (double)hauteur / image.rows;
		}
		Mat imageResize;
		resize(image, imageResize, Size(), ratio / 1.25, ratio / 1.25, INTER_LINEAR);
		images.push_back(imageResize);
	}
	return images;
}

Mat createBandeau(const vector<Mat>& imagesForBandeau) {
	int taille = tailleImageBandeau;
	if (taille > 100) taille = 100;

	Mat bandeau;
	for (int i = 0; i < imagesForBandeau.size(); ++i) {
		Mat tmp = imagesForBandeau[i].clone();
		
		resize(tmp, tmp, Size(100, 50));
		
		Mat bande = tmp(Rect(0, 0, taille, tmp.rows));
		
		if (i == 0)
			bandeau = bande;
		else
			hconcat(bandeau, bande, bandeau);
	}
	
	return bandeau;
}

Mat createImage(Mat& bandeau) {
	Mat image = Mat::zeros(hauteur, largeur, CV_8UC3);
	image.setTo(Scalar(0, 0, 0));
	bandeau.copyTo(image(Rect(0, image.rows - bandeau.rows, bandeau.cols, bandeau.rows)));
	return image;
}

int getImageFromCursorPos(const int x, const int y, const vector<Mat>& images) {
	int taille = tailleImageBandeau;
	if (taille > 100) taille = 100;
	int index = x / taille;
	
	if (index >= 0 && index < images.size() && y >= (hauteur - 50)) {
		return index;
	}
	return -1;
}

int main(int argc, char** argv)
{
	Mat image, cursor;
	VideoCapture cap;
	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);
	cap.open(0);
	
	auto images = loadImages(baseURL, nbImages);

	auto imagesResized = getImagesResized(images);
	
	Mat bandeau = createBandeau(images);

	while (1) {
		try {
			cap >> image;
			cap >> cursor;

			bandeau.copyTo(image(Rect(0, image.rows - bandeau.rows, bandeau.cols, bandeau.rows)));


			Mat hsv;
			cvtColor(cursor, hsv, COLOR_BGR2HSV);

			Mat1b masqueVert;
			inRange(hsv, Scalar(40, 40, 40), Scalar(70, 255, 255), masqueVert);

			vector<vector<Point>> contours;
			findContours(masqueVert, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			if (contours.size() > 0) {
				int max = 0;
				for (int i = 1; i < contours.size(); i++) {
					if (contourArea(contours[i]) > contourArea(contours[max])) {
						max = i;
					}
				}

				Moments m = moments(contours[max]);
				Point centre(m.m10 / m.m00, m.m01 / m.m00);

				circle(image, centre, 5, Scalar(0, 0, 255), -1);
				circle(masqueVert, centre, 5, Scalar(0, 0, 255), -1);

				int index = getImageFromCursorPos(centre.x, centre.y, images);

				if (index != -1) {
					Mat imageToLoad = imagesResized[index];
					rectangle(image, Rect(index * tailleImageBandeau, hauteur - 50, tailleImageBandeau, 50), Scalar(0, 255, 255), 2);
					imageToLoad.copyTo(image(Rect(50, 25, imageToLoad.cols, imageToLoad.rows)));
				}
			}
			
			imshow("Visionneuse", image);
			
			waitKey(1);
		}
		catch (Exception& e) {
			const char* err_message = e.what();
			cout << err_message << endl;
		}
	}

	return 0;
}