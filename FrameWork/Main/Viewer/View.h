#pragma once

class View
{
private:
	Vector3 eye, look, up;
	Matrix matrix;
public:
	View(Vector3 eye, Vector3 look, Vector3 up);
	~View();
	void GetMatrix(Matrix* matrix);
	void Set(Vector3 eye, Vector3 look, Vector3 up);
	void Set(Matrix* mat);
};