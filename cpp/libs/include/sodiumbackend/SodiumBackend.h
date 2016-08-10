
#ifndef SODIUMBACKEND_SODIUMBACKEND_H
#define SODIUMBACKEND_SODIUMBACKEND_H


namespace openpal
{

class SodiumBackend
{

public:

	static bool Initialize();
	
private:

	SodiumBackend() = delete;
	SodiumBackend(const SodiumBackend&) = delete;

};

}

#endif
