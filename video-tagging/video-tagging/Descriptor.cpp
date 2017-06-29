#include "stdafx.h"
#include "Descriptor.h"
#include "Manage.h"
using namespace dlib;
using namespace std;

Descriptor::Descriptor()
{
	detector = get_frontal_face_detector();

	string path = Manage::get_current();

	deserialize(path + "shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize(path + "dlib_face_recognition_resnet_model_v1.dat") >> net;
}

std::vector<matrix<float, 0, 1>> Descriptor::get_description(cv::Mat& frame)
{
	std::vector<matrix<rgb_pixel>> faces;
	cv_image<rgb_pixel> cimg(frame);

	for (auto face : detector(cimg))
	{
		auto shape = sp(cimg, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));
	}
	return net(faces);
}
