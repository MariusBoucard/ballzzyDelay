#include <thread>


class Mappers
{
private:
	Mappers()
		: mSampleRate(44100.0) {} 
	Mappers(const Mappers&) = delete; 
	Mappers& operator=(const Mappers&) = delete;

public:
	static Mappers& getMapperInstance()
	{
		static Mappers instance;
		return instance;
	}
	static void setGain(double& inGainSetup, double gain)
	{
		inGainSetup = gain;
	}
	static void setSampleRate(double sampleRate)
	{
		getMapperInstance().mSampleRate = sampleRate;
		// TODO Set reload all
	}


	// static void setToneStackMid(recursive_linear_filter::BiquadParams& inMidSetup, const double val) {
	// }

private:
	double mSampleRate;
};
