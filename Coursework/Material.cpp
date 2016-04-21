#include "Material.h"
#include "AssetManager.h"

Material::Material()
{
	// Initialise pointers
	BaseTexture_ = 0;
	NormalTexture_ = 0;
	AlphaTexture_ = 0;
	NoiseTexture_ = 0;
	DistortionTexture_ = 0;
	PerturbTexture_ = 0;

	// Initialise colours
	Tint_ = Config::Colour::White;
	SpecularAmount_ = 32.0f;

	// Add Default Data
	SetVector4("BaseColour", D3DXVECTOR4(1,1,1,1));
}

Material::Material(const Material& material)
{
	*this = material; // copy the object
}

Material::~Material()
{

}

// Shutdown
void Material::Shutdown()
{
	
}

// Setters
void Material::SetColour(D3DXVECTOR4 tint)
{
	Tint_ = tint;
}

void Material::SetSpecular(float amount)
{
	SpecularAmount_ = amount;
}

bool Material::SetBaseTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&BaseTexture_, textureFilename);

	if (!BaseTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetNormalTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&NormalTexture_, textureFilename);

	if (!NormalTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetAlphaTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&AlphaTexture_, textureFilename);

	if (!AlphaTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetNoiseTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&NoiseTexture_, textureFilename);

	if (!NoiseTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetDistortionTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&DistortionTexture_, textureFilename);

	if (!DistortionTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetPerturbTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&PerturbTexture_, textureFilename);

	if (!PerturbTexture_)
	{
		return false;
	}

	return true;
}

// Getters
D3DXVECTOR4 Material::GetTint()
{
	return Tint_;
}

float Material::GetSpecular()
{
	return SpecularAmount_;
}

ID3D11ShaderResourceView* Material::GetBaseTexture()
{
	if (!BaseTexture_)
		return 0;

	return BaseTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetNormalTexture()
{
	if (!NormalTexture_)
		return 0;

	return NormalTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetAlphaTexture()
{
	if (!AlphaTexture_)
		return 0;

	return AlphaTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetNoiseTexture()
{
	if (!NoiseTexture_)
		return 0;

	return NoiseTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetDistortionTexture()
{
	if (!DistortionTexture_)
		return 0;

	return DistortionTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetPerturbTexture()
{
	if (!PerturbTexture_)
		return 0;

	return PerturbTexture_->GetTexture();
}

//========================================================================
// NEW STUFF
//========================================================================

// Setters
void Material::SetFloat(string keyName, float value)
{
	FloatDatabase_[keyName] = value;
}

bool Material::SetTexture(string keyName, string value)
{
	if (TextureDatabase_.count(keyName))
	{
		return true;
	}

	Texture* newTexture = 0;

	// Load Texture
	AssetManager::Instance()->LoadTexture(&newTexture, value);

	if (!newTexture)
	{
		return false;
	}

	TextureDatabase_[keyName] = newTexture;

	return true;
}

void Material::SetTexture(string keyName, Texture* value)
{
	TextureDatabase_[keyName] = value;
}

void Material::SetVector2(string keyName, D3DXVECTOR2 value)
{
	Vector2Database_[keyName] = value;
}

void Material::SetVector3(string keyName, D3DXVECTOR3 value)
{
	Vector3Database_[keyName] = value;
}

void Material::SetVector4(string keyName, D3DXVECTOR4 value)
{
	Vector4Database_[keyName] = value;
}

// Getters
float Material::GetFloat(string keyName)
{
	if (FloatDatabase_.count(keyName))
	{
		return FloatDatabase_[keyName];
	}

	return 0;
}

Texture* Material::GetTexture(string keyName)
{
	if (TextureDatabase_.count(keyName))
	{
		return TextureDatabase_[keyName];
	}

	return 0;
}

D3DXVECTOR2 Material::GetVector2(string keyName)
{
	if (Vector2Database_.count(keyName))
	{
		return Vector2Database_[keyName];
	}

	return D3DXVECTOR2(0.0f, 0.0f);
}

D3DXVECTOR3 Material::GetVector3(string keyName)
{
	if (Vector3Database_.count(keyName))
	{
		return Vector3Database_[keyName];
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

D3DXVECTOR4 Material::GetVector4(string keyName)
{
	if (Vector4Database_.count(keyName))
	{
		return Vector4Database_[keyName];
	}

	return D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}