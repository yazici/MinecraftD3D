#include "Utilities.h"
#include "Camera.h"
#include "DirectXManager.h"
#include "WindowManager.h"

float Distance(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	float x, y, z;

	x = b.x - a.x;
	y = b.y - a.y;
	z = b.z - a.z;

	return sqrt(x*x + y*y + z*z);
}

D3DXVECTOR3 Normalise(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	float length = Distance(a, b);

	D3DXVECTOR3 Difference = b - a;

	float x = Difference.x / length;
	float y = Difference.y / length;
	float z = Difference.z / length;

	return D3DXVECTOR3(x, y, z);
}

void Round(float& number, int decimalplaces)
{
	int ten = pow(10, decimalplaces);

	number = roundf(number * ten) / ten;
}

std::string ToStr(float number)
{
	std::stringstream sstr;
	sstr << number;
	return sstr.str();
}

bool WindowActive()
{
	return GetActiveWindow() == GetForegroundWindow();
}

void LockMouseToCenter()
{
	int middleX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int middleY = GetSystemMetrics(SM_CYSCREEN) / 2;

	SetCursorPos(middleX, middleY);
}

void OutputToDebug(std::string text)
{
	int len;
	int slength = (int)text.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, text.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	OutputDebugString(r.c_str());
	OutputDebugString(L"\n");
}

void Wrap(int& value, int min, int max)
{
	int difference;

	if (value < min)
	{
		difference = min - abs(value);
		value = max - difference;
	}
	else if (value > max)
	{
		difference = value - max;
		value = min + difference;
	}
}

void Wrap(float& value, float min, float max)
{
	float difference;

	if (value < min)
	{
		difference = min - abs(value);
		value = max - difference;
	}
	else if (value > max)
	{
		difference = value - max;
		value = min + difference;
	}
}

void Clamp(int& value, int min, int max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}
void Clamp(float& value, float min, float max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}

void OutputErrorMessage(string message, char* data)
{
	message += data;

	int len;
	int slength = (int)message.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, message.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, message.c_str(), slength, buf, len);
	std::wstring result(buf);
	delete[] buf;

	MessageBox(NULL, result.c_str(), L"ERROR", MB_OK);
}

D3DXVECTOR3 RotateAroundPoint(D3DXVECTOR3 point, D3DXVECTOR3 center, float angle)
{
	float rotatedX = cos(angle) * (point.x - center.x) - sin(angle) * (point.y - center.y) + center.x;
	float rotatedY = sin(angle) * (point.x - center.x) + cos(angle) * (point.y - center.y) + center.y;
	
	return D3DXVECTOR3(rotatedX, rotatedY, point.z);
}

float AngleBetweenPoints(Vector2 A, Vector2 B)
{
	float deltaX = B.x - A.x;
	float deltaY = B.y - A.y;

	return D3DXToDegree(atan2(deltaY, deltaX));
}

bool RangeCheck(int value, int min, int max)
{
	return value >= min && value <= max;
}

bool RangeCheck(float value, float min, float max)
{
	return value >= min && value <= max;
}

bool CheckCollision(BoundingBox box, float position)
{
	bool insideX = RangeCheck(position, box.left, box.right);
	bool insideY = RangeCheck(position, box.bottom, box.top);
	bool insideZ = RangeCheck(position, box.front, box.back);

	return insideX && insideY && insideZ;
}

bool CheckCollision(BoundingBox box, D3DXVECTOR2 position)
{
	bool insideX = RangeCheck(position.x, box.left, box.right);
	bool insideY = RangeCheck(position.y, box.bottom, box.top);

	return insideX && insideY;
}

bool CheckCollision(BoundingBox box, D3DXVECTOR3 position)
{
	bool insideX = RangeCheck(position.x, box.left, box.right);
	bool insideY = RangeCheck(position.y, box.bottom, box.top);
	bool insideZ = RangeCheck(position.z, box.front, box.back);

	return insideX && insideY && insideZ;
}

D3DXVECTOR2 ConvertToScreenSpace(D3DXVECTOR3 pos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, Rect2D WindowSize)
{
	// Transform the coordinates using the view and projection matrix
	D3DXVec3TransformCoord(&pos, &pos, &viewMatrix);
	D3DXVec3TransformCoord(&pos, &pos, &projectionMatrix);

	// Convert the coordinates into screen space
	pos.x = WindowSize.width * (pos.x + 1.0) / 2.0;
	pos.y = WindowSize.height * (1.0 - ((pos.y + 1.0) / 2.0));

	return D3DXVECTOR2(pos.x, pos.y);
}

bool CheckScreenSpace(D3DXVECTOR3 pos)
{
	// Get View Matrices
	D3DXMATRIX viewMatrix, projectionMatrix;
	Camera::Instance()->GetViewMatrix(viewMatrix);
	DirectXManager::Instance()->GetProjectionMatrix(projectionMatrix);

	// Get size of window
	Rect2D windowSize = WindowManager::Instance()->GetWindowResolution();

	// Convert the 3D position to screen space
	D3DXVECTOR2 ConvertedPos = ConvertToScreenSpace(pos, viewMatrix, projectionMatrix, windowSize);
	float halfWidth = windowSize.width;
	float halfHeight = windowSize.height;

	// Check if its within the limits of screen
	bool xCheck = RangeCheck(ConvertedPos.x, -halfWidth, halfWidth);
	bool yCheck = RangeCheck(ConvertedPos.y, -halfHeight, halfHeight);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMATRIX frustrumMatrix;

	D3DXMatrixMultiply(&frustrumMatrix, &viewMatrix, &projectionMatrix);

	// Create a plane representing the screen
	D3DXPLANE viewPlane;
	viewPlane.a = frustrumMatrix._14 + frustrumMatrix._13;
	viewPlane.b = frustrumMatrix._24 + frustrumMatrix._23;
	viewPlane.c = frustrumMatrix._34 + frustrumMatrix._33;
	viewPlane.d = frustrumMatrix._44 + frustrumMatrix._43;
	D3DXPlaneNormalize(&viewPlane, &viewPlane);

	// Return whether its within the bounds of the screen and is on the screen
	return xCheck && yCheck && D3DXPlaneDotCoord(&viewPlane, &pos) >= 0.0f;
}

bool CheckScreenSpace(D3DXVECTOR3 position, BoundingBox box)
{
	// Create the positions in world space
	D3DXVECTOR3 leftPos = position + D3DXVECTOR3(box.left, 0, 0);
	D3DXVECTOR3 rightPos = position + D3DXVECTOR3(box.right, 0, 0);
	D3DXVECTOR3 upPos = position + D3DXVECTOR3(0, box.top, 0);
	D3DXVECTOR3 downPos = position + D3DXVECTOR3(0, box.bottom, 0);
	D3DXVECTOR3 frontPos = position + D3DXVECTOR3(0, 0, box.front);
	D3DXVECTOR3 backPos = position + D3DXVECTOR3(0, 0, box.back);

	// Check each position against the camera
	bool Left = CheckScreenSpace(leftPos);
	bool Right = CheckScreenSpace(rightPos);
	bool Up = CheckScreenSpace(upPos);
	bool Down = CheckScreenSpace(downPos);
	bool Front = CheckScreenSpace(frontPos);
	bool Back = CheckScreenSpace(backPos);
	bool Center = CheckScreenSpace(position);

	return Left || Right || Up || Down || Front || Back || Center;
}

void TransposeMatrix(MatrixBuffer& matrix)
{
	// Prepare matrices for the shader
	D3DXMatrixTranspose(&matrix.world, &matrix.world);
	D3DXMatrixTranspose(&matrix.view, &matrix.view);
	D3DXMatrixTranspose(&matrix.projection, &matrix.projection);
	D3DXMatrixTranspose(&matrix.reflection, &matrix.reflection);
}

void CalculateHardNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal)
{
	float length;

	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}

void CalculateTangentBinormal(VertexData vertex1, VertexData vertex2, VertexData vertex3, D3DXVECTOR3& tangent, D3DXVECTOR3& binormal)
{
	D3DXVECTOR3 vector[2];
	D3DXVECTOR2 texture[2];
	float den;
	float length;

	// Calculate the two vectors for this face
	vector[0] = vertex2.position - vertex1.position;
	vector[1] = vertex3.position - vertex1.position;

	// Calculate the tu and tv texture vectors
	texture[0].x = vertex2.texture.x - vertex1.texture.x;
	texture[0].y = vertex2.texture.y - vertex1.texture.y;

	texture[1].x = vertex3.texture.x - vertex1.texture.x;
	texture[1].y = vertex3.texture.y - vertex1.texture.y;

	// Calculate the denominator of the tangent/binormal equation
	den = 1.0f / (texture[0].x * texture[1].y - texture[1].x * texture[0].y);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal
	tangent.x = (texture[1].y * vector[0].x - texture[0].y * vector[1].x) * den;
	tangent.y = (texture[1].y * vector[0].y - texture[0].y * vector[1].y) * den;
	tangent.z = (texture[1].y * vector[0].z - texture[0].y * vector[1].z) * den;

	binormal.x = (texture[0].x * vector[1].x - texture[1].x * vector[0].x) * den;
	binormal.y = (texture[0].x * vector[1].y - texture[1].x * vector[0].y) * den;
	binormal.z = (texture[0].x * vector[1].z - texture[1].x * vector[0].z) * den;

	// Calculate the length of this normal
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the tangent and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the binormal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

string GetKey(float x, float y)
{
	string key;

	// Format: X Y Z
	key = to_string(x) + " " + to_string(y);

	return key;
}

string GetKey(float x, float y, float z)
{
	string key;

	// Format: X Y Z
	key = to_string(x) + " " + to_string(y) + " " + to_string(z);

	return key;
}

int GetIndex(int i, int j, int k, int maxJ, int maxK)
{
	return (i*maxJ + j)*maxK + k;
}

void OutputTimeDelay(string dataName, float before, float after)
{
	float totalTime = after - before;
	totalTime /= 1000.0f;

	string output = to_string(totalTime);
	TrimString(output);

	OutputToDebug(dataName + " : " + output + "ms");
}

void TrimString(string& string)
{
	string.erase(string.find_last_not_of('0') + 1, std::string::npos);
	if (string[string.size() - 1] == '.')
	{
		string.erase(string.end() - 1);
	}
}

void RoundVector(D3DXVECTOR2& vector)
{
	vector.x = round(vector.x);
	vector.y = round(vector.y);
}

void RoundVector(D3DXVECTOR3& vector)
{
	vector.x = round(vector.x);
	vector.y = round(vector.y);
	vector.z = round(vector.z);
}

int GetNoise(int x, int y, int z, float scale, int max)
{
	return floor((SimplexNoise::Noise(x * scale, y * scale, z * scale) + 1.0f) * (max / 2.0f));
}

int GetNoise(int x, int y, int z, float scale, int max, float power)
{
	// Generate noise value
	float noise = (SimplexNoise::Noise(x / scale, y / scale, z / scale) + 1.0f) * (max / 2.0f);


	if (power != 1)
		noise = pow(noise, power);

	return floor(noise);
}