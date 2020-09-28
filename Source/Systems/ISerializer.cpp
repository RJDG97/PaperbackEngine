#include "Systems/ISerializer.h"
#include "document.h"
#include "filereadstream.h"

template <typename type>
void ReadStream(ISerializer& stream, type& type_instance) {

	type_instance.Serialize(stream);
}

void ReadStream(ISerializer& stream, float& f) {

	stream.ReadFloat(f);
}

void ReadStream(ISerializer& stream, int& i) {

	stream.ReadInt(i);
}

void ReadStream(ISerializer& stream, std::string& s) {

	stream.ReadString(s);
}