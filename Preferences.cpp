#include <fstream>
#include <streambuf>

#include "Preferences.h"
#include "Logging.h"

Preferences* Preferences::Instance = 0;

void Preferences::Load()
{
	LOG_INFO("Loading Preferences from file: ");

	// Load the file as json
	json_error_t error;
	json_t* root = json_load_file("prefs.txt", 0, &error);
	if (!root)
	{
		LOG_ERROR("Preferences.Load: error on line " << error.line << ":" << error.text);
		return;
	}

	// Read properties
	ReadVector2(root, "boundsTopLeft", m_boundsTopLeft);
	ReadVector2(root, "boundsTopRight", m_boundsTopRight);
	ReadVector2(root, "boundsBottomRight", m_boundsBottomRight);
	ReadVector2(root, "boundsBottomLeft", m_boundsBottomLeft);

	ReadFloat(root, "totalFadeTimeMS", m_totalFadeTimeMS);

	ReadFloat(root, "camBrightness", m_camBrightness);
	ReadFloat(root, "camContrast", m_camContrast);
	ReadFloat(root, "camSaturation", m_camSaturation);
	ReadFloat(root, "camGain", m_camGain);

	// Cleanup
	json_decref(root);

	// Call save once, this add new properties to the prefs file
	Save();
}

json_t* Preferences::JsonEncode() const
{
	// Create new empty json object
	json_t* root = json_object();

	// Add to root json object
	json_object_set_new(root, "boundsTopLeft", json_vector2(m_boundsTopLeft));
	json_object_set_new(root, "boundsTopRight", json_vector2(m_boundsTopRight));
	json_object_set_new(root, "boundsBottomRight", json_vector2(m_boundsBottomRight));
	json_object_set_new(root, "boundsBottomLeft", json_vector2(m_boundsBottomLeft));
	json_object_set_new(root, "totalFadeTimeMS", json_real(m_totalFadeTimeMS));
	json_object_set_new(root, "camBrightness", json_real(m_camBrightness));
	json_object_set_new(root, "camContrast", json_real(m_camContrast));
	json_object_set_new(root, "camSaturation", json_real(m_camSaturation));
	json_object_set_new(root, "camGain", json_real(m_camGain));

	return root;
}

void Preferences::Save() const
{
	LOG_INFO("Saving Preferences");

	// Write json to file
	json_t* root = JsonEncode();
	json_dump_file(root, "prefs.txt", 0);
	json_decref(root);
}

void Preferences::GetBounds(Vector2& topLeft, Vector2& topRight, Vector2& bottomRight, Vector2& bottomLeft) const
{
	topLeft = m_boundsTopLeft;
	topRight = m_boundsTopRight;
	bottomRight = m_boundsBottomRight;
	bottomLeft = m_boundsBottomLeft;
}

void Preferences::SetBounds(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft)
{
	LOG_INFO("Preferences: bounds set");
	LOG_DEBUG(" - m_boundsTopLeft = (" << topLeft.X() << "," << topLeft.Y() << ")");
	LOG_DEBUG(" - m_boundsTopRight = (" << topRight.X() << "," << topRight.Y() << ")");
	LOG_DEBUG(" - m_boundsBottomRight = (" << bottomRight.X() << "," << bottomRight.Y() << ")");
	LOG_DEBUG(" - m_boundsBottomLeft = (" << bottomLeft.X() << "," << bottomLeft.Y() << ")");

	m_boundsTopLeft = topLeft;
	m_boundsTopRight = topRight;
	m_boundsBottomRight = bottomRight;
	m_boundsBottomLeft = bottomLeft;

	Save();
}

void Preferences::ReadVector2(const json_t *root, const char* propertyName, Vector2& dest)
{
	json_t* jVec2 = json_object_get(root, propertyName);
	if (!jVec2)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
	}

	if (json_is_vector2(jVec2))
    {
		dest = json_vector2_value(jVec2);
		LOG_DEBUG(" - " << propertyName << " = (" << dest.X() << "," << dest.Y() << ")");
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not a vector2");
	}
}

void Preferences::ReadInt(const json_t *root, const char* propertyName, int& dest)
{
	json_t* jInt = json_object_get(root, propertyName);
	if (!jInt)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
	}

	if (json_is_integer(jInt))
    {
		dest = (int)json_integer_value(jInt);
		LOG_DEBUG(" - " << propertyName << " = " << dest);
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not an integer");
	}
}

void Preferences::ReadFloat(const json_t *root, const char* propertyName, float& dest)
{
	json_t* jReal = json_object_get(root, propertyName);
	if (!jReal)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
	}

	if (json_is_real(jReal))
    {
		dest = (float)json_real_value(jReal);
		LOG_DEBUG(" - " << propertyName << " = " << dest);
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not a float");
	}
}