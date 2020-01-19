//
// (c) 2017-2018 Takahiro Hashimoto
//
#pragma once

#include <Eigen/Dense>
#include <Eigen/StdVector>

// #define Dynamic Eigen::Dynamic

typedef	Eigen::Vector2d		Vec2;	// (x, y) value
typedef Eigen::Vector2cd    Vec2c;
typedef	Eigen::Vector2i		Idx2;	// (x, y) location/idx
typedef	Eigen::Vector3d		Vec3;	// (x, y, z) value
typedef Eigen::Vector3cd    Vec3c;
typedef	Eigen::Vector3i		Idx3;	// (x, y, z) location/idx
typedef Eigen::Matrix<bool, 3, 1> Bool3;
typedef	Eigen::Vector4d		Vec4;
typedef	Eigen::Vector4i		Idx4;
typedef	Eigen::VectorXd		Vec;
typedef Eigen::Matrix<double, 6, 1> Vec6; //for (angle-axis, translation)
typedef Eigen::Matrix<double, 2, 3> Mat23;

typedef	Eigen::Matrix2d		Mat2;
typedef	Eigen::Matrix3d		Mat3;
typedef	Eigen::Matrix4d		Mat4;
typedef	Eigen::MatrixXd		Mat;
typedef	Eigen::MatrixXi		Mati;

typedef Eigen::Matrix2cd	Mat2c;
typedef Eigen::Matrix<Vec2c, Eigen::Dynamic, Eigen::Dynamic> MatXc2;

typedef	std::vector<int>									VecIdx;
typedef	std::vector<VecIdx>									VecVecIdx;
typedef std::vector<bool>									VecBool;

typedef	std::vector<Vec2, Eigen::aligned_allocator<Vec2>>	VecVec2;
typedef	std::vector<Idx2, Eigen::aligned_allocator<Idx2>>	VecIdx2;
typedef	std::vector<Vec3, Eigen::aligned_allocator<Vec3>>	VecVec3;
typedef	std::vector<Idx3, Eigen::aligned_allocator<Idx3>>	VecIdx3;
typedef	std::vector<Vec4, Eigen::aligned_allocator<Vec4>>	VecVec4;
typedef	std::vector<Idx4, Eigen::aligned_allocator<Idx4>>	VecIdx4;

typedef std::vector<Mat2, Eigen::aligned_allocator<Mat2>>   VecMat2;
typedef std::vector<Mat2c, Eigen::aligned_allocator<Mat2c>> VecMat2c;

typedef std::vector<std::complex<double>>    Vecc;
typedef std::vector<double>    Vecd;