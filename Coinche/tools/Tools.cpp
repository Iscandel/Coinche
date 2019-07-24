///////////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////////
#include "Tools.h"

#include <boost/tokenizer.hpp>
#include <cctype> 
#include <algorithm>

// Private data
namespace
{
 // Initialize the generator's seed with the current system time
     // in milliseconds, so that it is always different
     unsigned int InitializeSeed()
     {
         unsigned int seed = static_cast<unsigned int>(time(NULL));
         srand(seed);
         return seed;
     }
 
     // Global variable storing the current seed
     unsigned int globalSeed = InitializeSeed();
}

namespace tools
{
	struct my_toLower
	{ 
	    char operator()(char c) const 
	    {
	        return std::tolower(static_cast<unsigned char>(c));
	   }
	};

	//=========================================================================
	///////////////////////////////////////////////////////////////////////////
	std::wstring stringToWString(const std::string& s)
	{
		wchar_t* res = new wchar_t[s.size() + 1];
		mbstowcs(res,s.c_str(),s.size());
		//if(mbstowcs_s(NULL,res,1,s.c_str(),s.size())!= 0)
		//	return L"";
		res[s.size()] = '\0';
		std::wstring res2(res);
		delete res;
		return res2;
	}

	//=========================================================================
	///////////////////////////////////////////////////////////////////////////
	std::vector<std::string> split(const std::string& message, const std::string& separateurs)
	{
		std::vector<std::string> res;

		typedef boost::tokenizer<boost::char_separator<char> > my_tok;

		//Custom separator
		boost::char_separator<char> sep(separateurs.c_str());

		// create custom tokenize
		my_tok tok(message, sep);

		// iterate the token sequence
		for (my_tok::const_iterator i = tok.begin();
			i != tok.end();
			++i)
		{
			res.push_back(*i);
		}

		return res;
	}

	//=============================================================================
	///////////////////////////////////////////////////////////////////////////////
	bool stringToBool(const std::string& value)
	{
		std::string val2 = value;
		std::transform(val2.begin(), val2.end(), val2.begin(), my_toLower());

		return (val2 == "true" || val2 == "1");
	}

} //end namespace tools

namespace color
{
	//=============================================================================
	///////////////////////////////////////////////////////////////////////////////
	void intToARGB(int rgb, int& r, int& g, int& b, int& a)
	{
		a = (rgb >> 24) & 0xFF;
		r = (rgb >> 16) & 0xFF;
		g = (rgb >> 8 ) & 0xFF;
		b = (rgb      ) & 0xFF;
	}

	//=============================================================================
	///////////////////////////////////////////////////////////////////////////////
	int HSBtoRGB(float f, float f1, float f2)
	{
		int i = 0;
		int j = 0;
		int k = 0;
		if(f1 == 0.0F)
		{
			i = j = k = (int)(f2 * 255.0F + 0.5F);
		} 
		else
		{
			float f3 = (f - (float)floor(f)) * 6.0F;
			float f4 = f3 - (float)floor(f3);
			float f5 = f2 * (1.0F - f1);
			float f6 = f2 * (1.0F - f1 * f4);
			float f7 = f2 * (1.0F - f1 * (1.0F - f4));
			switch((int)f3)
			{
			case 0: // '\0'
				i = (int)(f2 * 255.0F + 0.5F);
				j = (int)(f7 * 255.0F + 0.5F);
				k = (int)(f5 * 255.0F + 0.5F);
				break;
			case 1: // '\001'
				i = (int)(f6 * 255.0F + 0.5F);
				j = (int)(f2 * 255.0F + 0.5F);
				k = (int)(f5 * 255.0F + 0.5F);
				break;

			case 2: // '\002'
				i = (int)(f5 * 255.0F + 0.5F);
				j = (int)(f2 * 255.0F + 0.5F);
				k = (int)(f7 * 255.0F + 0.5F);
				break;

			case 3: // '\003'
				i = (int)(f5 * 255.0F + 0.5F);
				j = (int)(f6 * 255.0F + 0.5F);
				k = (int)(f2 * 255.0F + 0.5F);
				break;

			case 4: // '\004'
				i = (int)(f7 * 255.0F + 0.5F);
				j = (int)(f5 * 255.0F + 0.5F);
				k = (int)(f2 * 255.0F + 0.5F);
				break;

			case 5: // '\005'
				i = (int)(f2 * 255.0F + 0.5F);
				j = (int)(f5 * 255.0F + 0.5F);
				k = (int)(f6 * 255.0F + 0.5F);
				break;
			}
		}
		return -16777216 | i << 16 | j << 8 | k << 0;
	}

	//=============================================================================
	///////////////////////////////////////////////////////////////////////////////
	int HSBtoRGB(int t, int s, int l)
	{
		float f1 = t / 240.0F;
		float f2 = s / 240.0F;
		float f3 = l / 120.0F;

		return HSBtoRGB(f1, f2, f3);
	}

} //end color namespace