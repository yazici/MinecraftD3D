#include "OBJLoader.h"

OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

bool OBJLoader::LoadModel(const char* ModelFilename, Model& model)
{
	ifstream fin;
	char linebuffer[256];

	// Current Model Data
	vector<D3DXVECTOR3> vertexData;
	vector<D3DXVECTOR2> textureData;
	vector<D3DXVECTOR3> normalData;
	vector<D3DXVECTOR3> faceData;

	// Temporary file reading
	D3DXVECTOR3 tempVertex;
	D3DXVECTOR2 tempTextureUV;
	D3DXVECTOR3 tempFace[3];
	std::string textureFilename;

	float vertexID, textureID, normalID;
	int indexCount, vertexCount;

	//===================
	// Load the OBJ File
	//===================

	// Open the file
	fin.open(MODEL_DIR + ModelFilename);

	// Check if file couldnt be opened
	if (fin.fail()) { return false; }

	// Loop through the file lines 
	while (!fin.eof())
	{
		// Get the current line
		fin.getline(linebuffer, 256);

		if (string(linebuffer).find("v ") == 0) 
		{
			//Read the tempVertex line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertexData.push_back(tempVertex);
		}
		else if (string(linebuffer).find("vt ") == 0) 
		{
			//Read the texture coord line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vt %f %f", &tempTextureUV.x, &tempTextureUV.y);
			textureData.push_back(tempTextureUV);
		}
		else if (string(linebuffer).find("vn ") == 0) 
		{
			//Read the normal line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vn %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			normalData.push_back(tempVertex);
		}
		else if (string(linebuffer).find("f ") == 0) 
		{
			//Read the face line
			sscanf_s(string(linebuffer).c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f",
				&tempFace[0].x, &tempFace[0].y, &tempFace[0].z,
				&tempFace[1].x, &tempFace[1].y, &tempFace[1].z,
				&tempFace[2].x, &tempFace[2].y, &tempFace[2].z);

			// Add the faceData into the vector
			faceData.push_back(tempFace[0]);
			faceData.push_back(tempFace[1]);
			faceData.push_back(tempFace[2]);
		}
		else if (string(linebuffer).find("usemtl ") == 0) 
		{
			// Check if we have face data to process
			if (faceData.size() > 0)
			{
				//====================
				// Construct the Mesh
				//====================

				VertexData* ObjMesh;
				unsigned long* Indices;

				// Store how many indices and vertices we will have
				indexCount = vertexCount = faceData.size();

				// Construct data storage
				ObjMesh = new VertexData[vertexCount];
				if (!ObjMesh) { return false; }

				Indices = new unsigned long[indexCount];
				if (!Indices) { return false; }

				// Loop through face data
				for (unsigned int Index = 0; Index < faceData.size(); Index++)
				{
					// Read in Face Data
					vertexID = faceData[Index].x - 1;
					textureID = faceData[Index].y - 1;
					normalID = faceData[Index].z - 1;

					// Grab data from the vectors and apply them to the model
					ObjMesh[Index].position.x = vertexData[vertexID].x;
					ObjMesh[Index].position.y = vertexData[vertexID].y;
					ObjMesh[Index].position.z = vertexData[vertexID].z * -1; //Flip for RH WindowManager

					ObjMesh[Index].texture.x = textureData[textureID].x;
					ObjMesh[Index].texture.y = 1 - textureData[textureID].y; //Flip for RH WindowManager

					ObjMesh[Index].normal.x = normalData[normalID].x;
					ObjMesh[Index].normal.y = normalData[normalID].y;
					ObjMesh[Index].normal.z = normalData[normalID].z * -1; //Flip for RH WindowManager

					Indices[Index] = Index;
				}

				//==============
				// Create Final
				//==============

				// Create Mesh
				Mesh3D* newMesh = new Mesh3D;
				newMesh->SetIndexCount(indexCount);
				newMesh->SetVertexCount(vertexCount);
				newMesh->SetMesh(ObjMesh, Indices);
				Result_ = newMesh->Build();
				if (!Result_)
				{
					return false;
				}

				// Create Material
				Material* newMaterial = new Material;
				Result_ = newMaterial->SetTexture("BaseTexture", textureFilename);
				if (!Result_)
				{
					return false;
				}

				// Pass to model
				model.AddMesh(newMesh);
				model.AddMaterial(newMaterial);

				// Clear Data
				vertexData.clear();
				textureData.clear();
				normalData.clear();
				faceData.clear();
			}	

			// Read in the texture name
			sscanf_s(string(linebuffer).c_str(), "usemtl %s", &textureFilename);

			// Compile and store new texture for faces
			textureFilename = TEXTURE_DIR + textureFilename + ".dds";
		}
	}

	// Close the file
	fin.close();

	return true;
}