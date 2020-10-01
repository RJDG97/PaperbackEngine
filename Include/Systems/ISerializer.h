#pragma once

#ifndef _ISERIALIZER_H_
#define _ISERIALIZER_H_

#include <string>

class ISerializer {
public:


	///Open the serialization stream from a file.
	virtual bool Open(const std::string& file)=0;
	///Does the stream still have data?
	virtual bool IsGood()=0;
	virtual void ReadInt(int& i)=0;
	virtual void ReadFloat(float& f)=0;
	virtual void ReadString(std::string& str)=0;

	virtual ~ISerializer() = default;
};

template <typename type>
void ReadStream(ISerializer& stream, type& type_instance);

void ReadStream(ISerializer& stream, float& f);

void ReadStream(ISerializer& stream, int& i);

void ReadStream(ISerializer& stream, std::string& s);

#endif