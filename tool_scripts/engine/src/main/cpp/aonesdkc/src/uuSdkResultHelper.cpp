#include "uuSdkResultHelper.h"
#include "xcore_json.h"

string uuSdkResultHelper::getJsonObject( string jsonData, string name )
{
	if (jsonData == "" || name == "")
		return "";

	XJsonValue value;
	XJsonReader reader;
	
	if (reader.parse(jsonData, value)) {
		if (value.type() == XJsonValue::objectValue) {
			XJsonValue::ObjectValues objectValues = value.as_objects();
			for (XJsonValue::ObjectIterator it = objectValues.begin(); it != objectValues.end(); ++it) {
				if (it->second.type() == XJsonValue::stringValue)
				{
					if (it->first == name) {
						return it->second.as_string();
					}
				}
			}
		}
	}

	return "";
}

